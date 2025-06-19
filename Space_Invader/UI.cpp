#include "UI.h"

UI::UI(Gameround* game) : game(game), pauseMenuSelection(resume), mainMenuSelection(start) {
}

UI::~UI() {
}

bool UI::changeGame(Gameround* newgame) {
	if (newgame != nullptr) {
		game = newgame;
		return true;
	}
	return false;   //return false if newgame could not be set
}

void UI::drawGameUI() {
	auto player = game->getPlayer();
	drawBars(player);
	drawScore(player);
}
void UI::drawScore(Player* player) {
	int fontsize = 25;
	DrawText(TextFormat("score: %d", player->getScore()), 5, 5, fontsize, WHITE);
	const char* playerName = player->name.c_str();
	DrawText(playerName, GetScreenWidth() - MeasureText(playerName, fontsize) - 5, 5, fontsize, WHITE);
}

void UI::drawBars(Player* player) {
	// UI position
	int barWidth = 500;
	int barHeight = 30;
	int x = GetScreenWidth() / 2;
	int y = GetScreenHeight() - 2 * barHeight;

	//background
	DrawRectangle(x - barWidth, y, 2 * barWidth, barHeight, DARKGRAY);

	//healthbar
	DrawRectangle(x - barWidth * (player->getHealth() / 100.0f), y, barWidth * (player->getHealth() / 100.0f), barHeight, RED);

	//shieldbar
	DrawRectangle(x, y, barWidth * (player->getShield() / 100.0f), barHeight, BLUE);
}

void UI::drawPausMenu(gamestate& gamestatus) {
	//controles
	switch (pauseMenuSelection) {
	case resume:
		if (IsKeyPressed(KEY_DOWN)) pauseMenuSelection = quit;
		break;
	case quit:
		if (IsKeyPressed(KEY_UP)) pauseMenuSelection = resume;
		break;
	default:
		pauseMenuSelection = resume;
	}

	const int sizeX = 500;
	const int sizeY = 220;
	const int ButtonY = 100;
	int fontsize = 50;
	const char* Resume = "RESUME";
	const char* Quit = "QUIT";
	//background
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0,0,0,100 });
	DrawRectangle(GetScreenWidth() / 2 - sizeX / 2, GetScreenHeight() / 2 - sizeY / 2, sizeX, sizeY, DARKGRAY);

	//resume button
	DrawRectangle(GetScreenWidth() / 2 - sizeX / 2 + 5, GetScreenHeight() / 2 - sizeY / 2 + 5, sizeX - 10, ButtonY, GRAY);
	DrawText(Resume, (GetScreenWidth() - MeasureText(Resume, fontsize)) / 2, GetScreenHeight() / 2 - sizeY / 2 + 35, fontsize, BLACK);
	//quit button
	DrawRectangle(GetScreenWidth() / 2 - sizeX / 2 + 5, GetScreenHeight() / 2 - sizeY / 2 + ButtonY + 15, sizeX - 10, ButtonY, GRAY);
	DrawText(Quit, (GetScreenWidth() - MeasureText(Quit, fontsize)) / 2, GetScreenHeight() / 2 - sizeY / 2 + ButtonY + 45, fontsize, BLACK);

	//button actions
	if (pauseMenuSelection == resume)  DrawRectangleLines(GetScreenWidth() / 2 - sizeX / 2 + 5, GetScreenHeight() / 2 - sizeY / 2 + 5, sizeX - 10, ButtonY, WHITE);
	else if (pauseMenuSelection == quit)  DrawRectangleLines(GetScreenWidth() / 2 - sizeX / 2 + 5, GetScreenHeight() / 2 - sizeY / 2 + ButtonY + 15, sizeX - 10, ButtonY, WHITE);
	if (IsKeyPressed(KEY_ENTER)) {
		if (pauseMenuSelection == resume) gamestatus = run;
		else if (pauseMenuSelection == quit) gamestatus = mainmenu;
	}
}

