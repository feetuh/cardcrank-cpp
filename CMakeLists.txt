cmake_minimum_required(VERSION 3.14)

# Export compile commands for IDE/LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/prelude.cmake)

project(
    cardcrank-cpp
    VERSION 0.1.0
    DESCRIPTION "cpp backend for cardcrank"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    cardcrank-cpp_lib OBJECT
    source/crank/lib.cpp
    source/crank/card.cpp
    source/crank/deck.cpp
    source/crank/table.cpp
    source/crank/rules.cpp
)

target_include_directories(
    cardcrank-cpp_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_compile_features(cardcrank-cpp_lib PUBLIC cxx_std_20)

# find_package(fmt REQUIRED)
# target_link_libraries(cardcrank-cpp_lib PRIVATE fmt::fmt)

# ---- Declare executable ----

add_executable(cardcrank-cpp_exe source/main.cpp)
add_executable(cardcrank-cpp::exe ALIAS cardcrank-cpp_exe)

set_property(TARGET cardcrank-cpp_exe PROPERTY OUTPUT_NAME cardcrank-cpp)

target_compile_features(cardcrank-cpp_exe PRIVATE cxx_std_20)

target_link_libraries(cardcrank-cpp_exe PRIVATE cardcrank-cpp_lib)

# ---- Tests ----

option(BUILD_TESTING "Build the testing tree." ON)
if(BUILD_TESTING AND NOT cardcrank-cpp_DEVELOPER_MODE)
  add_subdirectory(test)
endif()

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT cardcrank-cpp_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of cardcrank-cpp"
  )
endif()

include(cmake/dev-mode.cmake)
