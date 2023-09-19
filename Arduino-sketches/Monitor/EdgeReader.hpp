#include <SD.h>
// #include <string.h>

#define PIN_SPI_CS 4

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
    // File myFile;

    struct Edge edgeStruct(String line);

public:
    EdgeReader(/* args */);
    ~EdgeReader();

    void test();
    struct Edge tester();
};

EdgeReader::EdgeReader(/* args */)
{
}

EdgeReader::~EdgeReader()
{
}

struct Edge EdgeReader::tester() {
    // String line = "(648,\"controller.setcurrent[20].[]\",659)";
    String line = "(671,\"hc_unit.switchoff[].[]\",679)";
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
    String value = sub_str.substring(sub_str.indexOf("[") + 1, sub_str.indexOf("]"));
    edge.value = -1;

    if (value.length() > 0)
        edge.value = value.toInt();

    return edge;
}


void EdgeReader::test() {
    File myFile;

    Serial.begin(115200);

    if (!SD.begin(PIN_SPI_CS)) {
        Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
        while (1); // don't do anything more:
    }

    Serial.println(F("SD CARD INITIALIZED."));

    if (!SD.exists("arduino.txt")) {
        Serial.println(F("arduino.txt doesn't exist. Creating arduino.txt file..."));
        // create a new file by opening a new file and immediately close it
        myFile = SD.open("arduino.txt", FILE_WRITE);
        myFile.close();
    }

    // recheck if file is created or not
    if (SD.exists("arduino.txt"))
        Serial.println(F("arduino.txt exists on SD Card."));
    else
        Serial.println(F("arduino.txt doesn't exist on SD Card."));
}