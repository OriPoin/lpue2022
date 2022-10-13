add_executable(copy_test copy_test.cc)
target_link_libraries(copy_test copy ${Boost_LIBRARIES})
add_test(NAME copy_test COMMAND copy_test)