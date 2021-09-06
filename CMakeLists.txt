cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME OpenweathermapViewer)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(READ ../apikey.key OWM_API_KEY)

enable_testing()

find_package(QT NAMES Qt5 COMPONENTS Core Quick LinguistTools Test Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools Test Network REQUIRED)

set(TS_FILES ${PROJECT_NAME}_ru_RU.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/components/currentweather/currentweathermodel.cpp
        src/components/currentweather/currentweathermodel.h
        src/components/dailyforecast/dailyforecastmodel.cpp
        src/components/dailyforecast/dailyforecastmodel.h
        src/components/hourlyforecast/hourlyforecastmodel.cpp
        src/components/hourlyforecast/hourlyforecastmodel.h
        src/pages/cities/citieslistmodel.cpp
        src/pages/cities/citieslistmodel.h
        src/modules/openweathermapapiclient/openweathermapapiclient.cpp
        src/modules/openweathermapapiclient/openweathermapapiclient.h
        src/modules/openweathermapapiclient/abstractopenweathermapapiclient.cpp
        src/modules/openweathermapapiclient/abstractopenweathermapapiclient.h
        src/modules/appsettings/appsettings.h
        src/modules/appsettings/appsettings.cpp
        src/modules/appsettings/abstractappsettings.h
        src/modules/appsettings/abstractappsettings.cpp
        qml.qrc
        ${TS_FILES}
)

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME}
    ${PROJECT_SOURCES}
)

#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_compile_definitions(${TARGET_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${TARGET_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network)

configure_file(appsettings.ini.in ${CMAKE_BINARY_DIR}/appsettings.ini @ONLY)

add_subdirectory(test)


# INSTALATION (tested on debian:)
# Instalation used for cpack DEB package generation QT packages must be installed by apt
# dependancies: qt5-default qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qttools5-dev

#${CMAKE_BINARY_DIR}/deb

install(
	TARGETS ${TARGET_NAME}
	DESTINATION /opt/openweathermapviewer
)

install(
	FILES ${CMAKE_BINARY_DIR}/appsettings.ini
	DESTINATION /opt/openweathermapviewer
)

install(
	PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/inst/OpenweathermapViewer.sh
	DESTINATION /opt/openweathermapviewer
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/inst/OpenweathermapViewer.desktop
	DESTINATION /usr/share/applications/openweathermapviewer
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/images/openweathermapviewer.png
	DESTINATION /usr/share/openweathermapviewer
)

