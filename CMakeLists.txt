cmake_minimum_required(VERSION 3.8.2)

project(vrwithcmake LANGUAGES CXX)

if (MSVC)
  string(REGEX REPLACE "/W[0-9]" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})	
endif (MSVC)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
find_package(ComputeCpp)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(vrwithcmake main.cpp MainWindow.cpp Camera.cpp GraphicsScene.cpp Sphere.cpp VolumetricRayCasting.ui VolumetricRayCasting.qrc)

# Add the Qt5 Widgets for linking
target_link_libraries(vrwithcmake PRIVATE Qt5::Widgets Qt5::WinMain glm)

add_sycl_to_target(TARGET ${PROJECT_NAME} SOURCES main.cpp Sphere.h Sphere.cpp)