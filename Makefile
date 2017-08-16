# check homebrew? (sh -c "$(curl -fsSL https://raw.githubusercontent.com/Tolsadus/42homebrewfix/master/install.sh)")

# brew install SDL

# compile 
#for SDL2
#-I../.brew/Cellar/sdl2/2.0.5/include\
#-L../.brew/Cellar/sdl2/2.0.5/lib -lSDL2
#for openGl
#

PROG		= bomberman

#-- Core Program
SRCS		=	main.cpp MainGame.cpp Sprite.cpp
OBJS		= $(SRCS:%.cpp=%.o)

#-- OpenGL Module
#-- SDL Module

#-- Compiler, libs, flags
INCLUDES	= -I /include
WFLAGS		= -Wall -Wextra -Werror -g3 -std=c++11
CXX			= clang++
SDLFLAGS	= -I../../.brew/Cellar/sdl2/2.0.5/include/ -L../../.brew/Cellar/sdl2/2.0.5/lib 
OPENGLPATHS	= -I../../.brew/Cellar/glew/2.1.0/include/ -L../../.brew/Cellar/glew/2.1.0/lib
OPENGLFLAGS	= $(OPENGLPATHS) -lSDL2 -lGLEW -framework OpenGL
CXXFLAGS		= $(SDLFLAGS) $(OPENGLFLAGS)

.PHONY: clean fclean re run

#-- RULES --#
all: $(PROG)

$(PROG):
	@$(CXX) $(WFLAGS) $(SRCS) $(CXXFLAGS) -o $(PROG)
	@echo "\033[01;36m$(PROG) \t\t-- \033[00;32mCOMPILED\033[0m\n"

#-- General rules
clean:
	@rm -f $(OBJS)
	@echo "\033[01;36m$(PROG) \t\t-- \033[00;32mREMOVED OBJECT FILES\033[0m"

fclean: clean
	@rm -f $(PROG)
	@echo "\033[01;36m$(PROG) \t\t-- \033[00;32mREMOVED EXECUTABLE\033[0m\n"


re: fclean all

run:
	@rm -f $(PROG)
	@echo "\033[01;36m$(PROG) \t\t-- \033[00;32mREMOVED EXECUTABLE\033[0m\n"
	@$(MAKE) -s all
	./$(PROG)

#run_valgrind:
#	valgrind --track-origins=yes --leak-check=full --show-reachable=yes ./nibbler 20 20