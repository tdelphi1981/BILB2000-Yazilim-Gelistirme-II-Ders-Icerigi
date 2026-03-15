# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "01_designer_ornek_autogen"
  "02_obys_ana_pencere_autogen"
  "CMakeFiles\\01_designer_ornek_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\01_designer_ornek_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\02_obys_ana_pencere_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\02_obys_ana_pencere_autogen.dir\\ParseCache.txt"
  )
endif()