void UI::drawMainMenu(gamestate& gamestatus) {
	//controles
	switch (mainMenuSelection) {
	case start:
		if (IsKeyPressed(KEY_DOWN)) mainMenuSelection = highscores;
		break;
	case highscores:
		if (IsKeyPressed(KEY_UP)) mainMenuSelection = start;
		else if (IsKeyPressed(KEY_DOWN)) mainMenuSelection = quit;
		break;
	case quit:
		if (IsKeyPressed(KEY_UP)) mainMenuSelection = highscores;
		break;
	default:
		mainMenuSelection = start;
	}

	//game title
	const char* title = "SPACE INVADERS 3D";
	int titleFontSize = 80;
	int titleWidth = MeasureText(title, titleFontSize);
	DrawText(title, (GetScreenWidth() - titleWidth) / 2, GetScreenHeight() / 4, titleFontSize, WHITE);

	const int menuWidth = 400;
	const int menuHeight = 300;
	const int buttonHeight = 80;
	const int buttonSpacing = 20;

	int menuX = GetScreenWidth() / 2 - menuWidth / 2;
	int menuY = GetScreenHeight() / 2 - menuHeight / 2;

	//background
	DrawRectangle(menuX, menuY, menuWidth, menuHeight, DARKGRAY);
	DrawRectangleLines(menuX, menuY, menuWidth, menuHeight, WHITE);

	//button variables
	int button1Y = menuY + 30;
	int button2Y = button1Y + buttonHeight + buttonSpacing;
	int button3Y = button2Y + buttonHeight + buttonSpacing;
	int buttonX = menuX + 20;
	int buttonWidth = menuWidth - 40;
	int fontSize = 40;

	//START button
	Color startColor = (mainMenuSelection == start) ? LIGHTGRAY : GRAY;
	DrawRectangle(buttonX, button1Y, buttonWidth, buttonHeight, startColor);
	DrawRectangleLines(buttonX, button1Y, buttonWidth, buttonHeight,
		(mainMenuSelection == start) ? YELLOW : WHITE);
	const char* startText = "START";
	int startTextWidth = MeasureText(startText, fontSize);
	DrawText(startText, buttonX + (buttonWidth - startTextWidth) / 2,
		button1Y + (buttonHeight - fontSize) / 2, fontSize, BLACK);

	//HIGHSCORES button
	Color highscoreColor = (mainMenuSelection == highscores) ? LIGHTGRAY : GRAY;
	DrawRectangle(buttonX, button2Y, buttonWidth, buttonHeight, highscoreColor);
	DrawRectangleLines(buttonX, button2Y, buttonWidth, buttonHeight,
		(mainMenuSelection == highscores) ? YELLOW : WHITE);
	const char* highscoreText = "HIGHSCORES";
	int highscoreTextWidth = MeasureText(highscoreText, fontSize);
	DrawText(highscoreText, buttonX + (buttonWidth - highscoreTextWidth) / 2,
		button2Y + (buttonHeight - fontSize) / 2, fontSize, BLACK);

	//QUIT button
	Color quitColor = (mainMenuSelection == quit) ? LIGHTGRAY : GRAY;
	DrawRectangle(buttonX, button3Y, buttonWidth, buttonHeight, quitColor);
	DrawRectangleLines(buttonX, button3Y, buttonWidth, buttonHeight,
		(mainMenuSelection == quit) ? YELLOW : WHITE);
	const char* quitText = "QUIT";
	int quitTextWidth = MeasureText(quitText, fontSize);
	DrawText(quitText, buttonX + (buttonWidth - quitTextWidth) / 2,
		button3Y + (buttonHeight - fontSize) / 2, fontSize, BLACK);

	//controles help
	const char* controls = "ARROW KEYS: navigation | ENTER: select";
	int controlsFontSize = 20;
	int controlsWidth = MeasureText(controls, controlsFontSize);
	DrawText(controls, (GetScreenWidth() - controlsWidth) / 2,
		GetScreenHeight() - 50, controlsFontSize, LIGHTGRAY);

	//button actions
	if (IsKeyPressed(KEY_ENTER)) {
		switch (mainMenuSelection) {
		case start:
			gamestatus = newGame;
			break;
		case highscores:
			gamestatus = scoreboard;
			break;
		case quit:
			gamestatus = quitgame;
			break;
		}
	}
}

