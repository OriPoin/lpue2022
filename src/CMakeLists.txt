add_library(contentscan SHARED contentscan.cc)
target_link_libraries(contentscan ${Boost_LIBRARIES})

add_executable(contentscan_bin main.cc)
set_target_properties(contentscan_bin PROPERTIES OUTPUT_NAME contentscan)
target_link_libraries(contentscan_bin contentscan)