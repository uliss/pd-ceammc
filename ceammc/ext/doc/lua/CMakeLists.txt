file(GLOB _PDDOC_LUA "*.lua")

add_custom_target(pddoc_lua_files SOURCES
    ${_PDDOC_LUA})

# install extra files
install(FILES
    ${_PDDOC_LUA}
    DESTINATION ${DOC_INSTALL_PATH}/lua
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
