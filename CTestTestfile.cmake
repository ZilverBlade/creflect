# CMake generated Testfile for 
# Source directory: D:/Directories/Documents/Visual Studio 2019/Projects/creflect
# Build directory: D:/Directories/Documents/Visual Studio 2019/Projects/creflect
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("D:/Directories/Documents/Visual Studio 2019/Projects/creflect/testSuite[1]_include.cmake")
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(testSuite "D:/Directories/Documents/Visual Studio 2019/Projects/creflect/bin/win-x64/test/Debug/testSuite.exe")
  set_tests_properties(testSuite PROPERTIES  _BACKTRACE_TRIPLES "D:/Directories/Documents/Visual Studio 2019/Projects/creflect/tests.cmake;58;add_test;D:/Directories/Documents/Visual Studio 2019/Projects/creflect/tests.cmake;0;;D:/Directories/Documents/Visual Studio 2019/Projects/creflect/CMakeLists.txt;74;include;D:/Directories/Documents/Visual Studio 2019/Projects/creflect/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(testSuite "D:/Directories/Documents/Visual Studio 2019/Projects/creflect/bin/win-x64/test/Release/testSuite.exe")
  set_tests_properties(testSuite PROPERTIES  _BACKTRACE_TRIPLES "D:/Directories/Documents/Visual Studio 2019/Projects/creflect/tests.cmake;58;add_test;D:/Directories/Documents/Visual Studio 2019/Projects/creflect/tests.cmake;0;;D:/Directories/Documents/Visual Studio 2019/Projects/creflect/CMakeLists.txt;74;include;D:/Directories/Documents/Visual Studio 2019/Projects/creflect/CMakeLists.txt;0;")
else()
  add_test(testSuite NOT_AVAILABLE)
endif()
subdirs("_deps/googletest-build")
