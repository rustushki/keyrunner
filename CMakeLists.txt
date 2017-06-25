# Minimum CMake Version.
cmake_minimum_required (VERSION 3.0.1)

# Project Name.
project (keyrunner)

# Source the CMake scripts capable of finding SDL2.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${keyrunner_SOURCE_DIR}/cmake")

add_library(uitk STATIC
  src/uitk/Animation.cpp
  src/uitk/ButtonLayer.cpp
  src/uitk/ButtonLayerBuilder.cpp
  src/uitk/Layer.cpp
  src/uitk/SpriteSheet.cpp
  src/uitk/View.hpp
)

# Source Files.
set(SOURCES
  src/controller/BaseController.cpp
  src/controller/Controller.hpp
  src/controller/Display.cpp
  src/controller/EditController.cpp
  src/controller/Options.cpp
  src/controller/PlayController.cpp
  src/controller/main.cpp
  src/model/ButtonModel.cpp
  src/model/EditorModel.cpp
  src/model/LevelManager.cpp
  src/model/PlayModel.cpp
  src/model/TileType.cpp
  src/uitk/BaseView.cpp
  src/view/AnimationFactory.cpp
  src/view/BoardView.cpp
  src/view/EditInfoBarLayer.cpp
  src/view/GridLayer.cpp
  src/view/InfoBarLayer.cpp
  src/view/PlayInfoBarView.cpp
  src/view/TileLayer.cpp
  src/view/TileSelectorLayer.cpp
)

# Declare Executable.
add_executable(keyrunner ${SOURCES})

# Define CWD for KeyRunner.
add_definitions(-D_ROOT_="${CMAKE_INSTALL_PREFIX}/share/keyrunner")

target_link_libraries(keyrunner uitk)

# Define Path Separator depending on OS.
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	add_definitions(-DPATHSEP="/")
elseif(CMAKE_SYSTEM_NAME strequal "Windows")
	add_definitions(-DPATHSEP="\\")
endif()

# Include SDL.
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
	include_directories(${SDL2_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL2_LIBRARY})
endif()

# Include SDL Image.
find_package(SDL2_image REQUIRED)
if (SDL2_IMAGE_FOUND)
	include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL2_IMAGE_LIBRARIES})
endif()

# Include SDL TTF.
find_package(SDL2_ttf REQUIRED)
if (SDL2_TTF_FOUND)
	include_directories(${SDL2_TTF_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL2_TTF_LIBRARIES})
endif()

# Require C++ 11 Support.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Add Debug Flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Installation Directives.
install (TARGETS keyrunner DESTINATION bin)
install (DIRECTORY img level font DESTINATION share/keyrunner)
install()
