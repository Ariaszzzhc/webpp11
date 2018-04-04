cmake_minimum_required(VERSION 3.10)
project(webpp_examples)

add_executable(hello hello.cc)
target_link_libraries(hello ${Boost_LIBRARIES})