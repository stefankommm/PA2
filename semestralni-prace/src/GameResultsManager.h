//
// Created by stefam on 6/7/23.
//

#include <vector>
#include <string>

class GameResultsManager {
public:
    static void addResult(const std::pair<std::string, int> &result) {
        auto position = std::upper_bound(results.begin(), results.end(), result, compareResults);
        results.insert(position, result);
    }

    static std::vector<std::pair<std::string, int>> getResults() {
        return results;
    }

private:
    static bool compareResults(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
        return a.second > b.second;
    }

    static std::vector<std::pair<std::string, int>> results;
};

std::vector<std::pair<std::string, int>> GameResultsManager::results = {};
