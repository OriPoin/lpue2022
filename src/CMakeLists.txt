add_library(compute SHARED compute.cc)
target_link_libraries(compute ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(compute_bin main.cc)
set_target_properties(compute_bin PROPERTIES OUTPUT_NAME compute)
target_link_libraries(compute_bin compute)