#pragma once
#include "raylib.h"
#include <string>
#include <vector>

namespace HighscoreManager {

    struct score {
        std::string name;
        int value;
    };
    const std::string HIGHSCORE_FILE = "highscore.txt";
    const int MAX_HIGHSCORES = 10;

    void initialize();
    void deinitialize();
    std::vector<score> getHighscores();
	int getHighestScore();
    void addScore(score newScore);
    void updateHighscores();

}