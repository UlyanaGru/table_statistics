#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <yaml-cpp/yaml.h>
#include <cstdlib>

//Функция для загрузки переменных из .env файла
void loadEnv(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть .env файл: " << path << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t equals_pos = line.find('=');
        if (equals_pos != std::string::npos) {
            std::string key = line.substr(0, equals_pos);
            std::string value = line.substr(equals_pos + 1);
            setenv(key.c_str(), value.c_str(), 1);
        }
    }
}

//Функция для загрузки CSV файла
std::vector<std::string> loadLogs(const std::string& file_path) {
    std::vector<std::string> logs;
    std::ifstream file(file_path);
    
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть CSV файл: " << file_path << std::endl;
        return logs;
    }
    
    std::string line;
    //Пропускаем заголовок
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        size_t comma_pos = line.find(',');
        if (comma_pos != std::string::npos) {
            std::string node_id = line.substr(0, comma_pos);
            logs.push_back(node_id);
        }
    }
    
    return logs;
}