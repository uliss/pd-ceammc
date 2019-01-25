file (GLOB WRAPPER_SCRIPTS "*.py" "*.sh" "*.pddoc")

add_custom_target(wrapper_lib_scripts SOURCES ${WRAPPER_SCRIPTS})
