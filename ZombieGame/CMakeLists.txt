cmake_minimum_required( VERSION 3.8 )
project( ZombieGame )

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -std=c++14" )

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED sdl2_image)
pkg_search_module(SDL2_TTF REQUIRED sdl2_ttf)
pkg_search_module(SDL2_MIXER REQUIRED sdl2_mixer)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLM REQUIRED glm)

#find_package(SDL2)
IF (OpenGL_FOUND)
    message(STATUS "OpenGL Found")
    message(STATUS ${OpenGL_LIBRARIES})
    message(STATUS ${OpenGL_LIBRARY_DIRS})
    message(STATUS ${OpenGL_LDFLAGS})
    message(STATUS ${OPENGL_INCLUDE_DIR})
    message(STATUS "\n\r")
ENDIF()
IF (SDL2_FOUND)
    message(STATUS "SDL2 Found")
ENDIF()
IF (SDL2_IMAGE_FOUND)
    message(STATUS "SDL2_IMAGE Found")
    message(STATUS ${SDL2_IMAGE_LIBRARIES})
    message(STATUS ${SDL2_IMAGE_LIBRARY_DIRS})
    message(STATUS ${SDL2_IMAGE_LDFLAGS})
    message(STATUS ${SDL2_IMAGE_INCLUDE_DIRS})
    message(STATUS "\n\r")
ENDIF()
IF (SDL2_TTF_FOUND)
    message(STATUS "SDL2_TTF Found")
    message(STATUS ${SDL2_TTF_LIBRARIES})
    message(STATUS ${SDL2_TTF_LIBRARY_DIRS})
    message(STATUS ${SDL2_TTF_LDFLAGS})
    message(STATUS ${SDL2_TTF_INCLUDE_DIRS})
    message(STATUS "\n\r")
ENDIF()
IF (SDL2_MIXER_FOUND)
    message(STATUS "SDL2_MIXER Found")
    message(STATUS ${SDL2_MIXER_LIBRARIES})
    message(STATUS ${SDL2_MIXER_LIBRARY_DIRS})
    message(STATUS ${SDL2_MIXER_LDFLAGS})
    message(STATUS ${SDL2_MIXER_INCLUDE_DIRS})
    message(STATUS "\n\r")
ENDIF()
IF (GLEW_FOUND)
    message(STATUS "GLEW Found")
    message(STATUS ${GLEW_LDFLAGS})
ENDIF()
IF (GLM_FOUND)
    message(STATUS "GLM Found")
ENDIF()

set(CEGUI_LIBS_DYNAMIC	CEGUIBase-0.dylib
        CEGUICommonDialogs-0.dylib
        CEGUICoreWindowRendererSet.dylib
        CEGUIExpatParser.dylib
        CEGUIOpenGLRenderer-0.dylib
        CEGUISILLYImageCodec.dylib)

set(CEGUI_LIB_PATH	${CMAKE_CURRENT_SOURCE_DIR}/libs/darwin/cegui)

add_subdirectory(Bengine)

# - Directories for headers
include_directories(BEFORE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${OPENGL_INCLUDE_DIR}/Headers
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        )

# - Directories for libraries
link_directories(
        ${GLEW_LIBRARY_DIRS}
        ${GLM_LIBRARY_DIRS}
        ${SDL2_LIBRARY_DIRS}
        ${SDL2_IMAGE_LIBRARY_DIRS}
        ${SDL2_TTF_LIBRARY_DIRS}
        ${SDL2_MIXER_LIBRARY_DIRS}
        ${CEGUI_LIB_PATH}
        )

# - Executeable
set( SOURCE_FILES main.cpp Agent.cpp Zombie.cpp Human.cpp Gun.cpp Bullet.cpp Level.cpp Player.cpp MainGame.cpp )
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

# - Link to library
#target_link_libraries (${PROJECT_NAME} LINK_PUBLIC Bengine )
target_link_libraries (${PROJECT_NAME}
        Bengine
        ${OPENGL_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${GLEW_LIBRARIES}
        ${GLM_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${CEGUI_LIBS_DYNAMIC}
        ) # ${OPENGL_LIBRARIES} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLM_LIBRARIES}  ${SDL2_LIBRARIES}

