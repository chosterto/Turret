cmake_minimum_required(VERSION 3.18.4)

project(turret)


if (UNIX AND NOT APPLE)
	add_executable(
		turret_manual
		#test.cpp
		manual_keyboard.cpp
		servo.cpp
		)

	find_library(WIRINGPI_LIBS NAMES wiringPi)
	target_link_libraries(turret_manual ${WIRINGPI_LIBS})
endif()
