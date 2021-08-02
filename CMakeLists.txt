cmake_minimum_required(VERSION 3.5)

project(custom_allocator)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
project(custom_allocator VERSION ${PROJECT_VESRION})

set(CHUNK_SIZE "10" CACHE INTERNAL "chunk size")

configure_file(
        chunk.h.in 
        ${CMAKE_CURRENT_SOURCE_DIR}/chunk.h
)

add_executable(custom_allocator custom_allocator.cpp custom_list.h)
set_target_properties(custom_allocator PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(custom_allocator PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
set_target_properties(custom_allocator PROPERTIES LINK_FLAGS "-fsanitize=address -fno-omit-frame-pointer")

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)