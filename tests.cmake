set(SH_SRC "${CMAKE_SOURCE_DIR}/test")
file(GLOB_RECURSE ENDF6_SRC
      "${SH_SRC}/*.h"
      "${SH_SRC}/*.c")
      
add_executable("test" ${ENDF6_SRC})

foreach(_source IN ITEMS ${ENDF6_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${SH_SRC}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

set_target_properties("test" PROPERTIES
  POSITION_INDEPENDENT_CODE False
  INTERPROCEDURAL_OPTIMIZATION False
)

target_include_directories("test" PRIVATE 
  ${CMAKE_SOURCE_DIR}/src
)

if (WIN32)
set_property(TARGET "test" 
PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug> 
)
endif(WIN32)

set_target_properties("test" PROPERTIES
  OUTPUT_NAME "unitTests"
  ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}/test
  LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}/test
  RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}/test
)
add_test(NAME testSuite COMMAND unitTests)
add_dependencies(test creflect)
target_link_libraries(test creflect)