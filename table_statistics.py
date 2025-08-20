#Используемые библиотеки
import yaml
import csv
from collections import defaultdict
import os
from dotenv import load_dotenv
#Чтение перменных из файла .env
path = r'd:\CodeRun\table_statistics\table_statistics\variables.env'
load_dotenv(path)
def load_yaml(file_path):
    '''Функция для чтения .yaml'''
    with open(file_path, 'r') as file:
        return yaml.safe_load(file)
def load_logs(file_path):
    '''Функция для чтения .csv'''
    logs = []
    with open(file_path, 'r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            logs.append(row['node_id'])
    return logs
def find_statistics_nodes(folders):
    '''Функция для поиска по узлу'''
    #Мноежство для хранения результатов
    statistics_nodes = set()
    #Функция для проверки текущей ссылки на link
    def traverse(node):
        if 'link' in node:
            return
        if node.get('name') == 'statistics':
            statistics_nodes.add(node)
            return
        #Рекурсивный обход дочерних узлов
        for key, child in node.get('values', {}).items():
            traverse(child)
    #Рекурсивный обход узлов
    for key, node in folders.items():
        traverse(node)
    return statistics_nodes
def get_all_node_ids(node):
    '''Функция для получения id узла'''
    node_ids = set()
    #Функция для проверки отношения узла с link
    def collect_ids(node):
        if 'link' in node:
            return
        if 'values' in node:
            #Рекурсивный обход дочерних ключей
            for key, child in node['values'].items():
                node_ids.add(key)
                collect_ids(child)
    collect_ids(node)
    return node_ids
def main():
    # Считывание нахождения в директории
    file_path_yaml = os.getenv("file_path_yaml")
    file_path_csv = os.getenv("file_path_csv")
    folders = load_yaml(file_path_yaml)
    logs = load_logs(file_path_csv)
    #Идентификаторы узлов statistics
    home_statistics_id = '9cae29'
    tmp_statistics_id = '82cc0d'
    def get_all_node_ids(node):
        """Рекурсивно собирает все ID узлов в поддереве"""
        node_ids = set()
        if 'values' in node:
            for key, child in node['values'].items():
                node_ids.add(key)
                node_ids.update(get_all_node_ids(child))
        return node_ids
    def find_statistics_node(folders, target_id):
        """Находит узел statistics по его ID"""
        for key, node in folders.items():
            if key == target_id:
                return node
            if 'values' in node:
                found = find_statistics_node(node['values'], target_id)
                if found:
                    return found
        return None
    #Находим узел /home/statistics
    statistics_node = find_statistics_node(folders, home_statistics_id)
    if not statistics_node:
        print("Узел 'statistics' не найден в структуре.")
    else:
        #Собираем все ID узлов в поддереве statistics
        target_ids = get_all_node_ids(statistics_node)
        target_ids.add(home_statistics_id)  # Добавляем сам узел statistics
        #Подсчет обращений
        count = sum(1 for node_id in logs if node_id in target_ids)
        print(f"Количество обращений к /home/statistics и подузлам: {count}")
#Конструкция для скрипта
if __name__ == "__main__":
    main()