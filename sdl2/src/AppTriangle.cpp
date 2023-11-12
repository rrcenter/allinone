
/**
 * SDL2 with OpenGLES 3.0 using glad, base on Google Angle project 
 * 
 * vcpkg install angle:x86-windows sdl2:x86-windows 
 * vcpkg install sdl2-image:x86-windows sdl2-mixer:x86-windows sdl2-ttf:x86-windows sdl2-net:x86-windows
 * 
 * https://www.libsdl.org/
 * https://glad.dav1d.de/
 * https://github.com/google/angle
 */

#include <exception>
#include <functional>
#include <string>

#include "glad/glad.h"


#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>


#define FPS 60
#define FRAME_TIME (1000.0 / FPS)

typedef struct GameCtx
{
    bool needToQuit;
    float radians;

    float deltaTime;

    /*sdl*/
    SDL_Renderer* renderer;
    uint32_t lastTicks;
} GameCtx;


GameCtx game;

// Shader sources
const GLchar* vertexSource =
    "attribute vec4 position;    \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = vec4(position.xyz, 1.0);  \n"
    "}                            \n";
const GLchar* fragmentSource =
    "precision mediump float;\n"
    "void main()                                  \n"
    "{                                            \n"
    "  gl_FragColor = vec4 (1.0, 1.0, 1.0, 1.0 );\n"
    "}                                            \n";


std::function<void()> loop;
void main_loop()
{ 
    if (loop)
    {
        loop();
    }
}

void logicUpdate()
{
    // lock farme
    int timeToWait = FRAME_TIME - (SDL_GetTicks() - game.lastTicks);
    if (timeToWait > 0 && timeToWait <= FRAME_TIME)
    {
        SDL_Delay(timeToWait);
    }
    game.deltaTime = (SDL_GetTicks() - game.lastTicks) / 1000.0f;
    game.lastTicks = SDL_GetTicks();
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    auto wnd(
        SDL_CreateWindow("Mini", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    auto glc = SDL_GL_CreateContext(wnd);


    auto rdr = SDL_CreateRenderer(
        wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    
    // init GLAD
    if (!gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        SDL_DestroyWindow(wnd);
        SDL_Quit();
        return -1;
    }

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f};

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    const GLubyte* extensions = glGetString(GL_EXTENSIONS);
    printf("extensions: %s\n", extensions);

    printf("GL_VENDOR=%s\n", glGetString(GL_VENDOR));

    printf("GL_VERSION=%s\n", glGetString(GL_VERSION));


    loop = [&]
    {
        //1.
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) {
                loop = nullptr;
                //std::terminate();
                exit(0);
            }
        }

        // 2.
        logicUpdate();

        // 3.

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(wnd);
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while(true) main_loop();
#endif

    return 0;
}
