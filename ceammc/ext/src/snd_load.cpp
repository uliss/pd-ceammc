#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_object.h"
#include "ceammc_sound.h"

#include <cassert>

using namespace ceammc;
using namespace ceammc::sound;

class SndLoad : public BaseObject {
public:
    SndLoad(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();

        //        if (!a.args.empty()) {
        //            const Atom* array = a.args.find(isSymbol);
        //            if (array)
        //                array_ = array->asSymbol();
        //        }
        // parseArguments();
    }

public:
    void m_load(t_symbol* sel, const AtomList& lst)
    {
        if (lst.empty()) {
            OBJ_ERR << "arguments required";
            return postLoadUsage();
        }

        // getting filename
        if (!lst.first()->isSymbol()) {
            OBJ_ERR << "filename required";
            return postLoadUsage();
        }

        t_symbol* fname = lst.first()->asSymbol();

        // getting array names
        AtomList array_names;
        lst.property("@to", &array_names);
        if (!array_names.empty()) {
            OBJ_ERR << "destination arrays are not specified";
            return postLoadUsage();
        }

        // check arrays exists
        for (size_t i = 0; i < array_names.size(); i++) {
            if (!checkArray(array_names[i]))
                return;
        }

        // try to open file
        SoundFilePtr ptr = SoundFileLoader::open(fname->s_name);

        if (!ptr) {
            OBJ_ERR << "can't load file: " << fname->s_name;
            return;
        }

        long samples_in_file = ptr->sampleCount();
        if (samples_in_file < 1) {
            OBJ_ERR << "can't load file: " << fname->s_name;
            return;
        }

        const bool resize = lst.hasProperty("@resize");
        // resize all arrays
        if (resize) {
            for (size_t i = 0; i < array_names.size(); i++) {
                if (!resizeArray(array_names[i], samples_in_file))
                    return;
            }
        }

        // channel property
        // filled with zeroes
        std::vector<size_t> channels(array_names.size(), 0);
        AtomList prop_chan;
        lst.property("@channel", &prop_chan);

        // if no @channel specified
        if (prop_chan.empty()) {
            // fill with ascending values: 0, 1, 2, etc.
            for (size_t i = 0; i < array_names.size(); i++) {
                channels[i] = i;
            }
        } else {
            assert(array_names.size() == channels.size());

            if (prop_chan.size() > array_names.size()) {
                OBJ_DBG << "WARNING: number of channels is greater then number of arrays";
            }

            size_t total = std::min(array_names.size(), prop_chan.size());
            for (size_t i = 0; i < total; i++) {
                size_t chan = prop_chan.asSizeT(0);
                if (chan >= ptr->channels()) {
                    OBJ_ERR << "invalid channel specified: " << chan << ". Skipping...";
                    continue;
                }

                channels[i] = chan;
            }
        }

        // offset property
        long offset = 0;
        Atom prop_offset;
        if (lst.property("@offset", &prop_offset)) {
            offset = prop_offset.asInt(0);
            if (offset >= long(ptr->sampleCount())) {
                OBJ_ERR << "invalid offset specified: " << offset;
                OBJ_ERR << "should be less then " << ptr->sampleCount() << ". setting it to 0.";
                offset = 0;
            } else if (offset < 0) {
                offset = std::max<long>(0, long(ptr->sampleCount()) + offset);
                OBJ_DBG << "offset from file end: " << offset;
            }
        }

        // load data
        long bytes = 0;
        for (size_t i = 0; i < array_names.size(); i++) {
            bytes = loadArray(ptr, array_names[i], channels[i], offset);
            if (bytes < 1)
                return;
        }

        floatTo(0, bytes);
    }

    void m_info(t_symbol* sel, const AtomList& lst)
    {
    }

private:
    void postLoadUsage()
    {
        OBJ_DBG << "usage: load FILENAME @to ARRAY1 [ARRAY2] [@resize] [@channel value1 [value2]] [@offset value]";
    }

    t_garray* findArray(const Atom& name)
    {
        if (!name.isSymbol())
            return 0;

        t_symbol* array = name.asSymbol();
        return reinterpret_cast<t_garray*>(pd_findbyclass(array, garray_class));
    }

    bool checkArray(const Atom& name)
    {
        t_garray* arr = findArray(name);
        if (!arr) {
            OBJ_ERR << "no such array: " << to_string(name);
            return false;
        }

        int vecsize = 0;
        t_word* vecs;
        if (!garray_getfloatwords(arr, &vecsize, &vecs)) {
            OBJ_ERR << "invalid array template: " << to_string(name);
            return false;
        }

        return true;
    }

    bool resizeArray(const Atom& name, long newSize)
    {
        t_garray* arr = findArray(name);
        if (!arr) {
            OBJ_ERR << "no such array: " << to_string(name);
            return false;
        }

        int vecsize = 0;
        t_word* vecs;
        if (!garray_getfloatwords(arr, &vecsize, &vecs))
            return false;

        if (vecsize != newSize) {
            garray_resize_long(arr, newSize);
            /* for sanity's sake let's clear the save-in-patch flag here */
            garray_setsaveit(arr, 0);

            if (!garray_getfloatwords(arr, &vecsize, &vecs)
                /* if the resize failed, garray_resize reported the error */
                || (vecsize != newSize)) {
                OBJ_ERR << "resize of " << to_string(name) << " to " << newSize << " failed";
                return false;
            }
        }

        garray_redraw(arr);
        return true;
    }

    long loadArray(SoundFilePtr file, const Atom& name, size_t channel, long offset)
    {
        if (!file) {
            OBJ_ERR << "invalid file: " << file->filename();
            return -1;
        }

        t_garray* arr = findArray(name);
        if (!arr) {
            OBJ_ERR << "no such array: " << to_string(name);
            return -1;
        }

        int vecsize = 0;
        t_word* vecs;
        if (!garray_getfloatwords(arr, &vecsize, &vecs))
            return -1;

        long read = file->read(vecs, vecsize, channel, offset);

        if (read == -1) {
            OBJ_ERR << "load error: " << file->filename() << " to " << to_string(name);
            return -1;
        }

        garray_redraw(arr);

        return read;
    }
};

extern "C" void setup_snd0x2eload()
{
    ObjectFactory<SndLoad> obj("snd.load");
    obj.addMethod("load", &SndLoad::m_load);
    obj.addMethod("info", &SndLoad::m_info);
}
