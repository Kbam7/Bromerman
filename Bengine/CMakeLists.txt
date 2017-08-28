cmake_minimum_required(VERSION 3.8)
project(Bengine)

set(CMAKE_CXX_STANDARD 14)

set (PLATFORM ${CMAKE_SYSTEM_NAME})

message("Detected Platform ${PLATFORM}")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLM REQUIRED glm)

set(SOURCE_FILES AudioEngine.cpp Bengine.cpp BengineErrors.cpp Camera2D.cpp DebugRenderer.cpp GLSLProgram.cpp
        GUI.cpp ImageLoader.cpp IMainGame.cpp InputManager.cpp IOManager.cpp ParticleBatch2D.cpp ParticleEngine2D.cpp
        picoPNG.cpp ResourceManager.cpp ScreenList.cpp Sprite.cpp SpriteBatch.cpp SpriteFont.cpp TextureCache.cpp
        Timing.cpp Window.cpp)

set(CEGUI_LIBS_DYNAMIC	CEGUIBase-0.dylib
        CEGUICommonDialogs-0.dylib
        CEGUICoreWindowRendererSet.dylib
        CEGUIExpatParser.dylib
        CEGUIOpenGLRenderer-0.dylib
        CEGUISILLYImageCodec.dylib)

set(CEGUI_LIB_PATH	${CMAKE_CURRENT_SOURCE_DIR}/../libs/darwin/cegui)

# - Directories for headers
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/include ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} )

# - Directories for libraries
#link_directories(${GLEW_LIBRARY_DIRS} ${GLM_LIBRARY_DIRS} ${SDL2_LIBRARY_DIRS} ${CEGUI_LIB_PATH} )

# - Add library
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries (${PROJECT_NAME} ${OPENGL_LIBRARIES} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLM_LIBRARIES} ${SDL2_LIBRARIES} ${CEGUI_LIBS_DYNAMIC} )

# - Expose library
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})