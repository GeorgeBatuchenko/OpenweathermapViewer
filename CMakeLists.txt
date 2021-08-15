cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME OpenweathermapViewer)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(QT NAMES Qt5 COMPONENTS Core Quick LinguistTools Test Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools Test Network REQUIRED)

set(TS_FILES ${PROJECT_NAME}_ru_RU.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/components/currentweather/currentweathermodel.cpp
        src/components/currentweather/currentweathermodel.h
        src/components/openweathermapapiclient/openweathermapapiclient.cpp
        src/components/openweathermapapiclient/openweathermapapiclient.h
        src/components/openweathermapapiclient/abstractopenweathermapapiclient.cpp
        src/components/openweathermapapiclient/abstractopenweathermapapiclient.h
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

add_subdirectory(test)