void UI::drawScoreboard(gamestate& gamestatus) {
	auto highscores = HighscoreManager::getHighscores();
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	const int fontSize = 24;
	const int lineSpacing = 35;
	const int titleFontSize = 32;

	//draw title
	const char* title = "HIGHSCORES";
	int titleWidth = MeasureText(title, titleFontSize);
	int titleX = (screenWidth - titleWidth) / 2;
	int titleY = screenHeight / 2 - 200;
	DrawText(title, titleX, titleY, titleFontSize, WHITE);

	//calculate startposition
	int listStartY = titleY + 60;
	int maxEntries = std::min((int)highscores.size(), HighscoreManager::MAX_HIGHSCORES);

	//draw header
	const char* headerName = "Name";
	const char* headerScore = "Score";
	int headerY = listStartY;
	int nameColumnX = screenWidth / 2 - 150;
	int scoreColumnX = screenWidth / 2 + 50;

	DrawText(headerName, nameColumnX, headerY, fontSize, GREEN);
	DrawText(headerScore, scoreColumnX, headerY, fontSize, GREEN);

	//draw spaceing lines
	DrawLine(nameColumnX - 10, headerY + 30, scoreColumnX + 100, headerY + 30, GREEN);

	// draw highscores
	for (int i = 0; i < maxEntries; i++) {
		int entryY = listStartY + 50 + (i * lineSpacing);

		// ranking
		std::string rank = std::to_string(i + 1) + ".";
		DrawText(rank.c_str(), nameColumnX - 30, entryY, fontSize, GRAY);

		//draw name (12 chars max)
		std::string displayName = highscores[i].name;
		if (displayName.length() > 12) {
			displayName = displayName.substr(0, 12) + "...";
		}
		DrawText(displayName.c_str(), nameColumnX, entryY, fontSize, WHITE);

		//draw score
		std::string scoreText = std::to_string(highscores[i].value);
		DrawText(scoreText.c_str(), scoreColumnX, entryY, fontSize, WHITE);
	}
	//if highscorelist is Empty
	if (highscores.empty()) {
		const char* noScoresText = "No Highscores available";
		int textWidth = MeasureText(noScoresText, fontSize);
		int textX = (screenWidth - textWidth) / 2;
		int textY = listStartY + 80;
		DrawText(noScoresText, textX, textY, fontSize, GRAY);
	}

	//button actions
	if (IsKeyPressed(KEY_ENTER)) {
		gamestatus = mainmenu;
	}
}

bool UI::drawBlackoutScreen(double playerDeathTime) {
	//draw black screen
	double duration = 4.0;
	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, (unsigned char)(100 * ((GetTime() - playerDeathTime) / duration)) });
	if (GetTime() - playerDeathTime > duration) {
		return true; // return true if blackout is finished
	}
	else return false;
}

