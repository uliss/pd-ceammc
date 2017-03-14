#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_sound.h"

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
            return post_load_usage();
        }

        // getting filename
        if (!lst.first()->isSymbol()) {
            OBJ_ERR << "filename required";
            return post_load_usage();
        }

        t_symbol* fname = lst.first()->asSymbol();

        // getting array names
        Atom array_name;
        if (!lst.property("@to", &array_name)) {
            OBJ_ERR << "array name is not specified";
            return post_load_usage();
        }

        if (!array_name.isSymbol()) {
            OBJ_ERR << "array name should be symbol";
            return post_load_usage();
        }

        t_symbol* array = array_name.asSymbol();
        t_garray* arr = (t_garray*)pd_findbyclass(array, garray_class);
        if (!arr) {
            OBJ_ERR << "no such array: " << array->s_name;
            return;
        }

        int vecsize = 0;
        t_word* vecs;
        if (!garray_getfloatwords(arr, &vecsize, &vecs)) {
            OBJ_ERR << "bad template for tabwrite: " << array->s_name;
            return;
        }

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
        if (resize && (vecsize != samples_in_file)) {
            garray_resize_long(arr, samples_in_file);
            /* ceammc: from d_soundile.c */
            /* for sanity's sake let's clear the save-in-patch flag here */
            garray_setsaveit(arr, 0);

            if (!garray_getfloatwords(arr, &vecsize, &vecs)
                /* if the resize failed, garray_resize reported the error */
                || (vecsize != samples_in_file)) {
                OBJ_ERR << "resize of " << array->s_name << " to " << samples_in_file << " failed";
                return;
            }
        }

        // channel property
        size_t chan = 0;
        Atom prop_chan;
        if (lst.property("@channel", &prop_chan)) {
            chan = prop_chan.asSizeT(0);
            if (chan >= ptr->channels()) {
                OBJ_ERR << "invalid channel specified: " << chan;
                OBJ_ERR << "using channel 0";
                chan = 0;
            }
        }

        // offset property
        long offset = 0;
        Atom prop_offset;
        if (lst.property("@offset", &prop_offset)) {
            offset = prop_offset.asInt(0);
            if (offset >= long(ptr->sampleCount())) {
                OBJ_ERR << "invalid offset specified: " << offset;
                OBJ_ERR << "should be less then " << ptr->sampleCount() << ". setting to 0.";
                offset = 0;
            } else if (offset < 0) {
                offset = std::max<long>(0, long(ptr->sampleCount()) + offset);
                OBJ_DBG << "offset from file end: " << offset;
            }
        }

        long read = ptr->read(vecs, vecsize, chan, offset);

        if (read == -1) {
            OBJ_ERR << "load error: " << fname->s_name << " to " << array->s_name;
            return;
        }

        garray_redraw(arr);

        floatTo(0, read);
    }

    void m_info(t_symbol* sel, const AtomList& lst)
    {
    }

private:
    void post_load_usage()
    {
        OBJ_DBG << "usage: load FILENAME @to ARRAY [@resize] [@channel value] [@offset value]";
    }
};

extern "C" void setup_snd0x2eload()
{
    ObjectFactory<SndLoad> obj("snd.load");
    obj.addMethod("load", &SndLoad::m_load);
    obj.addMethod("info", &SndLoad::m_info);
}
