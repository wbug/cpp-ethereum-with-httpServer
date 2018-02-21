
string(MAKE_C_IDENTIFIER ${PROJECT_NAME} prefix)
set(buildinfo_library_name ${prefix}_buildinfo)

if(NOT TARGET ${buildinfo_library_name})
    set(version ${PROJECT_VERSION}/${CMAKE_CXX_COMPILER_ID})


    message(STATUS "VERSION: ${prefix}: ${PROJECT_VERSION}")

    set(name ${prefix}_buildinfo)

    configure_file(${CMAKE_CURRENT_LIST_DIR}/buildinfo.h buildinfo.h @ONLY)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/buildinfo.c buildinfo.c @ONLY)

    add_library(${buildinfo_library_name} ${CMAKE_CURRENT_BINARY_DIR}/buildinfo.c)
    target_include_directories(${buildinfo_library_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
endif()
