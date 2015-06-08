# Minimum CMake Version.
cmake_minimum_required (VERSION 3.0.1)

# Project Name.
project (keyrunner)

add_library(uitk STATIC
  src/uitk/Animation.cpp
  src/uitk/ButtonLayer.cpp
  src/uitk/ButtonLayerBuilder.cpp
  src/uitk/Layer.cpp
  src/uitk/SpriteSheet.cpp
)

# Source Files.
set(SOURCES
  src/AnimationFactory.cpp
  src/EditInfoBarLayer.cpp
  src/GridLayer.cpp
  src/KeyRunner.cpp
  src/main.cpp
  src/PlayInfoBarLayer.cpp
  src/RootLayer.cpp
  src/TileLayer.cpp
  src/ConveyorAnimation.cpp
  src/EditRootLayer.cpp
  src/InfoBarLayer.cpp
  src/LevelManager.cpp
  src/Options.cpp
  src/PlayRootLayer.cpp
  src/TileSelectorLayer.cpp
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
find_package(SDL 1.2.0 REQUIRED)
if (SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL_LIBRARY})
endif()

# Include SDL Image.
find_package(SDL_image REQUIRED)
if (SDL_IMAGE_FOUND)
	include_directories(${SDL_IMAGE_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL_IMAGE_LIBRARIES})
endif()

# Include SDL TTF.
find_package(SDL_ttf REQUIRED)
if (SDL_TTF_FOUND)
	include_directories(${SDL_TTF_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL_TTF_LIBRARIES})
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

# Installation Directives.
install (TARGETS keyrunner DESTINATION bin)
install (DIRECTORY img level font DESTINATION share/keyrunner)
install()