void UI::drawGameOverScreen(gamestate& gamestatus) {
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();
	//Background
	DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 100 }); // Semi-transparent black background

	// Game Over Title
	const char* gameOverText = "GAME OVER";
	const int titleFontSize = 120;
	int gameOverWidth = MeasureText(gameOverText, titleFontSize);
	// Pulsating effect for the title
	float pulseAmount = sinf(GetTime() * 7.0f) * 0.3f + 0.7f; // Creates a pulsing effect between 0.4 and 1.0 with sin function
	Color gameOverColor = {
		static_cast<unsigned char>(RED.r * pulseAmount),
		static_cast<unsigned char>(RED.g * pulseAmount),
		static_cast<unsigned char>(RED.b * pulseAmount),
		255
	};
	DrawText(gameOverText, (screenWidth - gameOverWidth) / 2, screenHeight / 4, titleFontSize, gameOverColor);

	// Stats panel
	const int panelWidth = 600;
	const int panelHeight = 400;
	const int panelX = (screenWidth - panelWidth) / 2;
	const int panelY = (screenHeight - panelHeight) / 2;
	const int fontSize = 40;
	const int smallFontSize = 30;
	const int lineSpacing = 60;

	// Draw semi-transparent panel background
	DrawRectangle(panelX, panelY, panelWidth, panelHeight, ColorAlpha(DARKGRAY, 0.9f));
	DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, WHITE);

	// Player name
	std::string playerName = game->getPlayer()->name;
	DrawText(playerName.c_str(), panelX + 30, panelY + 30, fontSize, WHITE);
	DrawLine(panelX + 30, panelY + 80, panelX + panelWidth - 30, panelY + 80, GRAY);

	// Score
	int playerScore = game->getPlayer()->getScore();
	std::string scoreText = "SCORE: " + std::to_string(playerScore);
	DrawText(scoreText.c_str(), panelX + 30, panelY + 100, fontSize, WHITE);

	// Check if it's a new highscore
	if (HighscoreManager::getHighestScore() < playerScore) {
		const char* newHighscoreText = "NEW HIGH SCORE!";
		int highscoreWidth = MeasureText(newHighscoreText, smallFontSize);
		DrawText(newHighscoreText, panelX + panelWidth - highscoreWidth - 30,
			panelY + 100, smallFontSize, YELLOW);
	}

	// Get kill counts
	Entity::killCounts kills = game->getPlayer()->getKillCounts();

	// Draw kills statistics
	const int killStatsY = panelY + 100 + lineSpacing;
	const char* killsTitle = "KILLS:";
	DrawText(killsTitle, panelX + 30, killStatsY, fontSize, WHITE);

	// Draw individual kill counts with smaller font and indent
	const int killDetailsX = panelX + 60;
	const int killDetailsSpacing = 40;

	// Asteroids kills
	DrawText("Asteroids:", killDetailsX, killStatsY + killDetailsSpacing, smallFontSize, LIGHTGRAY);
	DrawText(std::to_string(kills.killedAsteroids).c_str(), killDetailsX + panelWidth / 2, killStatsY + killDetailsSpacing, smallFontSize, LIGHTGRAY);

	// Enemy ships kills
	DrawText("Enemy Ships:", killDetailsX, killStatsY + 2 * killDetailsSpacing, smallFontSize, LIGHTGRAY);
	DrawText(std::to_string(kills.killedEnemies).c_str(), killDetailsX + panelWidth / 2, killStatsY + 2 * killDetailsSpacing, smallFontSize, LIGHTGRAY);

	// Total kills
	std::string totalKillsText = std::to_string(kills.killedAsteroids + kills.killedEnemies);
	DrawText("Total:", killDetailsX, killStatsY + 3 * killDetailsSpacing, smallFontSize + 1, WHITE);
	DrawText(totalKillsText.c_str(), killDetailsX + panelWidth / 2, killStatsY + 3 * killDetailsSpacing, smallFontSize, WHITE);

	// Instructions
	const char* pressEnterText = "Press ENTER to continue";
	int enterWidth = MeasureText(pressEnterText, smallFontSize);
	DrawText(pressEnterText, (screenWidth - enterWidth) / 2,
		panelY + panelHeight + 40, smallFontSize, GRAY);

	// Handle input
	if (IsKeyPressed(KEY_ENTER)) {
		HighscoreManager::addScore({ game->getPlayer()->name, playerScore }); // Add score to highscore list
		gamestatus = mainmenu;
	}
}

