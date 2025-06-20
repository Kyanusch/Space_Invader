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
	int score2LevelUp = 1000; // Score needed to level up, changes with each level
	int Level = 1; // Current level of the game

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
            if (currentGame->getPlayer()->getScore() >= score2LevelUp) { // If the player has enough score to level up
				currentGame->levelUpTime = GetTime(); // Set the level up time
                gamestatus = UI::levelup; 
            }
			if (currentGame->getPlayer()->getEnableDelete()) { // If the player is dead, draw the blackout screen
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
		case UI::levelup:
            currentGame->Draw();
            ui.drawGameUI();
            if (ui.drawLevelUpScreen(currentGame->levelUpTime)) {
				Level++; // Increase the level
                currentGame->getPlayer()->setShield(100.0f); // Reset player's shield to 100 after level up
				currentGame->difficultie += 10.0f; // Increase difficulty for the next level
                for (int i = 0; i < Level; i++) {
                    currentGame->spawnEnemy(); // Spawn enemys when levelup
                }
                score2LevelUp = score2LevelUp + 1000*Level; // Increase the score needed to level up for the next level
                gamestatus = UI::run; // Continue the game after level up
            }
			break;
		case UI::gameover:
            currentGame->Draw();
            ui.drawGameOverScreen(gamestatus);
            break;
        case UI::newGame: {
			score2LevelUp = 1000; // reset score needed to level up
			Level = 1; // reset level to 1
            Gameround* oldGame = currentGame;
			currentGame = new Gameround(0, 10.0f, ui.inputPlayernameScreen(oldGame->getPlayer()->name)); // Create a new game instance with the player's name
			currentGame->spawnEnemy(); // Spawn enemy at the start 
            
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