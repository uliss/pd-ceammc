find_program(DOCKER_EXE docker)

if(DOCKER_EXE)
    file(GLOB _DOCKER_BUILD_SCRIPTS "*.sh")
    add_custom_target(docker_build_scripts SOURCES ${_DOCKER_BUILD_SCRIPTS})

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ubuntu_16.04.sh
                   ${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_16.04.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ubuntu_17.10.sh
                   ${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_17.10.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/debian_9.4.sh
                   ${CMAKE_BINARY_DIR}/dist/docker/docker_debian_9.4.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ubuntu_18.04.sh
                   ${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_18.04.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ubuntu_18.10.sh
                   ${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_18.10.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ubuntu_19.04.sh
                   ${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_19.04.sh @ONLY)

    add_custom_target(docker_debian9
        COMMAND "${CMAKE_BINARY_DIR}/dist/docker/docker_debian_9.4.sh"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_custom_target(docker_ubuntu_1604
        COMMAND "${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_16.04.sh"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_custom_target(docker_ubuntu_1710
        COMMAND "${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_17.10.sh"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_custom_target(docker_ubuntu_1804
        COMMAND "${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_18.04.sh"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_custom_target(docker_ubuntu_1810
        COMMAND "${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_18.10.sh"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_custom_target(docker_ubuntu_1904
        COMMAND "${CMAKE_BINARY_DIR}/dist/docker/docker_ubuntu_19.04.sh"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
