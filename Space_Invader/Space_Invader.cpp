#include "raylib.h"
#include <iostream>
#include "Gameround.h"
#include "UI.h"
#include "HighscoreManager.h"
#include "Bullet.h"
#include "Player.h"
#include "Asteroid.h"
//#include "spaceship.h"
#include <vector>
#include <algorithm>

int main() {
    const int screenWidth = 2160;
    const int screenHeight = 1080;
	std::cout << "Space Invaders 3D" << std::endl;
    InitWindow(screenWidth, screenHeight, "Space Invaders 3D");
    ToggleBorderlessWindowed();
    Soundmanager::initialize();
    HighscoreManager::initialize();
    HighscoreManager::addScore({ "first", GetRandomValue(100, 10000)});
    Gameround game(0, 100.0f);
    UI ui(game.getThis());
    UI::gamestate gamestatus = UI::mainmenu;
    
    //gameloop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        switch (gamestatus) {
        case UI::mainmenu:
            ui.drawMainMenu(gamestatus);
            break;
        case UI::scoreboard:
            ui.drawScoreboard(gamestatus);
            break;
        case UI::run:
            if (IsKeyPressed(KEY_TAB)) gamestatus = UI::pause;
            game.Update();
            game.Draw();
            if (game.getPlayer()->getEnableDelete()) DrawFPS(5, 50);
            ui.drawGameUI();
            break;
        case UI::pause:
            if (IsKeyPressed(KEY_TAB)) gamestatus = UI::run;
            game.Draw();
            ui.drawGameUI();
            ui.drawPausMenu(gamestatus);
            break;
        }
        EndDrawing();
    }
    
    Soundmanager::deinitialize();
    HighscoreManager::deinitialize();
    CloseWindow();
    return 0;
}