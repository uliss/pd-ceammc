# Develop

## Faust

### Adding Faust extension

1. Put your faust dsp file info **ceammc/faust** directory.
   File should be named like: **PREFIX_OBJECTNAME.dsp**
2. In this directory run
    ```
    ./generate_skeleton.sh PREFIX OBJECTNAME
    ```
    
    This will generate **ceammc/ext/src/PREFIX_OBJECTNAME.h** and **ceammc/ext/src/PREFIX_OBJECTNAME.cpp** files.

3. Edit **ceammc/ext/src/PREFIX_OBJECTNAME.cpp** for your needs.

4. Add entry to **ceammc/ext/src/CMakeLists.txt**:
   ```
   ceammc_faust_extension(PREFIX OBJECTNAME simple_pd_control_ext)
   ```
5. Run ```make```

Run ```make faust_PREFIX_OBJECTNAME``` to regenerate **ceammc/ext/src/PREFIX_OBJECTNAME.h** every time on faust DSP file change.

