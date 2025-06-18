#pragma once
#include "raylib.h"
#include "virtualCamera.h"
#include "HighscoreManager.h"
#include "Gameround.h"
class UI {
private:
	Gameround* game;
	void drawScore(Player* player);
	void drawBars(Player* player);
	enum selectedButton {
		noSelection,
		quit,
		start,
		resume,
		highscores
	};
	selectedButton pauseMenuSelection;
	selectedButton mainMenuSelection;
public:
	enum gamestate {
		run,
		quitgame,
		pause,
		mainmenu,
		gameover,
		scoreboard,
		newGame
	};
	UI(Gameround* game);
	virtual ~UI();
	bool changeGame(Gameround* newgame);
	void drawGameUI();
	void drawPausMenu(gamestate& gamestatus);
	void drawMainMenu(gamestate& gamestatus);
	void drawScoreboard(gamestate& gamestatus);
	bool drawBlackoutScreen(double playerDeathTime);
	void drawGameOverScreen(gamestate& gamestatus);
	std::string inputPlayernameScreen(std::string oldPlayername = "");
};
