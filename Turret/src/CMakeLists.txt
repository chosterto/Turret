cmake_minimum_required(VERSION 3.18.4)

project(turret)


if (UNIX AND NOT APPLE)
	add_executable(
		turret_manual
		#test.cpp
		manual_keyboard.cpp
		servo.cpp
	)
	add_executable(
		turret_color
		color_track.cpp
		servo.cpp
		camera.cpp
	)
	add_executable(
		turret_aruco
		aruco_detect.cpp
		camera.cpp
	)

	find_library(WIRINGPI_LIBS NAMES wiringPi)

	find_package(OpenCV REQUIRED)
	include_directories(${OpenCV_INCLUDE_DIRS})

	target_link_libraries(turret_manual ${WIRINGPI_LIBS})
	target_link_libraries(turret_color ${WIRINGPI_LIBS} ${OpenCV_LIBS})
	target_link_libraries(turret_aruco ${WIRINGPI_LIBS} ${OpenCV_LIBS})

endif()
