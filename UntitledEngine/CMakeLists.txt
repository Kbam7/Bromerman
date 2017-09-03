cmake_minimum_required(VERSION 3.8)
project(UntitledEngine)

set(CMAKE_CXX_STANDARD 14)

set (PLATFORM ${CMAKE_SYSTEM_NAME})

message("Detected Platform ${PLATFORM}")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW3 REQUIRED glfw3)
pkg_search_module(GLM REQUIRED glm)

set(SOURCE_FILES    src/UntitledEngineErrors.cpp    src/GLSLProgram.cpp     src/IOManager.cpp
					src/UntitledEngine.cpp          src/Window.cpp          #src/AudioEngine.cpp
					src/Camera2D.cpp                src/DebugRenderer.cpp   src/ImageLoader.cpp
					src/IMainGame.cpp               src/InputManager.cpp    src/ParticleBatch2D.cpp
					src/ParticleEngine2D.cpp        src/picoPNG.cpp         src/ResourceManager.cpp
					src/ScreenList.cpp              src/Sprite.cpp          src/SpriteBatch.cpp
					src/TextureCache.cpp    		src/Timing.cpp			#src/SpriteFont.cpp
		)

# - Directories for headers
include_directories(BEFORE include/ ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS} )

# - Add library
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries (${PROJECT_NAME} ${OPENGL_LIBRARIES} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} ${GLM_LIBRARIES} ${GLFW3_LIBRARIES} )

# - Expose library
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})