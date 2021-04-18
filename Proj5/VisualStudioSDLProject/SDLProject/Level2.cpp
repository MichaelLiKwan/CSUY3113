#include "Level2.h"

#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 2

unsigned int level2_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1, 1,
 3, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
 3, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level2::Initialize() {

    state.fontTextureID = Util::LoadTexture("font1.png");
    state.nextScene = -1;
    state.gameStart = false;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1.0f, 0.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->gravity = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.75;
    state.player->width = 0.4;

    state.player->jumpPower = 7.0f;

    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    
    GLuint enemy2TextureID = Util::LoadTexture("zombie2.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemy2TextureID;
    state.enemies[0].position = glm::vec3(10.0, 0.0, 0.0);
    state.enemies[0].gravity = glm::vec3(0, -9.81f, 0);
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = RIGHT;
    state.enemies[0].speed = 1;
    state.enemies[0].width = 0.6;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemy2TextureID;
    state.enemies[1].position = glm::vec3(6.0, 0.0, 0.0);
    state.enemies[1].gravity = glm::vec3(0, -9.81f, 0);
    state.enemies[1].aiType = WALKER;
    state.enemies[1].aiState = RIGHT;
    state.enemies[1].speed = 1;
    state.enemies[1].width = 0.6;
}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.map);
    }

    if (state.player->position.y < -8 || state.player->dead) {
        int tempLives = state.player->lives - 1;
        if (tempLives <= 0) {
            state.player->fail = true;
        }
        else {
            Initialize();
            state.player->lives = tempLives;
        }
    }

    if (state.player->position.x >= 12) {
        state.nextScene = 3;
    }
}
void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    if (state.player->position.x > 5) {
        Util::DrawText(program, state.fontTextureID, "Lives:", 1, -0.5f, glm::vec3(-4.75 + state.player->position.x, -.5, 0));

        std::string strLives = std::to_string(state.player->lives);
        Util::DrawText(program, state.fontTextureID, strLives, 1, -0.5f, glm::vec3(-1.75 + state.player->position.x, -.5, 0));
    }
    else {
        Util::DrawText(program, state.fontTextureID, "Lives:", 1, -0.5f, glm::vec3(0.25, -.5, 0));

        std::string strLives = std::to_string(state.player->lives);
        Util::DrawText(program, state.fontTextureID, strLives, 1, -0.5f, glm::vec3(3.25, -.5, 0));
    }

    if (state.player->fail) {
        Util::DrawText(program, state.fontTextureID, "You Lose", 1, -0.5f, glm::vec3(-1.0 + state.player->position.x, -2.5, 0));
    }
}