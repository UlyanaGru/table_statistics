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

//Рекурсивная функция для сбора всех ID узлов в поддереве
void getAllNodeIds(const YAML::Node& node, std::unordered_set<std::string>& node_ids) {
    if (node.IsMap()) {
        if (node["values"]) {
            const YAML::Node& values = node["values"];
            for (YAML::const_iterator it = values.begin(); it != values.end(); ++it) {
                std::string key = it->first.as<std::string>();
                node_ids.insert(key);
                getAllNodeIds(it->second, node_ids);
            }
        }
    }
}

//Рекурсивная функция для поиска узла по ID
const YAML::Node findStatisticsNode(const YAML::Node& node, const std::string& target_id) {
    if (node.IsMap()) {
        //Проверяем текущий узел
        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
            std::string key = it->first.as<std::string>();
            if (key == target_id) {
                return it->second;
            }
            
            //Рекурсивно ищем в дочерних узлах
            if (it->second.IsMap() && it->second["values"]) {
                const YAML::Node& found = findStatisticsNode(it->second["values"], target_id);
                if (found.IsDefined()) {
                    return found;
                }
            }
        }
    }
    
    return YAML::Node(); //Возвращаем пустой узел если не нашли
}

int main() {
    //Загрузка переменных из .env файла
    std::string env_path = "d:\\CodeRun\\table_statistics\\table_statistics\\variables.env";
    loadEnv(env_path);
    
    const char* yaml_path = std::getenv("file_path_yaml");
    const char* csv_path = std::getenv("file_path_csv");
    
    if (!yaml_path || !csv_path) {
        std::cerr << "Переменные окружения не найдены" << std::endl;
        return 1;
    }
    
    try {
        //Загрузка YAML файла
        YAML::Node folders = YAML::LoadFile(yaml_path);
        
        //Загрузка CSV файла
        std::vector<std::string> logs = loadLogs(csv_path);
        
        //Идентификаторы узлов statistics
        std::string home_statistics_id = "9cae29";
        
        //Находим узел /home/statistics
        const YAML::Node statistics_node = findStatisticsNode(folders, home_statistics_id);

        if (!statistics_node.IsDefined()) {
            std::cout << "Узел 'statistics' не найден в структуре." << std::endl;
            return 1;
        }
        
        //Собираем все ID узлов в поддереве statistics
        std::unordered_set<std::string> target_ids;
        target_ids.insert(home_statistics_id);
        getAllNodeIds(statistics_node, target_ids);
        
        // Подсчет обращений
        int count = 0;
        for (const auto& node_id : logs) {
            if (target_ids.find(node_id) != target_ids.end()) {
                count++;
            }
        }
        
        std::cout << "Количество обращений к /home/statistics и подузлам: " << count << std::endl;
        
    } catch (const YAML::Exception& e) {
        std::cerr << "Ошибка при обработке YAML файла: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}