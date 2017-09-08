cmake_minimum_required( VERSION 3.8 )
project( ZombieGame )

set( CMAKE_BUILD_TYPE Release )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -std=c++14" )
set( PLATFORM ${CMAKE_SYSTEM_NAME} )

find_package( OpenGL REQUIRED )
find_package( PkgConfig REQUIRED )

pkg_search_module( GLEW REQUIRED glew )
pkg_search_module( GLFW3 REQUIRED glfw3 )
pkg_search_module( GLM REQUIRED glm )
pkg_search_module( ASSIMP REQUIRED assimp )

message( "\n" )
message( "Detected Platform: ${PLATFORM}" )
IF (OpenGL_FOUND)
    message( STATUS "OpenGL Found" )
#    message(STATUS ${OPENGL_gl_LIBRARIES})
#    message(STATUS ${OPENGL_gl_LIBRARY_DIRS})
#    message(STATUS ${OPENGL_gl_LDFLAGS})
#    message(STATUS ${OPENGL_INCLUDE_DIR})
ENDIF ()
IF (GLEW_FOUND)
    message( STATUS "GLEW Found" )
#    message(STATUS "INC_DIRS: ${GLEW_INCLUDE_DIRS}")
#    message(STATUS "LIBS: ${GLEW_LIBRARIES}")
#    message(STATUS "LIB_DIRS: ${GLEW_LIBRARY_DIRS}")
#    message(STATUS "LD_FLAGS: ${GLEW_LDFLAGS}")
ENDIF ()
IF (GLFW3_FOUND)
    message( STATUS "GLFW3 Found" )
#    message(STATUS "INC_DIRS: ${GLFW3_INCLUDE_DIRS}")
#    message(STATUS "LIBS: ${GLFW3_LIBRARIES}")
#    message(STATUS "LIB_DIRS: ${GLFW3_LIBRARY_DIRS}")
#    message(STATUS "LD_FLAGS: ${GLFW3_LDFLAGS}")
ENDIF ()
IF (GLM_FOUND)
    message( STATUS "GLM Found" )
#    message(STATUS "INC_DIRS: ${GLM_INCLUDE_DIRS}")
#    message(STATUS "LIBS: ${GLM_LIBRARIES}")
#    message(STATUS "LIB_DIRS: ${GLM_LIBRARY_DIRS}")
#    message(STATUS "LD_FLAGS: ${GLM_LDFLAGS}")
ENDIF ()
IF (ASSIMP_FOUND)
    message( STATUS "ASSIMP Found" )
#    message(STATUS "INC_DIRS: ${ASSIMP_INCLUDE_DIRS}")
#    message(STATUS "LIBS: ${ASSIMP_LIBRARIES}")
#    message(STATUS "LIB_DIRS: ${ASSIMP_LIBRARY_DIRS}")
#    message(STATUS "LD_FLAGS: ${ASSIMP_LDFLAGS}")
ENDIF ()
message( "\n" )

add_subdirectory( UntitledEngine )

# - Directories for headers
include_directories( BEFORE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENGL_INCLUDE_DIR}/Headers
        ${GLEW_INCLUDE_DIRS}
        ${GLFW3_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIRS}
        )

# - Directories for libraries
link_directories(
        ${GLEW_LIBRARY_DIRS}
        ${GLFW3_LIBRARY_DIRS}
        ${GLM_LIBRARY_DIRS}
        ${ASSIMP_LIBRARY_DIRS}
)

# - Executeable
#set(HEADER_FILES Level.h Agent.h Bullet.h Gun.h Human.h MainGame.h Player.h Zombie.h)
set( SOURCE_FILES main.cpp Agent.cpp Zombie.cpp Human.cpp Gun.cpp Bullet.cpp Level.cpp Player.cpp MainGame.cpp )
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

# - Link to library
target_link_libraries( ${PROJECT_NAME}
        UntitledEngine
        ${OPENGL_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${GLEW_LIBRARIES}
        ${GLFW3_LIBRARIES}
        ${GLM_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        )

message( STATUS ${PROJECT_BINARY_DIR} )

# - Custom Commands
#ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ../libs ${PROJECT_BINARY_DIR}/libs
#        )

#cmake_minimum_required( VERSION 3.8 )
#project( ZombieGame )
#
#set( CMAKE_CXX_STANDARD 14 )
#set( CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -std=c++14" )
#
#find_package(OpenGL REQUIRED)
#find_package(PkgConfig REQUIRED)
#
#pkg_search_module(GLEW REQUIRED glew)
#pkg_search_module(GLFW3 REQUIRED glfw3)
#pkg_search_module(GLM REQUIRED glm)
#
##find_package(SDL2)
#IF (OpenGL_FOUND)
#    message(STATUS "OpenGL Found")
##    message(STATUS ${OpenGL_LIBRARIES})
##    message(STATUS ${OpenGL_LIBRARY_DIRS})
##    message(STATUS ${OpenGL_LDFLAGS})
##    message(STATUS ${OPENGL_INCLUDE_DIR})
#    message("\n")
#ENDIF()
#IF (GLEW_FOUND)
#    message(STATUS "GLEW Found")
##    message(STATUS ${GLEW_LDFLAGS})
#    message("\n")
#ENDIF()
#IF (GLFW3_FOUND)
#    message(STATUS "GLFW3 Found")
##    message(STATUS ${GLFW3_LIBRARIES})
##    message(STATUS ${GLFW3_LIBRARY_DIRS})
##    message(STATUS ${GLFW3_LDFLAGS})
##    message(STATUS ${GLFW3_INCLUDE_DIRS})
#    message("\n")
#ENDIF()
#IF (GLM_FOUND)
#    message(STATUS "GLM Found")
#    message("\n")
#ENDIF()
#
#add_subdirectory(UntitledEngine)
#
## - Directories for headers
#include_directories(BEFORE
#        ${CMAKE_CURRENT_SOURCE_DIR}/include
#        ${OPENGL_INCLUDE_DIR}/Headers
#        ${GLEW_INCLUDE_DIRS}
#        ${GLFW3_INCLUDE_DIRS}
#        ${GLM_INCLUDE_DIRS}
#        )
#
## - Directories for libraries
#link_directories(
#        ${GLEW_LIBRARY_DIRS}
#        ${GLFW3_LIBRARY_DIRS}
#        ${GLM_LIBRARY_DIRS}
#        )
#
## - Executeable
#set( SOURCE_FILES main.cpp )
#add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )
#
## - Link to library
#target_link_libraries (${PROJECT_NAME}
#        UntitledEngine
#        ${OPENGL_LIBRARIES}
#        ${OPENGL_gl_LIBRARY}
#        ${GLEW_LIBRARIES}
#        ${GLFW3_LIBRARIES}
#        ${GLM_LIBRARIES}
#        ) # ${OPENGL_LIBRARIES} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLM_LIBRARIES}  ${SDL2_LIBRARIES}
#
#message(STATUS ${PROJECT_BINARY_DIR})
#
## - Custom Commands
## Copies CEGUI files into build directory
##ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
##        COMMAND ${CMAKE_COMMAND} -E copy_directory ../libs ${PROJECT_BINARY_DIR}/libs
##        )