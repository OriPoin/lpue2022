add_executable(compute_test compute_test.cc)
target_link_libraries(compute_test compute ${Boost_LIBRARIES})
add_test(NAME compute_test COMMAND compute_test)