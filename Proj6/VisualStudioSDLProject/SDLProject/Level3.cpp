#include "Level3.h"

#define LEVEL3_WIDTH 8
#define LEVEL3_HEIGHT 40

#define LEVEL3_ENEMY_COUNT 10
#define LEVEL3_BULLET_COUNT 10

unsigned int level3_data[] =
{
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
1, 2, 3, 4, 5, 6, 7, 8,
9, 10, 11, 12, 13, 14, 15, 16,
17, 18, 19, 20, 21, 22, 23, 24,
25, 26, 27, 28, 29, 30, 31, 32,
};

void Level3::Initialize() {

    state.fontTextureID = Util::LoadTexture("blockFont.png");
    state.nextScene = -1;
    state.gameStart = false;

    GLuint mapTextureID = Util::LoadTexture("spaceStars.jpg");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 7, 4);

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(3.75f, -37.0f, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("spaceship.png");

    state.player->height = 0.75;
    state.player->width = 0.4;

    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemy3TextureID = Util::LoadTexture("8-bit-enemies.png");

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemy3TextureID;
        state.enemies[i].speed = 1;
        state.enemies[i].aiType = WAITANDGO;
        state.enemies[i].aiState = IDLE;
        state.enemies[i].width = 0.6;
        state.enemies[i].height = 0.6;
        state.enemies[i].animCols = 10;
        state.enemies[i].animRows = 10;
        state.enemies[i].animIndex = i+2;
    }

    state.enemies[0].position = glm::vec3(6.0, -10.0, 0.0);
    state.enemies[1].position = glm::vec3(4.0, -10.0, 0.0);
    state.enemies[2].position = glm::vec3(2.0, -10.0, 0.0);
    state.enemies[3].position = glm::vec3(4.0, -15.0, 0.0);
    state.enemies[4].position = glm::vec3(2.0, -21.0, 0.0);
    state.enemies[5].position = glm::vec3(6.0, -21.0, 0.0);
    state.enemies[6].position = glm::vec3(7.0, -25.0, 0.0);
    state.enemies[7].position = glm::vec3(5.0, -27.0, 0.0);
    state.enemies[8].position = glm::vec3(6.0, -30.0, 0.0);
    state.enemies[9].position = glm::vec3(2.0, -30.0, 0.0);

    state.bullets = new Entity[LEVEL3_BULLET_COUNT];
    state.bulletCnt = LEVEL3_BULLET_COUNT;
    GLuint bulletTextureID = Util::LoadTexture("blast.png");
    for (int i = 0; i < LEVEL3_BULLET_COUNT; i++) {
        state.bullets[i].entityType = BULLET;
        state.bullets[i].textureID = bulletTextureID;
        state.bullets[i].speed = 3;
        state.bullets[i].position = glm::vec3(0, 0, 0);
        state.bullets[i].movement = glm::vec3(0, 0, 0);
        state.bullets[i].gravity = glm::vec3(0, 0, 0);
        state.bullets[i].height = .2;
        state.bullets[i].width = .2;
        state.bullets[i].isActive = false;
    }

}
void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, NULL, 0, state.map);
    }

    for (int i = 0; i < LEVEL3_BULLET_COUNT; i++) {
        state.bullets[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    }

    if (state.player->dead) {
        int tempLives = state.player->lives - 1;
        if (tempLives <= 0) {
            state.player->fail = true;
        }
        else {
            Initialize();
            state.player->lives = tempLives;
        }
    }

    if (state.player->position.y >= -7) {
        state.player->success = true;
    }
}
void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    for (int i = 0; i < LEVEL3_BULLET_COUNT; i++) {
        state.bullets[i].Render(program);
    }

    Util::DrawText(program, state.fontTextureID, "Lives:", 1, -0.6f, glm::vec3(0.0, -1.5 + state.player->position.y, 0));

    std::string strLives = std::to_string(state.player->lives);
    Util::DrawText(program, state.fontTextureID, strLives, 1, -0.6f, glm::vec3(2.5, -1.5 + state.player->position.y, 0));

    Util::DrawText(program, state.fontTextureID, "Bullets:", 1, -0.6f, glm::vec3(3.5, -1.5 + state.player->position.y, 0));

    std::string strBulletCnt = std::to_string(state.bulletCnt);
    Util::DrawText(program, state.fontTextureID, strBulletCnt, 1, -0.6f, glm::vec3(6.7, -1.5 + state.player->position.y, 0));

    Util::DrawText(program, state.fontTextureID, "Level 3", 1, -0.6f, glm::vec3(0.0, 7.5 + state.player->position.y, 0));


    if (state.player->fail) {
        Util::DrawText(program, state.fontTextureID, "You Lose", 1, -0.5f, glm::vec3(2.0, 2.5 + state.player->position.y, 0));
    }

    if (state.player->success) {
        Util::DrawText(program, state.fontTextureID, "You Win", 1, -0.5f, glm::vec3(2.0, 2.5 + state.player->position.y, 0));
    }
}