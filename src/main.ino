////////////////////////////////////////////////////////////////////////////////////////////////////////
// Jan Rudecki Elektronika i Telekomunikacja Praca Inżynierska
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Including libraries
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <WiFi.h> // WiFi communication support
#include <BH1750.h> // Light intensity sensor support
#include <Adafruit_BMP085.h> // Temperature sensor support
#include <Adafruit_BMP280.h> // Temperature sensor support
#include <Wire.h> 

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pin assignments
////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DIR 13 // Signal responsible for motor rotation direction
#define STP 12 // Signal responsible for motor step
#define DOWN 14 // Virtual button for lowering the roller blind
#define UP 15   // Virtual button for raising the roller blind

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sensor definitions
////////////////////////////////////////////////////////////////////////////////////////////////////////
Adafruit_BMP085 bmp180;
Adafruit_BMP280 bmp280;
BH1750 bh1750_a;
BH1750 bh1750_b;
BH1750 bh1750_c;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// WiFi access definition
////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* ssid = "FunBox2-3AC1";
const char* password = "mietek16";
WiFiServer server(80);

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Motor speed definition
////////////////////////////////////////////////////////////////////////////////////////////////////////
const int MAX_SPEED = 750;
int motorSpeed = 0;
bool motorRunning = false;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Error counters initialization
////////////////////////////////////////////////////////////////////////////////////////////////////////
int error_counter_bmp180 = 0;
int error_counter_bmp280 = 0;
int error_counter_1_a = 0;
int error_counter_2_a = 0;
int error_counter_1_b = 0;
int error_counter_2_b = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup function
////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  Wire1.begin(18, 19);

  if (!bmp180.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1) {}
  }

  if (!bmp280.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1) {}
  }

  bh1750_a.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire);
  bh1750_b.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C, &Wire);
  bh1750_c.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire1);

  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  digitalWrite(DIR, LOW);
  delay(10);

  pinMode(DOWN, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Loop function
////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  float temperature_bmp180 = bmp180.readTemperature();
  float pressure_bmp180 = bmp180.readPressure();
  float temperature_bmp280 = bmp280.readTemperature();
  float pressure_bmp280 = bmp280.readPressure();

  float light_level_a;
  if (bh1750_a.measurementReady()) {
    light_level_a = bh1750_a.readLightLevel();
  } else {
    error_counter_1_a++;
  }

  float light_level_b;
  if (bh1750_b.measurementReady()) {
    light_level_b = bh1750_b.readLightLevel();
  } else {
    error_counter_1_b++;
  }

  float light_level_c;
  if (bh1750_c.measurementReady()) {
    light_level_c = bh1750_c.readLightLevel();
  } else {
    error_counter_2_a++;
  }

  WiFiClient client = server.available();
  
if (client) {
  Serial.println("New Client.");
  String currentLine = "";
  String header = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      if (c == '\n') {
        if (currentLine.length() == 0) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          client.println("<html><head><title>Sensor and Motor Data</title></head><body style=\"background-color: #F0F0F0;\">");

          // Ramka z danymi z czujników
          client.print("<div style=\"border: 2px solid #999; padding: 10px; margin-bottom: 10px; display: flex; flex-wrap: wrap; background-color: #E0E0E0;\">");
          client.print("<h1 style=\"flex-basis: 100%;\">Light Sensor Data</h1>");

          client.print("<div style=\"flex-basis: 100%; display: flex; justify-content: space-between;\">");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\">");
          client.print("<h1>Sensor BH1750 Inside</h1>");
          client.print("<p>Light Level: " + String(light_level_a) + " lx</p>");
          client.print("</div>");

          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\">");
          client.print("<h1>Sensor BH1750 Inside</h1>");
          client.print("<p>Light Level: " + String(light_level_b) + " lx</p>");
          client.print("</div>");

          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\">");
          client.print("<h1>Sensor BH1750 outside</h1>");
          client.print("<p>Light Level: " + String(light_level_c) + " lx</p>");
          client.print("</div>");
          client.print("</div>");

          client.print("<div style=\"flex-basis: 100%; display: flex; justify-content: space-between;\">");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\">");
          client.print("<h1>Sensor BMP280</h1>");
          client.print("<p>Temperature: " + String(temperature_bmp280) + " *C</p>");
          client.print("</div>");

          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\">");
          client.print("<h1>Sensor BMP180</h1>");
          client.print("<p>Temperature: " + String(temperature_bmp180) + " *C</p>");
          client.print("</div>");
          client.print("</div>");

          client.print("</div>");
          // Koniec ramki z danymi z czujników

          // Ramka z danymi do sterowania silnikiem
          client.print("<div style=\"border: 2px solid #999; padding: 10px; display: flex; flex-wrap: wrap; background-color: #E0E0E0;\">");
          client.print("<h1 style=\"flex-basis: 100%;\">Roller Blind Control</h1>");

          client.print("<div style=\"flex-basis: 100%; display: flex; justify-content: space-between;\">");
          client.print("<div style=\"flex-basis: 48%; background-color: #D0D0D0;\"><a href=\"/lower\"><button id=\"lowerBtn\" style=\"width: 100%;\">Lower Roller Blind</button></a></div>");
          client.print("<div style=\"flex-basis: 48%; background-color: #D0D0D0;\"><a href=\"/raise\"><button id=\"raiseBtn\" style=\"width: 100%;\">Raise Roller Blind</button></a></div>");
          client.print("</div>");

          client.print("<h1 style=\"flex-basis: 100%;\">Motor Control</h1>");
          client.print("<div style=\"flex-basis: 100%; display: flex; justify-content: space-between;\">");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\"><a href=\"/CW\"><button id=\"CWBtn\" style=\"width: 100%;\">Rotate Clockwise</button></a></div>");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\"><a href=\"/CCW\"><button id=\"CCWBtn\" style=\"width: 100%;\">Rotate Counterclockwise</button></a></div>");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\"><a href=\"/STOP\"><button id=\"stopBtn\" style=\"width: 100%;\">Stop Motor</button></a></div>");
          client.print("</div>");

          client.print("<br><br>");
          client.print("<div style=\"flex-basis: 100%; display: flex; justify-content: space-between;\">");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\"><a href=\"/speed?value=1\"><button id=\"speed1Btn\" style=\"width: 100%;\">Speed 1</button></a></div>");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\"><a href=\"/speed?value=2\"><button id=\"speed2Btn\" style=\"width: 100%;\">Speed 2</button></a></div>");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\"><a href=\"/speed?value=3\"><button id=\"speed3Btn\" style=\"width: 100%;\">Speed 3</button></a></div>");
          client.print("<div style=\"flex-basis: 30%; background-color: #D0D0D0;\"><a href=\"/speed?value=4\"><button id=\"speed4Btn\" style=\"width: 100%;\">Speed 4</button></a></div>");
          client.print("</div>");

          client.print("<br>");
          client.print("<p style=\"flex-basis: 100%;\">Current Motor Speed: " + String(motorSpeed) + "</p>");

          client.print("</div>");
          // Koniec ramki z danymi do sterowania silnikiem
