cmake_minimum_required (VERSION 3.0.1)
project (keyrunner)

set(SOURCES
  src/Animation.cpp
  src/ButtonLayer.cpp
  src/EditInfoBarLayer.cpp
  src/GridLayer.cpp
  src/KeyRunner.cpp
  src/main.cpp
  src/PlayInfoBarLayer.cpp
  src/RootLayer.cpp
  src/TileLayer.cpp
  src/ButtonLayerBuilder.cpp
  src/ConveyorAnimation.cpp
  src/EditRootLayer.cpp
  src/InfoBarLayer.cpp
  src/Layer.cpp
  src/LevelManager.cpp
  src/Options.cpp
  src/PlayRootLayer.cpp
  src/SpriteSheet.cpp
  src/TileSelectorLayer.cpp
)

add_executable(keyrunner ${SOURCES})
add_definitions(-D_ROOT_="${CMAKE_INSTALL_PREFIX}/share/keyrunner")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	add_definitions(-DPATHSEP="/")
elseif(CMAKE_SYSTEM_NAME strequal "Windows")
	add_definitions(-DPATHSEP="\\")
endif()

find_package(SDL 1.2.0 REQUIRED)
if (SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL_LIBRARY})
endif()

find_package(SDL_image REQUIRED)
if (SDL_IMAGE_FOUND)
	include_directories(${SDL_IMAGE_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL_IMAGE_LIBRARIES})
endif()

find_package(SDL_ttf REQUIRED)
if (SDL_TTF_FOUND)
	include_directories(${SDL_TTF_INCLUDE_DIRS})
	target_link_libraries (keyrunner ${SDL_TTF_LIBRARIES})
endif()


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

install (TARGETS keyrunner DESTINATION bin)
install (DIRECTORY img level font DESTINATION share/keyrunner)
install()
