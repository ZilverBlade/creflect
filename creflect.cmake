set(SH_SRC "${CMAKE_SOURCE_DIR}/src")
file(GLOB_RECURSE ENDF6_SRC
      "${SH_SRC}/*.h"
      "${SH_SRC}/*.c")
      
add_library("creflect" ${ENDF6_SRC})

foreach(_source IN ITEMS ${ENDF6_SRC})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${SH_SRC}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

set_target_properties("creflect" PROPERTIES
  POSITION_INDEPENDENT_CODE False
  INTERPROCEDURAL_OPTIMIZATION False
)

target_include_directories("creflect" PRIVATE 
  ${CMAKE_SOURCE_DIR}/src
)

if (WIN32)
set_property(TARGET "creflect" 
PROPERTY MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>DLL
)
endif(WIN32)

set_target_properties("creflect" PROPERTIES
  OUTPUT_NAME "creflect"
  ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}
  LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}
  RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}
)
target_compile_definitions(creflect PRIVATE creflect_src)