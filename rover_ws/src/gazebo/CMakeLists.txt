cmake_minimum_required(VERSION 3.8)
project(gazebo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)

add_executable(sensor src/sensors.cpp)
ament_target_dependencies(sensor rclcpp sensor_msgs)

install(TARGETS
  sensor
  DESTINATION lib/${PROJECT_NAME})

install(
  DIRECTORY launch model
  DESTINATION share/${PROJECT_NAME}
)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
 
  set(ament_cmake_copyright_FOUND TRUE)
  
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
