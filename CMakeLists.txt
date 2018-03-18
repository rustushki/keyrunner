# Minimum CMake Version.
cmake_minimum_required (VERSION 3.0.1)

# Project Name.
project (keyrunner)

# Source the CMake scripts capable of finding SDL2.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${keyrunner_SOURCE_DIR}/cmake")

add_library(uitk STATIC
    src/uitk/Animation.cpp
    src/uitk/ButtonView.cpp
    src/uitk/ImageView.cpp
    src/uitk/LabelView.cpp
    src/uitk/MenuView.cpp
    src/uitk/RectangleView.cpp
    src/uitk/SpriteSheet.cpp
)

# Source Files.
set(SOURCES
	src/model/BaseBoardEntity.cpp
	src/model/Coordinate.cpp
	src/model/HitBoxType.cpp
	src/model/RectangleHitBox.cpp
	src/model/TileCoordinate.cpp
    src/controller/BoardController.cpp
    src/controller/Display.cpp
    src/controller/DisplayController.cpp
    src/controller/EditorController.cpp
    src/controller/GameController.cpp
    src/controller/GameOverController.cpp
    src/controller/GameWinController.cpp
    src/controller/OptionController.cpp
    src/controller/PlayController.cpp
    src/controller/TitleScreenController.cpp
    src/controller/main.cpp
    src/model/AnimationTypeFactory.cpp
    src/model/BaseModel.cpp
    src/model/BoardEntityType.hpp
    src/model/BoardModel.cpp
    src/model/EditorBoardModel.cpp
    src/model/GameModel.cpp
    src/model/GameOverModel.cpp
    src/model/GameWinModel.cpp
    src/model/JsonLevelManager.cpp
    src/model/OptionModel.cpp
    src/model/PlayBoardModel.cpp
    src/model/TitleScreenModel.cpp
    src/uitk/BaseView.cpp
    src/view/AnimationFactory.cpp
    src/view/BoardView.cpp
    src/view/EditorBoardView.cpp
    src/view/LevelNumberView.cpp
    src/view/TimerView.cpp
)

# Declare Executable.
add_executable(keyrunner ${SOURCES})

# Let the preprocessor know that we're building a Snapcraft package.
if(BUILDER_SNAP)
	add_definitions(-DBUILDER_SNAP=1)
else()
	add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")
endif()

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

# Include nlohmann's json library
include_directories("json")

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