std::string UI::inputPlayernameScreen(std::string oldPlayername) {
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();
	std::string playerName = oldPlayername;
	const int maxLength = 12;

	// Panel dimensions
	const int panelWidth = 600;
	const int panelHeight = 300;
	const int panelX = (screenWidth - panelWidth) / 2;
	const int panelY = (screenHeight - panelHeight) / 2;

	// Font sizes
	const int titleFontSize = 50;
	const int inputFontSize = 40;
	const int helpFontSize = 20;

	// Title text
	const char* titleText = "ENTER YOUR NAME";
	int titleWidth = MeasureText(titleText, titleFontSize);

	// Input field dimensions
	const int inputFieldWidth = 400;
	const int inputFieldHeight = 60;
	const int inputFieldX = (screenWidth - inputFieldWidth) / 2;
	const int inputFieldY = screenHeight / 2;

	while (!WindowShouldClose()) {
		// Handle input
		int key = GetCharPressed();
		while (key > 0) {
			if ((key >= 32) && (key <= 125) && (playerName.length() < maxLength)) {
				playerName += (char)key;
			}
			key = GetCharPressed();
		}

		// Handle backspace
		if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) {
			playerName.pop_back();
		}

		// Handle enter (confirm name)
		if (IsKeyPressed(KEY_ENTER) && !playerName.empty()) {
			return playerName;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		// Draw semi-transparent background
		DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 100 });

		// Draw panel
		DrawRectangle(panelX, panelY, panelWidth, panelHeight, ColorAlpha(DARKGRAY, 0.9f));
		DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, WHITE);

		// Draw title
		DrawText(titleText, (screenWidth - titleWidth) / 2, panelY + 40, titleFontSize, WHITE);

		// Draw input field background
		DrawRectangle(inputFieldX, inputFieldY, inputFieldWidth, inputFieldHeight, GRAY);
		DrawRectangleLines(inputFieldX, inputFieldY, inputFieldWidth, inputFieldHeight, WHITE);

		// Draw input text (with cursor effect)
		std::string displayText = playerName;
		if (((int)(GetTime() * 2) % 2) == 0) { // Blinking cursor effect
			displayText += "_";
		}
		else displayText += " "; // Add space if no cursor
		int textWidth = MeasureText(displayText.c_str(), inputFontSize);
		DrawText(displayText.c_str(),
			inputFieldX + (inputFieldWidth - textWidth) / 2,
			inputFieldY + (inputFieldHeight - inputFontSize) / 2,
			inputFontSize,
			WHITE);

		// Draw character count
		std::string charCount = std::to_string(playerName.length()) + "/" + std::to_string(maxLength);
		DrawText(charCount.c_str(),
			inputFieldX + inputFieldWidth - MeasureText(charCount.c_str(), helpFontSize) - 10,
			inputFieldY + inputFieldHeight + 5,
			helpFontSize,
			GRAY);

		// Draw help text
		const char* helpText = "Press ENTER to confirm | BACKSPACE to delete";
		int helpWidth = MeasureText(helpText, helpFontSize);
		DrawText(helpText,
			(screenWidth - helpWidth) / 2,
			panelY + panelHeight - 40,
			helpFontSize,
			LIGHTGRAY);

		EndDrawing();
	}

	return playerName; // In case window is closed
}

