#include "StartScreen.h"

void StartScreen::Initialize() {

    state.fontTextureID = Util::LoadTexture("font1.png");
    state.nextScene = -1;
    state.gameStart = false;

    state.player = new Entity();
    state.map = new Map(0, 0, NULL, NULL, 0, 0, 0);
}
void StartScreen::Update(float deltaTime) {

    if (state.gameStart == true){
        state.nextScene = 1;
    }
}
void StartScreen::Render(ShaderProgram* program) {

    Util::DrawText(program, state.fontTextureID, "ZOMBIE WORLD", 1, -0.5f, glm::vec3(2.5, -2.75, 0));
    Util::DrawText(program, state.fontTextureID, "Press Enter to Start", 1, -0.6f, glm::vec3(1.5, -3.75, 0));

}