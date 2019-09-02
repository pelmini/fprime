####
# F prime CMakeLists.txt:
#
# SOURCE_FILES: combined list of source and autocoding diles
# MOD_DEPS: (optional) module dependencies
#
####
set(SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/LedBlinkerComponentAi.xml"
  "${CMAKE_CURRENT_LIST_DIR}/LedBlinker.cpp"
)
# Check the Linux build
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND SOURCE_FILES "${CMAKE_CURRENT_LIST_DIR}/LedBlinkerLinux.cpp")
endif()
register_fprime_module()
