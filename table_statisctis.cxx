#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

//Функция для загрузки переменных из файла .env
std::unordered_map<std::string, std::string> load_env(const std::string& file_path) {
    std::unordered_map<std::string, std::string> env_vars;
    std::ifstream file(file_path);
    std::string line;
    
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            env_vars[key] = value;
        }
    }
    return env_vars;
}