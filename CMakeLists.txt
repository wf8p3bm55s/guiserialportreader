cmake_minimum_required(VERSION 3.5)

project(guiserialportreader LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick SerialPort REQUIRED)

if(WIN32)
    add_executable(guiserialportreader WIN32
      main.cpp
      backend.cpp
      qml.qrc
    )
else()
    add_executable(guiserialportreader
      main.cpp
      backend.cpp
      qml.qrc
    )
endif()

target_link_libraries(guiserialportreader
    PRIVATE Qt5::Core Qt5::Quick Qt5::SerialPort)
