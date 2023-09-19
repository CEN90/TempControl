#include <SD.h>
// #include <Array.h>
#include <Vector.h>

#define PIN_SPI_CS 53
// #define PIN_SPI_CS 4
#define BAUD       115200
#define MAXTRANSITIONS 121

struct Edge
{
    int from;
    String label;
    int value;
    int to;
};


class EdgeReader
{
private:
    const String filename = "states.aut";
    struct Edge edgeStruct(String line);
    Edge _edges[MAXTRANSITIONS];

public:
    EdgeReader(/* args */);
    ~EdgeReader();

    void test();
    struct Edge tester();
    void printEdge(struct Edge edge);
    // void getEdges();
    Vector<struct Edge> getEdges();
};

EdgeReader::EdgeReader(/* args */)
{
}

EdgeReader::~EdgeReader()
{
}

struct Edge EdgeReader::tester() {
    String line = "(648,\"controller.setcurrent[20].[]\",659)";
    // String line = "(671,\"hc_unit.switchoff[].[]\",679)";
    return this->edgeStruct(line);
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
    edge.label = sub_str.substring(label_begin + 2, sub_str.indexOf("["));

    // Some transitions do not have a value, send -1 instead
    String value = sub_str.substring(sub_str.indexOf("[") + 1, sub_str.indexOf("]"));
    edge.value = value.length() > 0 ? value.toInt() : -1;

    return edge;
}


int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int) __brkval);  
}

void display_freeram() {
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
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

 Vector<struct Edge> EdgeReader::getEdges() { 
    File file;
    Vector<struct Edge> edges;
    // Edge _edges[MAXTRANSITIONS];
    edges.setStorage(_edges);
    int size = 0;

    // Serial.begin(BAUD);

    if (!SD.begin(PIN_SPI_CS)) {
        // Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
        while (1); // don't do anything more:
    }
    
    if (!SD.exists(filename))
    {
        // Serial.println(F("File not found"));
        exit(EXIT_FAILURE);
    }

    file = SD.open(filename);

    String des = file.readStringUntil('\n');
    // Serial.println(file.readStringUntil('\n'));

    while (file.available())
    {
        String line = file.readStringUntil('\n');
        Edge edge = edgeStruct(line);
        // printEdge(edge);
        // Serial.print(edge.from);
        // Serial.print(" -> ");
        // Serial.print(edge.label);
        // Serial.print(" -> ");
        // Serial.print(edge.to); 
        // Serial.print(" -> ");
        // Serial.println(edge.value);
        edges.push_back(edge);
        size += 1;
    }

    display_freeram();

    // for (size_t i = 0; i < size; i++)
    // {
    //     printEdge(edges.at(i));
    // }
    

    // Serial.println(edges.size());    
    file.close();

    return edges;
}

