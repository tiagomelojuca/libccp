cmake_minimum_required(VERSION 3.12)

project(libccp)

set(SRC_DIR                 "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR                "${CMAKE_SOURCE_DIR}/test")

set(SRC_MINNEG              "${SRC_DIR}/minneg")
set(SRC_PUNC                "${SRC_DIR}/punc")

set(INC_DIR                 "${SRC_MINNEG}"
                            "${SRC_PUNC}")

set(OUT_DIR                 "exeobj_cmake")
set(CMAKE_INSTALL_PREFIX    "${CMAKE_BINARY_DIR}/${OUT_DIR}")

# -----------------------------------------------------------------------------

add_library(
    ccp
    SHARED
    "${SRC_MINNEG}/minneg.c"
    "${SRC_PUNC}/punc.c"
)

target_compile_definitions(ccp PRIVATE -DBUILD_DLL)

# -----------------------------------------------------------------------------

add_subdirectory(test)

# -----------------------------------------------------------------------------

install(TARGETS ccp DESTINATION "lib")
install(FILES "${SRC_MINNEG}/minneg.h" DESTINATION "inc")
install(FILES "${SRC_PUNC}/punc.h" DESTINATION "inc")

# deprecated
# install(TARGETS ccp test_minneg test_punc DESTINATION "test")
add_custom_command(TARGET ccp POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   "${CMAKE_INSTALL_PREFIX}/lib/libccp.dll"
                   "${CMAKE_BINARY_DIR}/test/libccp.dll")

# -----------------------------------------------------------------------------
