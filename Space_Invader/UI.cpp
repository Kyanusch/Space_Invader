#include "UI.h"

UI::UI(Gameround* game) : game(game), pauseMenuSelection(resume), mainMenuSelection(start) {
}

UI::~UI() {
}

void UI::drawGameUI() {
    auto player = game->getPlayer();
    drawBars(player);
    drawScore(player);
    
}
void UI::drawScore(Player* player) {
    DrawText(TextFormat("score: %d", player->getScore()), 5, 5, 25, WHITE);
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
    int fontsize = 70;
    const char* Resume = "RESUME";
    const char* Quit = "QUIT";
    //background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0,0,0,100 });
    DrawRectangle(GetScreenWidth() / 2 - sizeX / 2, GetScreenHeight() / 2 - sizeY/2, sizeX, sizeY, DARKGRAY);

    //resume button
    DrawRectangle(GetScreenWidth() / 2 - sizeX / 2 + 5, GetScreenHeight() / 2 - sizeY / 2 + 5, sizeX - 10, ButtonY, GRAY);  
    DrawText(Resume, (GetScreenWidth() - MeasureText(Resume, fontsize)) / 2, GetScreenHeight() / 2 - sizeY / 2 + 20, fontsize, WHITE);
    //quit button
    DrawRectangle(GetScreenWidth() / 2 - sizeX / 2 + 5, GetScreenHeight() / 2 - sizeY / 2 + ButtonY + 15, sizeX - 10, ButtonY, GRAY);
    DrawText(Quit, (GetScreenWidth() - MeasureText(Quit, fontsize)) / 2, GetScreenHeight() / 2 - sizeY / 2 + ButtonY + 30, fontsize, WHITE);
    
    //button actions
    if (pauseMenuSelection == resume)  DrawRectangleLines(GetScreenWidth() / 2 - sizeX / 2 + 5, GetScreenHeight() / 2 - sizeY / 2 + 5, sizeX - 10, ButtonY, WHITE);
    else if (pauseMenuSelection == quit)  DrawRectangleLines(GetScreenWidth() / 2 - sizeX / 2 + 5, GetScreenHeight() / 2 - sizeY / 2 + ButtonY + 15, sizeX - 10, ButtonY, WHITE);
    if (IsKeyPressed(KEY_ENTER)) {
        if (pauseMenuSelection == resume) gamestatus = run;
        else if(pauseMenuSelection == quit) gamestatus = mainmenu;
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
            gamestatus = run;
            break;
        case highscores:
            gamestatus = scoreboard;
            break;
        case quit:
            //gamestatus = quit;
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