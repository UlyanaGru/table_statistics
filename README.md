## table_statistics

Проект для анализа статистики обращений к узлам в структуре YAML на основе данных из CSV логов

### Функциональность

- Чтение и парсинг YAML файла с структурой узлов
- Загрузка логов обращений из CSV файла
- Рекурсивный поиск узла statistics в древовидной структуре
- Сбор всех идентификаторов подузлов statistics
- Подсчет количества обращений к узлу statistics и его подузлам

### Установка и запуск

1. Убедитесь, что файл variables.env содержит правильные пути:
    - `file_path_yaml=data/structure.yaml`
    - `file_path_csv=data/logs.csv`
2. Скомпилируйте проект:\
`javac -cp ".;snakeyaml-1.33.jar" table_statistics.java`
3. Зпустите программу:\
`java -cp ".;snakeyaml-1.33.jar" table_statistics`

### Формат данных

1. YAML файл structure.yaml

node_id:\
  name: node_name\
  values:\
    child_node_id:\
      name: child_name

2. CSV файл logs.csv

node_id,timestamp,other_data\
9cae29,2023-01-01,...\
82cc0d,2023-01-01,...

### Реализация

- Рекурсивный обход древовидной структуры YAML
- Эффективный поиск с использованием HashSet для быстрого сравнения
- Обработка больших файлов через потоковое чтение
- Поддержка сложных вложенных структур

Файлы можно найти по [ссылке](https://disk.yandex.ru/d/bwL030DyUv9TvA)

### Решения

- [x] [Python](https://github.com/UlyanaGru/table_statistics/blob/master/table_statistics.py)
- [x] [Java](https://github.com/UlyanaGru/table_statistics/blob/master/table_statistics.java)
- [x] [C++](https://github.com/UlyanaGru/table_statistics/blob/master/table_statisctis.cxx)