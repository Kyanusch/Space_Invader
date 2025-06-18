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
    //HighscoreManager::addScore({ "first", GetRandomValue(100, 10000)});
	Gameround* currentGame = new Gameround(0, 100.0f, "");; // Pointer to the current game instance
    UI ui(currentGame);
    UI::gamestate gamestatus = UI::mainmenu; 
	bool programmRunning = true;

    //gameloop
    while (!WindowShouldClose() && programmRunning) {
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
            currentGame->Update();
            currentGame->Draw();
            ui.drawGameUI();
            if (currentGame->getPlayer()->getEnableDelete()) {
				if (ui.drawBlackoutScreen(currentGame->playerDeathTime)) {
					gamestatus = UI::gameover;
				 }
            }
            break;
        case UI::pause:
            if (IsKeyPressed(KEY_TAB)) gamestatus = UI::run;
            currentGame->Draw();
            ui.drawGameUI();
            ui.drawPausMenu(gamestatus);
            break;
		case UI::gameover:
            currentGame->Draw();
            ui.drawGameOverScreen(gamestatus);
            break;
        case UI::newGame: {
            Gameround* oldGame = currentGame;
			currentGame = new Gameround(0, 100.0f, ui.inputPlayernameScreen(oldGame->getPlayer()->name)); // Create a new game instance with the player's name
            ui.changeGame(currentGame);
            delete oldGame; // Clean up the old game instance
            gamestatus = UI::run;
            break;
        }
        case UI::quitgame:
			programmRunning = false; // Exit the game
			break;
        }
        EndDrawing();
    }
    
    Soundmanager::deinitialize();
    HighscoreManager::deinitialize();
    CloseWindow();
    return 0;
}