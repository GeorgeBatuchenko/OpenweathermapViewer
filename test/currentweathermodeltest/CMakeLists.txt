cmake_minimum_required(VERSION 3.14)

set(TEST_PROJECT_NAME CurrentWeatherModelTest)

project(${TEST_PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_PROJECT_SOURCES
	main.cpp
	../../src/components/currentweather/currentweathermodel.cpp
	../../src/components/currentweather/currentweathermodel.h
	../../src/components/openweathermapapiclient/abstractopenweathermapapiclient.cpp
	../../src/components/openweathermapapiclient/abstractopenweathermapapiclient.h
)

add_definitions(-DDATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
set(TEST_TARGET_NAME ${TEST_PROJECT_NAME})

add_executable(${TEST_PROJECT_NAME} ${TEST_PROJECT_SOURCES})
add_test(
	NAME ${TEST_TARGET_NAME}
	COMMAND ${TEST_TARGET_NAME}
)

target_link_libraries(${TEST_TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Test)

target_include_directories(${TEST_TARGET_NAME} PRIVATE
	../../src
)

