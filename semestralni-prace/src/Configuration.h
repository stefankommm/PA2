//
// Created by stefam on 14. 5. 2023.
//

#pragma once


#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "constants.h"



using namespace std;
class Configuration {
public:

    explicit Configuration(const string & filename) {};
    static std::vector<std::vector<CellType>> loadMapFromFile(const string & mapLocation);
    static LevelSettings loadConfiguration (int difficulty);
//    static map<string, string> writeConfiguration (const string & difficulty, const string & name);
//    static map<string, string> writeDefaultConfiguration ();


private:
};

