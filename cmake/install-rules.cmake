install(
    TARGETS cardcrank-cpp_exe
    RUNTIME COMPONENT cardcrank-cpp_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
