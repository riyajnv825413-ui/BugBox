# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BugGenerator3000_autogen"
  [[CMakeFiles\BugGenerator3000_autogen.dir\AutogenUsed.txt]]
  [[CMakeFiles\BugGenerator3000_autogen.dir\ParseCache.txt]]
  )
endif()
