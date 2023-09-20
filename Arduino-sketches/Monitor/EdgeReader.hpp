#include <SD.h>
// #include <Vector.h>

#define PIN_SPI_CS 53 // Mega
// #define PIN_SPI_CS 4 // Uno 
#define BAUD       115200
#define MAXTRANSITIONS 121

struct Edge
{
    int from;
    int label;
    // String label;
    int value;
    int to;
};

class EdgeReader
{
private:
    // Edge _edges[MAXTRANSITIONS];
    int states(String line);
    bool checks(String filename);
    struct Edge edgeStruct(String line);
    int replaceLabel(String label);

public:
    EdgeReader();
    ~EdgeReader();

    void test();
    int getEdges();
    int getStateNames();
    // Vector<struct Edge> getEdges();
    void printEdge(struct Edge edge);

    struct Edge* edges;
    String* edgenames;
    int edges_size = 0;
    int edgenames_size = 0;
};

EdgeReader::EdgeReader() { }
EdgeReader::~EdgeReader() { }

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);  
}

void display_freeram() {
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
}

void EdgeReader::test() {
    Serial.begin(BAUD);
    Serial.println("Glider in i EdgeReader");

    auto states_nr = getStateNames();

    for (size_t i = 0; i < states_nr; i++)
    {
        Serial.println(edgenames[i]);
    }

    auto transitions = getEdges();

    for (size_t i = 0; i < transitions; i++)
    {
        printEdge(edges[i]);
    }
    
    Serial.println(F("Done printing"));
}

int EdgeReader::replaceLabel(String label) {
    int replacement = -1;

    for (size_t i = 0; i < edgenames_size; i++)
    {
        replacement = label == edgenames[i] ? i : replacement;
    }
    
    return replacement;
}

// Given a string containing an edge return a struct Edge with the data
struct Edge EdgeReader::edgeStruct(String line) {
    if (!line.startsWith("(") && !line.endsWith(")"))
    {
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
    // edge.label = sub_str.substring(label_begin + 2, sub_str.indexOf("["));

    // Some transitions do not have a value, send -1 instead
    String value = sub_str.substring(sub_str.indexOf("[") + 1, sub_str.indexOf("]"));
    edge.value = value.length() > 0 ? value.toInt() : -1;

    return edge;
}

void EdgeReader::printEdge(struct Edge edge) {
    Serial.print(edge.from);
    Serial.print(" -> ");
    Serial.print(edge.label);
    Serial.print(" -> ");
    Serial.print(edge.to);
    Serial.print(" -> ");
    Serial.println(edge.value);
}

bool EdgeReader::checks(String filename) {
    if (!SD.begin(PIN_SPI_CS)) {
        Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
        return false;
    }
    
    if (!SD.exists(filename))
    {
        Serial.println(F("File not found"));
        return false;
    }

    return true;
}

int EdgeReader::states(String line) {
    if (!line.startsWith("des"))
    {
        return -1;
    }
    
    auto sub_line = line.substring(line.indexOf(",") + 1, line.lastIndexOf(",")).toInt();

    return sub_line <= 0 ? -1 : sub_line;
}

int EdgeReader::getEdges() {
    const String filename = "states.aut";

    if (!checks(filename))
    {
        Serial.println(F("Error opening SD-card"));
        exit(EXIT_FAILURE);
    }
    
    File file = SD.open(filename);
    int transitions = states(file.readStringUntil('\n'));

    if (transitions <= 0)
    {
        Serial.println(F("Error reading number of states"));
        exit(EXIT_FAILURE);
    }

    edges = new Edge[transitions];

    if (edges == NULL)
    {
        Serial.println(F("Error allocating memory"));
        exit(EXIT_FAILURE);
    }
    
    int transitions_added = 0;

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        edges[transitions_added] = edgeStruct(line);
        transitions_added += 1;
    }
    
    file.close();

    return transitions_added;
}

int EdgeReader::getStateNames() {
    const String filename = "states.txt";

    if (!checks(filename))
    {
        Serial.println(F("Error opening SD-card"));
        exit(EXIT_FAILURE);
    }
    
    File file = SD.open(filename);
    int states_len = file.read();

    if (states_len <= 0)
    {
        Serial.println(F("Error reading number of states"));
        exit(EXIT_FAILURE);
    }
    
    edgenames = new String[states_len];

    if (edgenames == NULL)
    {
        Serial.println(F("Error allocating memory"));
        exit(EXIT_FAILURE);
    }

    int states_added = 0;

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        edgenames[states_added] = line;
        states_added += 1;
    }
    
    file.close();

    return states_added;
}

//  Vector<struct Edge> EdgeReader::getEdges() { 
//     File file;
//     Vector<struct Edge> edges;
//     // Edge _edges[MAXTRANSITIONS]; //deklarerad i private
//     edges.setStorage(_edges);


//     file = SD.open(transitionsFile);

//     String des = file.readStringUntil('\n');
//     // Serial.println(file.readStringUntil('\n'));

//     while (file.available())
//     {
//         String line = file.readStringUntil('\n');
//         Edge edge = edgeStruct(line);
//         // printEdge(edge);
//         edges.push_back(edge);
//     }

//     // display_freeram();
//     file.close();

//     return edges;
// }

