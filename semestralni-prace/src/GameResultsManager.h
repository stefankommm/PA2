#pragma once

#include <vector>
#include <string>


/**
 * @brief Stores scores of the players.
 * Handles saving and loading results from after playing game (Playing mode).
 * Loads the results in the RankingTable Mode.
 */
class GameResultsManager {
public:
    /**
     * @brief Adds new score to the vector
     * @param result pair of the string and the appropriate score achieved.
     */
    static void addResult(const std::pair<std::string, int> &result) {
        // Remove invisible characters Enter,esc, newline...
        std::string cleanedString;
        for (char c : result.first) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                cleanedString += c;
            }
        }

        // If not empty add it to the list
        if (!cleanedString.empty()) {
            auto position = std::upper_bound(results.begin(), results.end(), std::make_pair(cleanedString, result.second), compareResults);
            results.insert(position, std::make_pair(cleanedString, result.second));
        }
    }

    /**
     * @brief Passes the reference to the scores.
     * @return Returns the reference to the saved scores
     */
    static std::vector<std::pair<std::string, int>> getResults() {
        return results;
    }

private:
    /**
     * Internal comparing function for handling the sorted array.
     * @param a First pair of the vector
     * @param b Second pair of the vector
     * @return true if the first pair has bigger score.
     */
    static bool compareResults(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
        return a.second > b.second;
    }

    static std::vector<std::pair<std::string, int>> results;
};

std::vector<std::pair<std::string, int>> GameResultsManager::results = {};
