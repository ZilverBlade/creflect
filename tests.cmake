cmake_minimum_required(VERSION 3.14)
enable_testing()
set(SH_SRC "${CMAKE_SOURCE_DIR}/test")
file(GLOB_RECURSE ENDF6_SRC
      "${SH_SRC}/*.h"
      "${SH_SRC}/*.cpp")
      
add_executable("testSuite" ${ENDF6_SRC})
include(GoogleTest)
gtest_discover_tests(testSuite)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # release-1.17.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

foreach(_source IN ITEMS ${ENDF6_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${SH_SRC}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

set_target_properties("testSuite" PROPERTIES
  POSITION_INDEPENDENT_CODE False
  INTERPROCEDURAL_OPTIMIZATION False
)

target_include_directories("testSuite" PRIVATE 
  ${CMAKE_SOURCE_DIR}/test
  ${CMAKE_SOURCE_DIR}/src
  ${gtest_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR}
)

if (WIN32)
set_property(TARGET "testSuite" 
PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug> 
)
endif(WIN32)

set_target_properties("testSuite" PROPERTIES
  OUTPUT_NAME "testSuite"
  ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}/test
  LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}/test
  RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}/test
)
add_test(NAME testSuite COMMAND testSuite)
add_dependencies(testSuite creflect)
target_link_libraries(testSuite
  creflect
  GTest::gtest_main
  )
