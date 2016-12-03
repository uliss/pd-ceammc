# Develop

## Faust

### Adding Faust extension

1. put your faust dsp file info *ceammc/faust* directory.
   file should be named like: PREFIX_OBJECTNAME.dsp
2. create *ceammc/ext/src/PREFIX_OBJECTNAME.cpp*
    with following content:

    ```
    #include "${PREFIX}_${OBJECTNAME}.h"

    extern "C" void setup_${PREFIX}0x2e${OBJECTNAME}_tilde()
    {
        t_symbol* s = gensym("${PREFIX}.${OBJECTNAME}~");
        faust_class = class_new(s, reinterpret_cast<t_newmethod>(faust_new),
            reinterpret_cast<t_method>(faust_free),
            sizeof(t_faust),
            CLASS_DEFAULT,
            A_GIMME, A_NULL);
        // ....
        // ... some code
        // ....
        class_addmethod(faust_class, nullfn, &s_signal, A_NULL);
        class_addmethod(faust_class, reinterpret_cast<t_method>(faust_dsp), gensym("dsp"), A_NULL);
        CLASS_MAINSIGNALIN(faust_class, t_faust, f);

        s_button = gensym("button");
        s_checkbox = gensym("checkbox");
        s_vslider = gensym("vslider");
        s_hslider = gensym("hslider");
        s_nentry = gensym("nentry");
        s_vbargraph = gensym("vbargraph");
        s_hbargraph = gensym("hbargraph");
    }
    ```

3. add entry to *ceammc/ext/src/CMakeLists.txt*:
   ```
   ceammc_faust_extension(PREFIX OBJECTNAME simple_pd_control_ext)
   ```
4. run ```make faust_PREFIX_OBJECTNAME``` to generate *ceammc/ext/src/PREFIX_OBJECTNAME.h*
5. run ```make```

