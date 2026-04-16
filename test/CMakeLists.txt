# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(cardcrank-cppTests LANGUAGES CXX)

# ---- Dependencies ----

enable_testing()
find_package(GTest CONFIG REQUIRED)

# ---- Tests ----

add_executable(cardcrank-cpp_test source/cardcrank-cpp_test.cpp)
target_link_libraries(
    cardcrank-cpp_test PRIVATE
    cardcrank-cpp_lib
    GTest::gmock_main
)
target_compile_features(cardcrank-cpp_test PRIVATE cxx_std_20)

include(GoogleTest)

add_test(CrankTests crank_test)

# ---- End-of-file commands ----

add_folders(Test)
