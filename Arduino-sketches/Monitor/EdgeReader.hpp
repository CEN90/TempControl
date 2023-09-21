#include <SD.h>

// #include <Vector.h>

#define PIN_SPI_CS 53 // Mega
// #define PIN_SPI_CS 10 // Uno 
#define BAUD       115200
#define MAXTRANSITIONS 121

struct Edge
{
    int from;
    int label;
    int value;
    int to;
};

enum FileOp { Transition, Name };

class EdgeReader
{
private:
    int totalTransitions(String line);
    bool checkStorage(String filename);
    int replaceLabel(String label);
    struct Edge edgeStruct(String line);
    int readFile(String filename, FileOp op);

    struct Edge* edges;
    String* edgenames;
    int edges_size = 0;
    int edgenames_size = 0;

public:
    EdgeReader();
    ~EdgeReader();

    void test();
    void printEdge(struct Edge edge);

    int getEdges(struct Edge* edges);
    int getEdgenames(String* edgenames);
};

EdgeReader::EdgeReader() { }
EdgeReader::~EdgeReader() { }

// Get function for edge names
int EdgeReader::getEdgenames(String* _edgenames) {
    if (_edgenames != NULL || edgenames == NULL || edgenames_size <= 0) {
        return -1;
    }
    
    _edgenames = edgenames;
    return edgenames_size;
}

// Get function for edges
int EdgeReader::getEdges(struct Edge* _edges) {
    if (_edges != NULL || edges == NULL || edges_size <= 0) {
        return -1;
    }
    
    _edges = edges;
    return edges_size;
}

void EdgeReader::test() {
    Serial.println("Glider in i EdgeReader");

    edgenames_size = readFile("states.txt", Name);
    edges_size = readFile("states.aut", Transition);

    for (size_t i = 0; i < edgenames_size; i++) {
        Serial.println(edgenames[i]);
    }

    for (size_t i = 0; i < edges_size; i++) {
        printEdge(edges[i]);
    }

    Serial.println(F("Done printing"));
}

// Storing transition label as a string and ref later due to memory shortage on arduino
int EdgeReader::replaceLabel(String label) {
    int replacement = 0;

    for (int i = 0; i < edgenames_size; i++) {
        if (edgenames[i].equals(label)) {
            replacement = i;
        }
    }
    
    return replacement; // ID for later
}

// Given a string containing an edge return a struct Edge with the data
struct Edge EdgeReader::edgeStruct(String line) {
    if (!line.startsWith("(") && !line.endsWith(")")) {
        Serial.println(F("Mismatched input, not an edge!"));
        delay(100);
        exit(EXIT_FAILURE);
    }
    
    String sub_str = line.substring(1, line.length() - 1); 
    auto label_begin = sub_str.indexOf(",\"");
    auto label_end = sub_str.indexOf("\",");
    
    Edge edge;
    
    // Hard coded magic values, look away...
    edge.from = sub_str.substring(0, label_begin).toInt();
    edge.to = sub_str.substring(label_end + 2).toInt();
    edge.label = replaceLabel(sub_str.substring(label_begin + 2, sub_str.indexOf("[")));

    // Some transitions do not have a value, send -1 instead
    String value = sub_str.substring(sub_str.indexOf("[") + 1, sub_str.indexOf("]"));
    edge.value = value.length() > 0 ? value.toInt() : -1;

    return edge;
}

void EdgeReader::printEdge(struct Edge edge) {
    Serial.print(edge.from);
    Serial.print(" -> ");
    Serial.print(edgenames[edge.label]);
    // Serial.print(edge.label);
    Serial.print(" -> ");
    Serial.print(edge.to);
    Serial.print(" -> ");
    Serial.println(edge.value);
}

// Simple pre-flight checks
bool EdgeReader::checkStorage(String filename) {
    if (!SD.begin(PIN_SPI_CS)) {
        Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
        return false;
    }
    
    if (!SD.exists(filename)) {
        Serial.println(F("File not found"));
        return false;
    }

    return true;
}

// Get total transitions from line 'des ([start], [total transitions], [total states])'
int EdgeReader::totalTransitions(String line) {
    if (!line.startsWith("des")) {
        return -1;
    }
    
    auto sub_line = line.substring(line.indexOf(",") + 1, line.lastIndexOf(",")).toInt();

    return sub_line <= 0 ? -1 : sub_line;
}

// Combined function to read in names of transitions and the transitions themselves, therefore FileOp paramater
int EdgeReader::readFile(String filename, FileOp op) {
    if (!checkStorage(filename)) {
        Serial.println(F("Error opening SD-card"));
        delay(100);
        exit(EXIT_FAILURE);
    }
    
    int file_len = 0;
    File file = SD.open(filename);

    // Allocating different arrs depending on operation
    if (op == Transition) {
        file_len = totalTransitions(file.readStringUntil('\n'));
        edges = new Edge[file_len];
    }
    
    if (op == Name) {
        file_len = file.read();
        edgenames = new String[file_len];
    }
    
    if (file_len <= 0) {
        Serial.println(F("Error reading number of states"));
        delay(100);
        exit(EXIT_FAILURE);
    }

    if (op == Transition && edges == NULL || op == Name && edgenames == NULL) {
        Serial.println(F("Error allocating memory"));
        delay(100);
        exit(EXIT_FAILURE);
    }
    
    int lines_added = 0;

    while (file.available()) {
        String line = file.readStringUntil('\n');

        if (op == Transition) {
            edges[lines_added] = edgeStruct(line);
        }
    
        if (op == Name) {
            edgenames[lines_added] = line;
        }

        lines_added += 1;
    }
    
    file.close();

    return lines_added;
}