bool UI::drawLevelUpScreen(double starttime) {
	const int screenWidth = GetScreenWidth();
	const int screenHeight = GetScreenHeight();
	const double animationDuration = 1.5; // Total duration for all animations
	const double holdDuration = 3.5; // Time to hold the level up screen after animations
	const double timeElapsed = GetTime() - starttime;

	// Draw semi-transparent background
	DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 100 });

	// Level Up Title with pulsing effect - same size as Game Over title
	const char* levelUpText = "LEVEL UP!";
	const int titleFontSize = 120; // Same as game over screen
	int levelUpWidth = MeasureText(levelUpText, titleFontSize);
	float pulseAmount = sinf(GetTime() * 7.0f) * 0.3f + 0.7f;
	Color titleColor = {
		static_cast<unsigned char>(YELLOW.r * pulseAmount),
		static_cast<unsigned char>(YELLOW.g * pulseAmount),
		static_cast<unsigned char>(YELLOW.b * pulseAmount),
		255
	};
	DrawText(levelUpText, (screenWidth - levelUpWidth) / 2, screenHeight / 4, titleFontSize, titleColor);

	// Panel dimensions - same as game over screen
	const int panelWidth = 600;
	const int panelHeight = 300;
	const int panelX = (screenWidth - panelWidth) / 2;
	const int panelY = (screenHeight - panelHeight) / 2;
	const int fontSize = 40;
	const int smallFontSize = 30;
	const int lineSpacing = 60;

	// Draw panel with fade-in animation
	float panelAlpha = (float)fmin(1.0, timeElapsed * 2);
	DrawRectangle(panelX, panelY, panelWidth, panelHeight, ColorAlpha(DARKGRAY, 0.9f * panelAlpha));
	DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, ColorAlpha(WHITE, panelAlpha));

	// Stats animations
	const int statsStartY = panelY + 40;
	const double statDelay = animationDuration / 3;

	// Score with animation
	if (timeElapsed > statDelay * 0) {
		float scoreAlpha = (float)fmin(1.0, (timeElapsed - statDelay * 0) * 2);
		int score = game->getPlayer()->getScore();
		const char* scoreLabel = "SCORE:";
		std::string scoreValue = std::to_string(score);

		// Draw label on left and value on right
		DrawText(scoreLabel, panelX + 30, statsStartY, fontSize,
			ColorAlpha(WHITE, scoreAlpha));
		DrawText(scoreValue.c_str(),
			panelX + panelWidth - MeasureText(scoreValue.c_str(), fontSize) - 30,
			statsStartY, fontSize, ColorAlpha(WHITE, scoreAlpha));
	}

	// Kill counts
	Entity::killCounts kills = game->getPlayer()->getKillCounts();
	const int killStatsY = statsStartY + lineSpacing;

	// Draw kills title
	if (timeElapsed > statDelay * 1) {
		float killsAlpha = (float)fmin(1.0, (timeElapsed - statDelay * 1) * 2);
		const char* killsTitle = "KILLS:";
		DrawText(killsTitle, panelX + 30, killStatsY, fontSize,
			ColorAlpha(WHITE, killsAlpha));
	}

	// Asteroids kills with animation
	if (timeElapsed > statDelay * 1.5) {
		float asteroidsAlpha = (float)fmin(1.0, (timeElapsed - statDelay * 1.5) * 2);
		const char* asteroidsLabel = "Asteroids:";
		std::string asteroidsValue = std::to_string(kills.killedAsteroids);

		DrawText(asteroidsLabel, panelX + 60, killStatsY + 40, smallFontSize,
			ColorAlpha(LIGHTGRAY, asteroidsAlpha));
		DrawText(asteroidsValue.c_str(),
			panelX + panelWidth / 2 + 50, killStatsY + 40, smallFontSize,
			ColorAlpha(LIGHTGRAY, asteroidsAlpha));
	}

	// Enemy ships kills with animation
	if (timeElapsed > statDelay * 2) {
		float enemiesAlpha = (float)fmin(1.0, (timeElapsed - statDelay * 2) * 2);
		const char* enemiesLabel = "Enemy Ships:";
		std::string enemiesValue = std::to_string(kills.killedEnemies);

		DrawText(enemiesLabel, panelX + 60, killStatsY + 80, smallFontSize,
			ColorAlpha(LIGHTGRAY, enemiesAlpha));
		DrawText(enemiesValue.c_str(),
			panelX + panelWidth / 2 + 50, killStatsY + 80, smallFontSize,
			ColorAlpha(LIGHTGRAY, enemiesAlpha));
	}

	// Total kills with animation
	if (timeElapsed > statDelay * 2.5) {
		float totalAlpha = (float)fmin(1.0, (timeElapsed - statDelay * 2.5) * 2);
		const char* totalLabel = "Total:";
		std::string totalValue = std::to_string(kills.killedAsteroids + kills.killedEnemies);

		DrawText(totalLabel, panelX + 60, killStatsY + 120, smallFontSize + 1,
			ColorAlpha(WHITE, totalAlpha));
		DrawText(totalValue.c_str(),
			panelX + panelWidth / 2 + 50, killStatsY + 120, smallFontSize,
			ColorAlpha(WHITE, totalAlpha));
	}

	// Return true when all animations are complete
	return timeElapsed >= animationDuration + holdDuration;
}