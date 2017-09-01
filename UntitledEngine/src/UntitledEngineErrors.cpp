#include "include/UntitledEngineErrors.h"

#include <cstdlib>
#include <iostream>

namespace UntitledEngine {

    //Prints out an error message and exits the game
    void fatalError(std::string errorString) {
        std::cout << errorString << std::endl;
        std::cout << "Enter any key to quit...";
        int tmp;
        std::cin >> tmp;
        glfwTerminate();
        exit(1);
    }

}