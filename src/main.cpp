#include "../inc/MainGame.h"
#include "../inc/MainMenu.h"

//#include <cstdio>
//#include <iostream>
//#include <cstdlib>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#include "../UntitledEngine/include/GLSLProgram.h"

//GLuint  LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
//
//void initShaders() {
//    // Compile our color shader
//    textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
//    textureProgram.addAttribute("vertexPosition");
//    textureProgram.addAttribute("vertexColor");
//    textureProgram.addAttribute("vertexUV");
//    textureProgram.linkShaders();
//}

int main(void) {

    MainGame    *mainGame;
    MainMenu    mainMenu;
    GLFWwindow  *window;

    nanogui::init();
    // create the window and pass it to main game
    mainGame = nullptr;
    mainMenu.initMenu(750, 750, mainGame , true, false);
   

    window = mainMenu.getGlfwWindow();

    mainGame->run(window);

    nanogui::shutdown();
    return 0;
} 
//    GLSLProgram textureProgram;
//
//    // Initialise GLFW
//    if( !glfwInit() )
//    {
//        fprintf( stderr, "Failed to initialize GLFW\n" );
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
//
//    // Open a window and create its OpenGL context
//    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
//    window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
//    if( window == NULL ){
//        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window); // Initialize GLEW
//    glewExperimental = true; // Needed in core profile
//    if (glewInit() != GLEW_OK) {
//        fprintf(stderr, "Failed to initialize GLEW\n");
//        return -1;
//    }
//
//    // Ensure we can capture the escape key being pressed below
//    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//
//    // Create VAO
//    GLuint VertexArrayID;
//    glGenVertexArrays(1, &VertexArrayID);
//    glBindVertexArray(VertexArrayID);
//
//    // An array of 3 vectors which represents 3 vertices
//    static const GLfloat g_vertex_buffer_data[] = {
//            -1.0f, -1.0f, 0.0f,
//            1.0f, -1.0f, 0.0f,
//            0.0f,  1.0f, 0.0f,
//    };
//
//    // This will identify our vertex buffer
//    GLuint vertexbuffer;
//    // Generate 1 buffer, put the resulting identifier in vertexbuffer
//    glGenBuffers(1, &vertexbuffer);
//    // The following commands will talk about our 'vertexbuffer' buffer
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    // Give our vertices to OpenGL.
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//
//    // Create and compile our GLSL program from the shaders
//    GLuint programID = LoadShaders( "../Shaders/SimpleVertexShader.vertexshader", "../Shaders/SimpleFragmentShader.fragmentshader" );
//
//    do{
//
//        // Clear
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // Use GLSL program
//        glUseProgram(programID);
//
//        // 1rst attribute buffer : vertices
//        glEnableVertexAttribArray(0);
//        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//        glVertexAttribPointer(
//                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//                3,                  // size
//                GL_FLOAT,           // type
//                GL_FALSE,           // normalized?
//                0,                  // stride
//                (void*)0            // array buffer offset
//        );
//        // Draw the triangle !
//        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//        glDisableVertexAttribArray(0);
//
//        // Swap buffers
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//
//    } // Check if the ESC key was pressed or the window was closed
//    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
//           glfwWindowShouldClose(window) == 0 );



//
//#include <fstream>
//#include <vector>
//
//GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
//
//    // Create the shaders
//    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
//    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
//
//    // Read the Vertex Shader code from the file
//    std::string VertexShaderCode;
//    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
//    if(VertexShaderStream.is_open()){
//        std::string Line = "";
//        while(getline(VertexShaderStream, Line))
//            VertexShaderCode += "\n" + Line;
//        VertexShaderStream.close();
//    }else{
//        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
//        getchar();
//        return 0;
//    }
//
//    // Read the Fragment Shader code from the file
//    std::string FragmentShaderCode;
//    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
//    if(FragmentShaderStream.is_open()){
//        std::string Line = "";
//        while(getline(FragmentShaderStream, Line))
//            FragmentShaderCode += "\n" + Line;
//        FragmentShaderStream.close();
//    }
//
//    GLint Result = GL_FALSE;
//    int InfoLogLength;
//
//
//    // Compile Vertex Shader
//    printf("Compiling shader : %s\n", vertex_file_path);
//    char const * VertexSourcePointer = VertexShaderCode.c_str();
//    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
//    glCompileShader(VertexShaderID);
//
//    // Check Vertex Shader
//    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
//    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//    if ( InfoLogLength > 0 ){
//        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
//        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
//        printf("%s\n", &VertexShaderErrorMessage[0]);
//    }
//
//
//
//    // Compile Fragment Shader
//    printf("Compiling shader : %s\n", fragment_file_path);
//    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
//    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
//    glCompileShader(FragmentShaderID);
//
//    // Check Fragment Shader
//    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
//    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//    if ( InfoLogLength > 0 ){
//        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
//        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
//        printf("%s\n", &FragmentShaderErrorMessage[0]);
//    }
//
//
//
//    // Link the program
//    printf("Linking program\n");
//    GLuint ProgramID = glCreateProgram();
//    glAttachShader(ProgramID, VertexShaderID);
//    glAttachShader(ProgramID, FragmentShaderID);
//    glLinkProgram(ProgramID);
//
//    // Check the program
//    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
//    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
//    if ( InfoLogLength > 0 ){
//        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
//        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
//        printf("%s\n", &ProgramErrorMessage[0]);
//    }
//
//
//    glDetachShader(ProgramID, VertexShaderID);
//    glDetachShader(ProgramID, FragmentShaderID);
//
//    glDeleteShader(VertexShaderID);
//    glDeleteShader(FragmentShaderID);
//
//    return ProgramID;
//}