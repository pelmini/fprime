# Generate unit test
# UT_SOURCE_FILES: Sources for unit test
# UT_MODULES: List of modules linked in unit tests

set(UT_SOURCE_FILES
  "${CMAKE_CURRENT_LIST_DIR}/TimeTest.cpp"
)

set(UT_MODULES
  "${FPRIME_CORE_DIR}/Fw/Time"
  "${FPRIME_CORE_DIR}/Fw/Types"
)

#add_unit_test("${UT_SOURCE_FILES}" "${UT_MODULES}")
