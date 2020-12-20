file(GLOB _PY_FILES "*.py")
file(GLOB _PY_FILES2 "ceammc/*.py")
add_custom_target(python_files SOURCES ${_PY_FILES} ${_PY_FILES2})
