#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_sound.h"

using namespace ceammc;
using namespace ceammc::sound;

class SndLoad : public BaseObject {
private:
    t_symbol* array_;

public:
    SndLoad(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();

        if (!a.args.empty()) {
            const Atom* array = a.args.find(isSymbol);
            if (array)
                array_ = array->asSymbol();
        }
        // parseArguments();
    }

public:
    void m_load(t_symbol* sel, const AtomList& lst)
    {
        if (lst.empty()) {
            OBJ_ERR << "argument required";
            return;
        }

        t_symbol* fname = lst.first()->asSymbol();
        SoundFilePtr ptr = SoundFileLoader::open(fname->s_name);

        if (!ptr) {
            OBJ_ERR << "can't load this format: " << fname->s_name;
            return;
        }

        t_garray* arr = (t_garray*)pd_findbyclass(array_, garray_class);
        if (!arr) {
            OBJ_ERR << "no such table: " << array_->s_name;
            return;
        }

        int vecsize = 0;
        t_word* vecs;
        if (!garray_getfloatwords(arr, &vecsize, &vecs)) {
            OBJ_ERR << "bad template for tabwrite: " << array_->s_name;
            return;
        }

        long samples_in_file = ptr->sampleCount();
        if (samples_in_file < 1) {
            OBJ_ERR << "no data in " << fname->s_name;
            return;
        }

        bool resize = true;
        if (resize && (vecsize != samples_in_file)) {
            garray_resize_long(arr, samples_in_file);
            /* ceammc: from d_soundile.c */
            /* for sanity's sake let's clear the save-in-patch flag here */
            garray_setsaveit(arr, 0);

            if (!garray_getfloatwords(arr, &vecsize, &vecs)
                /* if the resize failed, garray_resize reported the error */
                || (vecsize != samples_in_file)) {
                OBJ_ERR << "resize of " << array_->s_name << " to " << samples_in_file << " failed";
                return;
            }
        }

        long read = ptr->read(vecs, vecsize, 0);

        garray_redraw(arr);

        floatTo(0, read);
    }

    void m_info(t_symbol* sel, const AtomList& lst)
    {
    }
};

extern "C" void setup_snd0x2eload()
{
    ObjectFactory<SndLoad> obj("snd.load");
    obj.addMethod("open", &SndLoad::m_load);
    obj.addMethod("info", &SndLoad::m_info);
}
