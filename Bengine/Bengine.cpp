#include <SDL.h>
#include <GL/glew.h>

#include "Bengine.h"

namespace Bengine {

    int init() {
        //Initialize SDL
        SDL_Init(SDL_INIT_EVERYTHING);

        //Tell SDL that we want a double buffered window so we dont get
        //any flickering
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        return 0;
    }

}