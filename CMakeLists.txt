cmake_minimum_required(VERSION 3.8.2)

project(vrwithcmake LANGUAGES CXX)

if (MSVC)
  string(REGEX REPLACE "/W[0-9]" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})	
endif (MSVC)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
find_package(ComputeCpp)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)

# There may be a way to tell up front if Qt5 is going to be found, but I haven't found
# a foolproof way to do it yet, so settle for the default error message for now.
#if(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)
#    message(WARNING "CMAKE_PREFIX_PATH is not defined, so find_package may not work. Set the CMAKE_PREFIX_PATH "
#            "environment variable to the install prefix of Qt 5, either on the command line as "
#            "-DCMAKE_PREFIX_PATH=\"path/to/Qt5/lib/cmake\" or with set(CMAKE_PREFIX_PATH path/to/Qt5/lib/cmake)")
#endif(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)

# Add a compiler flag

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

#set (CMAKE_PREFIX_PATH "C:\\Qt\\5.11.1\\msvc2017_64\\")

# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(glm CONFIG REQUIRED)

# Generate code from ui files
qt5_wrap_ui(UI_HEADERS VolumetricRayCasting.ui)

# Generate rules for building source files from the resources
qt5_add_resources(QRCS VolumetricRayCasting.qrc)

# Tell CMake to create the helloworld executable
add_executable(vrwithcmake main.cpp MainWindow.cpp Camera.cpp GraphicsScene.cpp Sphere.cpp ${UI_HEADERS} ${QRCS})

# Add the Qt5 Widgets for linking
target_link_libraries(vrwithcmake PUBLIC Qt5::Widgets Qt5::WinMain)
target_link_libraries(vrwithcmake PRIVATE glm)

add_sycl_to_target(TARGET ${PROJECT_NAME} SOURCES main.cpp Sphere.h Sphere.cpp)
