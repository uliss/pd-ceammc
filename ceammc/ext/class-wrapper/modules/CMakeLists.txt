set(WRAPPER_MODULES
    matrix/iface_matrix.h
    numeric/iface_numeric.h)


add_custom_target(wrapper_lib_modules SOURCES ${WRAPPER_MODULES})
