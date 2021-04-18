#include "Level3.h"

#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8

#define LEVEL3_ENEMY_COUNT 2

unsigned int level3_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0, 0,
 3, 0, 0, 0, 2, 0, 0, 3, 0, 3, 3, 3, 1, 1,
 3, 2, 0, 2, 2, 0, 0, 3, 3, 3, 3, 3, 3, 3,
 3, 2, 2, 2, 2, 0, 0, 3, 3, 3, 3, 3, 3, 3
};

void Level3::Initialize() {

    state.fontTextureID = Util::LoadTexture("font1.png");
    state.nextScene = -1;
    state.gameStart = false;

    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);

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

    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];

    GLuint enemy3TextureID = Util::LoadTexture("zombie3.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemy3TextureID;
    state.enemies[0].position = glm::vec3(8.0, 0.0, 0);
    state.enemies[0].gravity = glm::vec3(0, -9.81f, 0);
    state.enemies[0].aiType = JUMPER;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].jumpPower = 5.0f;
    state.enemies[0].width = 0.6;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemy3TextureID;
    state.enemies[1].position = glm::vec3(10.0, 0.0, 0);
    state.enemies[1].gravity = glm::vec3(0, -9.81f, 0);
    state.enemies[1].aiType = JUMPER;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].jumpPower = 5.0f;
    state.enemies[1].width = 0.6;
}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
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
        state.player->success = true;
    }
}
void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
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

    if (state.player->success) {
        Util::DrawText(program, state.fontTextureID, "You Win", 1, -0.5f, glm::vec3(-1.0 + state.player->position.x, -2.5, 0));
    }
}