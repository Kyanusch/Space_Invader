#include "HighscoreManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

namespace HighscoreManager {

    std::vector<score> highscores;

    void loadHighscores() {
        highscores.clear();
        std::ifstream file(HIGHSCORE_FILE);

        if (!file) {
            std::cerr << "INFO: HighscoreManager: High score file not found. New file will be created.\n";
            std::ofstream newFile(HIGHSCORE_FILE);
            if (!newFile) {
                std::cerr << "ERROR: HighscoreManager: Could not create high score file.\n";
            }
            return;
        }

        std::string name;
        int value;
        while (file >> name >> value) {
            if (file.fail()) {
                std::cerr << "WARNING: HighscoreManager: Invalid entry in high score file.\n";
                file.clear();
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            highscores.push_back({ name, value });
        }

        file.close();
        std::sort(highscores.begin(), highscores.end(), [](const score& a, const score& b) {
            return b.value < a.value;
            });

        if (highscores.size() > MAX_HIGHSCORES) {
            highscores.resize(MAX_HIGHSCORES);
        }
    }

    void saveHighscores() {
        std::ofstream file(HIGHSCORE_FILE, std::ios::trunc);
        if (!file) {
            std::cerr << "ERROR: HighscoreManager: Could not open high score file for writing.\n";
            return;
        }

        for (const auto& entry : highscores) {
            file << entry.name << " " << entry.value << "\n";
        }

        file.close();
    }

    void initialize() {
        loadHighscores();
    }

    void deinitialize() {
        saveHighscores();
    }

    std::vector<score> getHighscores() {
        return highscores;
    }

    void addScore(score newScore) {
        highscores.push_back(newScore);
        std::sort(highscores.begin(), highscores.end(), [](const score& a, const score& b) {
            return b.value < a.value;
            });

        if (highscores.size() > MAX_HIGHSCORES) {
            highscores.resize(MAX_HIGHSCORES);
        }
    }

    void updateHighscores() {
        saveHighscores();
    }
}
