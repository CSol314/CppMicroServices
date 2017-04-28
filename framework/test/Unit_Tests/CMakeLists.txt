#-----------------------------------------------------------------------------
# Link to GTest Libraries
#-----------------------------------------------------------------------------


if(GTEST_INCLUDE_DIRS)
  set(tests
    AnyTest_Converted.cpp
    LogTest_Converted.cpp
)
  include_directories(${GTEST_INCLUDE_DIRS})
  add_executable(UnitTests main.cpp tests)
  target_link_libraries(UnitTests ${GTEST_BOTH_LIBRARIES})
  target_link_libraries(UnitTests ${Framework_TARGET})
endif()