// Koniec ramki z danymi do sterowania silnikiem



  // Dodanie skryptu JavaScript
  client.print("<script>");
  client.print("var buttons = document.getElementsByTagName('button');");
  client.print("for (var i = 0; i < buttons.length; i++) {");
  client.print("  buttons[i].addEventListener('mousedown', function() { this.style.backgroundColor = '#FF0000'; this.style.color = '#FFFFFF'; setTimeout(function() { this.style.backgroundColor = ''; this.style.color = ''; }.bind(this), 1000); });");
  client.print("}");
  client.print("</script>");
  client.print("</div>");
  // Koniec ramki z danymi do sterowania silnikiem


  client.println("</body></html>");
  client.println();
  break;


          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /CW")) {
          digitalWrite(DIR, HIGH);
          motorRunning = true;
          tone(STP, motorSpeed);
        }
        if (currentLine.endsWith("GET /CCW")) {
          digitalWrite(DIR, LOW);
          motorRunning = true;
          tone(STP, motorSpeed);
        }
        if (currentLine.endsWith("GET /STOP")) {
          noTone(STP);
          motorRunning = false;
        }
        if (currentLine.startsWith("GET /speed?value=")) {
          motorSpeed = map(currentLine.charAt(17), '1', '4', 100, MAX_SPEED);
          if (motorRunning) {
            tone(STP, motorSpeed);
          }
        }

        if (currentLine.endsWith("GET /lower")) {
          lowerRollerBlind();
        }
        if (currentLine.endsWith("GET /raise")) {
          raiseRollerBlind();
        }

      }
    }
    if (!motorRunning) {
      noTone(STP);
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
void lowerRollerBlind() {
  digitalWrite(DIR, LOW);
  motorRunning = true;
  tone(STP, motorSpeed);
  delay(20000);  // Opuszcz przez 15 sekund
  noTone(STP);
  motorRunning = false;
}

void raiseRollerBlind() {
  digitalWrite(DIR, HIGH);
  motorRunning = true;
  tone(STP, motorSpeed);
  delay(20000);  // Podnieś przez 15 sekund
  noTone(STP);
  motorRunning = false;
}
