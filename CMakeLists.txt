cmake_minimum_required(VERSION 3.8)
project(ZombieGame)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -std=c++14")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW3 REQUIRED glfw3)
pkg_search_module(GLM REQUIRED glm)


IF (OpenGL_FOUND)
	message(STATUS "OpenGL Found")
	message("\n\r")
ENDIF ()
IF (GLEW_FOUND)
	message(STATUS "GLEW Found")
	message("\n\r")
ENDIF ()
IF (GLFW3_FOUND)
	message(STATUS "GLFW3 Found")
	message("\n\r")
ENDIF ()
IF (GLM_FOUND)
	message(STATUS "GLM Found")
	message("\n\r")
ENDIF ()

add_subdirectory(lib/nanogui)
include_directories(lib/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})
add_definitions(${NANOGUI_EXTRA_DEFS})

add_subdirectory(UntitledEngine)

# - Directories for headers
include_directories(BEFORE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${OPENGL_INCLUDE_DIR}/Headers
		${GLEW_INCLUDE_DIRS}
		${GLFW3_INCLUDE_DIRS}
		${GLM_INCLUDE_DIRS}
		)

# - Directories for libraries
link_directories(
		${GLEW_LIBRARY_DIRS}
		${GLFW3_LIBRARY_DIRS}
		${GLM_LIBRARY_DIRS}
)

# - Executeable
set(SOURCE_FILES src/main.cpp src/Agent.cpp src/Zombie.cpp src/Human.cpp src/Gun.cpp src/Bullet.cpp src/Level.cpp src/Player.cpp src/MainGame.cpp src/MainMenu.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# - Link to library
target_link_libraries(${PROJECT_NAME}
		UntitledEngine
		${OPENGL_LIBRARIES}
		${OPENGL_gl_LIBRARY}
		${GLEW_LIBRARIES}
		${GLFW3_LIBRARIES}
		${GLM_LIBRARIES}
		nanogui
		${NANOGUI_EXTRA_LIBS}
		)

message(STATUS ${PROJECT_BINARY_DIR})