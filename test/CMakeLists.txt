set(FETCHCONTENT_QUIET OFF CACHE BOOL "USE FETCHCONTENT_QUIET" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "USE MSVCRT.DLL" FORCE)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)

# -----------------------------------------------------------------------------

add_executable(test_minneg "test_minneg.cpp")
target_include_directories(test_minneg PRIVATE "${SRC_DIR}/minneg")
target_link_libraries(test_minneg PUBLIC gtest_main ccp)

# -----------------------------------------------------------------------------

add_executable(test_punc "test_punc.cpp")
target_include_directories(test_punc PRIVATE "${SRC_DIR}/punc")
target_link_libraries(test_punc PUBLIC gtest_main ccp)

# -----------------------------------------------------------------------------
