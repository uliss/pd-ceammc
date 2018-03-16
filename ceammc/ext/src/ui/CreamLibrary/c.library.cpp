/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "c.library.hpp"

static const char creamversion[] = "Beta 0.4 - ceammc";
static t_eclass* cream_class;

extern "C" void libpd_loadcream(void)
{
    cream_setup();
}

static void* cream_new(t_symbol* s)
{
    t_eobj* x = (t_eobj*)eobj_new(cream_class);
    if (x) {

        logpost(x, 3, "Cream Library by Pierre Guillot\n© 2013 - 2015  "
                      "CICM | Paris 8 University\nVersion %s (%s) for Pure Data %i.%i\n",
            creamversion, __DATE__, PD_MAJOR_VERSION, PD_MINOR_VERSION);
    }
    return (x);
}

void cream_setup(void)
{
    cream_class = eclass_new("cream", (method)cream_new, (method)eobj_free, sizeof(t_eobj), CLASS_PD, A_NULL, 0);
    cream_class = eclass_new("Cream", (method)cream_new, (method)eobj_free, sizeof(t_eobj), CLASS_PD, A_NULL, 0);
    t_eobj* obj = (t_eobj*)cream_new(NULL);
    if (!obj) {

        verbose(3, "Cream Library by Pierre Guillot\n© 2013 - 2015  CICM | "
                   "Paris 8 University\nVersion %s (%s) for Pure Data %i.%i\n",
            creamversion, __DATE__, PD_MAJOR_VERSION, PD_MINOR_VERSION);
        eobj_free(obj);
    }

    //    setup_c0x2egain_tilde();

    //CEAMMC
//    setup_ui0x2ebang();
//    setup_ui0x2ecolorpanel();
//    setup_ui0x2edsp_tilde();
//    setup_ui0x2eincdec();
//    setup_ui0x2enumber_tilde();
//    setup_ui0x2enumber();
//    setup_ui0x2ematrix();
    setup_ui0x2emenu();
//    setup_ui0x2emeter_tilde();
//    setup_ui0x2epreset();
//    setup_ui0x2eradio();
//    setup_ui0x2erslider();
    // setup_ui0x2eslider();
//    setup_ui0x2etab();
    // setup_ui0x2etoggle();

#ifdef __APPLE__
//    setup_c0x2ecamomile();
#endif
}
