#include <WiFi.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>

// Defining Part
#define LED 2
#define DIR 12
#define STP 13

//I2C Defining Part
BH1750 lightMeter;
Adafruit_BMP085 bmp;

//Motor Control Part
const int MAX_SPEED = 750;  // Maksymalna prędkość obrotów silnika
int motorSpeed = 0;
float volts, VoltPercent, amps, microamps, lux, alsValue;
bool motorRunning = false;
bool stopMotor = false;

//Wifi Parameters Part
const char* ssid     = "FunBox2-3AC1";
const char* password = "mietek16";
WiFiServer server(80);


void setup()
{
  //I2C Defining Part2
  Wire.begin();
  lightMeter.begin();
  if (!bmp.begin())
  {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }

  Serial.begin(115200);
  pinMode(LED, OUTPUT);      // set the LED pin mode
  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  digitalWrite(DIR, LOW);
  delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

void loop()
{
  float bh1750Lux = lightMeter.readLightLevel();
  float bmpTemperature = bmp.readTemperature();
  float bmpPressure = bmp.readPressure();
  float bmpAltitude = bmp.readAltitude(101500); // Adjust for your current sea level pressure




 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
