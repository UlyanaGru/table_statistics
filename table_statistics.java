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
    
    //Функция для получения всех ID узлов в поддереве
    public static Set<String> getAllNodeIds(Node node) {
        Set<String> nodeIds = new HashSet<>();
        collectIds(node, nodeIds);
        return nodeIds;
    }
    
    private static void collectIds(Node node, Set<String> nodeIds) {
        if (node == null) return;
        if (node.getValues() != null) {
            for (Map.Entry<String, Node> entry : node.getValues().entrySet()) {
                nodeIds.add(entry.getKey());
                collectIds(entry.getValue(), nodeIds);
            }
        }
    }
    
    //Функция для преобразования Map<String, Object> в Map<String, Node>
    @SuppressWarnings("unchecked")
    private static Map<String, Node> convertToNodeMap(Map<String, Object> rawData) {
        Map<String, Node> result = new HashMap<>();
        for (Map.Entry<String, Object> entry : rawData.entrySet()) {
            if (entry.getValue() instanceof Map) {
                Node node = new Node();
                Map<String, Object> nodeData = (Map<String, Object>) entry.getValue();
                
                if (nodeData.containsKey("name")) {
                    node.setName(nodeData.get("name").toString());
                }
                if (nodeData.containsKey("link")) {
                    node.setLink(nodeData.get("link").toString());
                }
                if (nodeData.containsKey("values")) {
                    Map<String, Object> valuesData = (Map<String, Object>) nodeData.get("values");
                    node.setValues(convertToNodeMap(valuesData));
                }
                
                result.put(entry.getKey(), node);
            }
        }
        return result;
    }
    
    public static void main(String[] args) {
        try {
            //Загрузка переменных из .env файла
            String envPath = "d:\\CodeRun\\table_statistics\\table_statistics\\variables.env";
            Map<String, String> envVars = new HashMap<>();
            
            try (BufferedReader reader = new BufferedReader(new FileReader(envPath))) {
                String line;
                while ((line = reader.readLine()) != null) {
                    if (line.contains("=") && !line.trim().startsWith("#")) {
                        String[] parts = line.split("=", 2);
                        if (parts.length == 2) {
                            envVars.put(parts[0].trim(), parts[1].trim());
                        }
                    }
                }
            }
            
            String filePathYaml = envVars.get("file_path_yaml");
            String filePathCsv = envVars.get("file_path_csv");
            
            if (filePathYaml == null || filePathCsv == null) {
                System.err.println("Не найдены необходимые переменные в .env файле");
                return;
            }
            
            //Идентификаторы узлов statistics
            String homeStatisticsId = "9cae29";
            
            //Загрузка данных
            Map<String, Object> rawData = loadYaml(filePathYaml);
            Map<String, Node> folders = convertToNodeMap(rawData);
            List<String> logs = loadLogs(filePathCsv);
            
            //Находим узел /home/statistics
            Node statisticsNode = findStatisticsNode(folders, homeStatisticsId);
            
            