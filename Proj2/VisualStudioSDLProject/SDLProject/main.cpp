#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

bool gameOver = false;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix, player1Matrix, player2Matrix, ballMatrix;

glm::vec3 player1_position = glm::vec3(-5, 0, 0);
glm::vec3 player1_movement = glm::vec3(0, 0, 0);

float player1_speed = 2.0f;

glm::vec3 player2_position = glm::vec3(5, 0, 0);
glm::vec3 player2_movement = glm::vec3(0, 0, 0);

float player2_speed = 2.0f;

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(1.0, 1.0, 0);

float ball_speed = 2.0f;

//GLuint playerTextureID;
//GLuint sunTextureID;
//
//GLuint LoadTexture(const char* filePath) {
//    int w, h, n;
//    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
//
//    if (image == NULL) {
//        std::cout << "Unable to load image. Make sure the path is correct\n";
//        assert(false);
//    }
//
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//    stbi_image_free(image);
//    return textureID;
//}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    player1Matrix = glm::mat4(1.0f);
    player2Matrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    //glEnable(GL_BLEND);
    // Good setting for transparency
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //playerTextureID = LoadTexture("knight.png");
    //sunTextureID = LoadTexture("sun.png");
}

void ProcessInput() {

    player1_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);
    //ball_movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                // Some sort of action
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    if (gameOver == false) {

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_S]) {
            if (player1_position.y > -2.75) {
                player1_movement.y = -1.0f;
            }
        }
        else if (keys[SDL_SCANCODE_W]) {
            if (player1_position.y < 2.75) {
                player1_movement.y = 1.0f;
            }
        }

        if (keys[SDL_SCANCODE_DOWN]) {
            if (player2_position.y > -2.75) {
                player2_movement.y = -1.0f;
            }
        }
        else if (keys[SDL_SCANCODE_UP]) {
            if (player2_position.y < 2.75) {
                player2_movement.y = 1.0f;
            }
        }

        if (glm::length(player1_movement) > 1.0f) {
            player1_movement = glm::normalize(player1_movement);
        }

        if (glm::length(player2_movement) > 1.0f) {
            player2_movement = glm::normalize(player2_movement);
        }

        if (ball_position.y > 3.65 || ball_position.y < -3.65) {
            ball_movement.y = -1 * ball_movement.y;
        }

        if (ball_position.x > 4.9 || ball_position.x < -4.9) {
            ball_movement = glm::vec3(0);
            gameOver = true;
        }

        if ((ball_position.x < -4.7) && ((player1_position.y + 1) > ball_position.y) && (ball_position.y > (player1_position.y - 1))) {
            ball_movement.x = -1 * ball_movement.x;
        }

        if ((ball_position.x > 4.7) && ((player2_position.y + 1) > ball_position.y) && (ball_position.y > (player2_position.y - 1))) {
            ball_movement.x = -1 * ball_movement.x;
        }
    }
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    player1_position += player1_movement * player1_speed * deltaTime;
    player2_position += player2_movement * player2_speed * deltaTime;
    ball_position += ball_movement * ball_speed * deltaTime;

    player1Matrix = glm::mat4(1.0f);
    player1Matrix = glm::translate(player1Matrix, player1_position);

    player2Matrix = glm::mat4(1.0f);
    player2Matrix = glm::translate(player2Matrix, player2_position);

    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ball_position);
}

void Render() {
    float vertices1[] = { 0.0, -1.0, 0.2, -1.0, 0.2, 1.0, 0.0, -1.0, 0.2, 1.0, 0.0, 1.0 };
    float vertices2[] = { -0.2, -1.0, 0.0, -1.0, 0.0, 1.0, -0.2, -1.0, 0.0, 1.0, -0.2, 1.0 };
    float vertices3[] = { -0.1, -0.1, 0.1, -0.1, 0.1, 0.1, -0.1, -0.1, 0.1, 0.1, -0.1, 0.1 };

    glClear(GL_COLOR_BUFFER_BIT);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
    glEnableVertexAttribArray(program.positionAttribute);
    program.SetModelMatrix(player1Matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(program.positionAttribute);
    program.SetModelMatrix(player2Matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
    glEnableVertexAttribArray(program.positionAttribute);
    program.SetModelMatrix(ballMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}