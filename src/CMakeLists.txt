add_library(copy SHARED copy.cc)
target_link_libraries(copy ${Boost_LIBRARIES})

add_executable(copy_bin main.cc)
set_target_properties(copy_bin PROPERTIES OUTPUT_NAME copy)
target_link_libraries(copy_bin copy)