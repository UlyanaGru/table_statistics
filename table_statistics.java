//Вызов библиотек
import java.io.*;
import java.util.*;
import java.nio.file.Paths;
import java.nio.file.Files;
//Используемые библиотеки
public class TableStatistics {
    //Функция для чтения .yaml
    public static Map<String, Object> loadYaml(String filePath) throws IOException {
        /**
         *Функция для чтения .yaml
         *@param filePath путь к файлу YAML
         *@return Map с данными из YAML
         */
        Map<String, Object> data = new HashMap<>();
        return data;
    }
    //Функция для чтения .csv
    public static List<String> loadLogs(String filePath) throws IOException {
        /**
         *Функция для чтения .csv
         *@param filePath путь к CSV файлу
         *@return List с node_id из логов
         */
        List<String> logs = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line = reader.readLine(); //Читаем заголовок
            if (line != null && line.contains("node_id")) {
                while ((line = reader.readLine()) != null) {
                    String[] columns = line.split(",");
                    if (columns.length > 0) {
                        logs.add(columns[0]); //Предполагаем, что node_id в первом столбце
                    }
                }
            }
        }
        return logs;
    }
    
    //Функция для поиска узла statistics по ID
    public static Node findStatisticsNode(Map<String, Node> folders, String targetId) {
        for (Map.Entry<String, Node> entry : folders.entrySet()) {
            if (entry.getKey().equals(targetId)) {
                return entry.getValue();
            }
            if (entry.getValue().getValues() != null) {
                Node found = findStatisticsNode(entry.getValue().getValues(), targetId);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }