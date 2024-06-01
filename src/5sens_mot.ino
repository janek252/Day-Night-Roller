#include <WiFi.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

#define DIR 12
#define STP 13

Adafruit_BMP085 bmp180;
Adafruit_BMP280 bmp280_1;
Adafruit_BMP280 bmp280_2;

BH1750 bh1750_a;
BH1750 bh1750_b;

const char* ssid = "FunBox2-3AC1";
const char* password = "mietek16";
WiFiServer server(80);

const int MAX_SPEED = 750;  // Maksymalna prędkość obrotów silnika
int motorSpeed = 0;
float volts, VoltPercent, amps, microamps, lux, alsValue;

BH1750 lightMeter;
Adafruit_BMP085 bmp;

bool motorRunning = false;
bool stopMotor = false;

TwoWire wire1 = TwoWire(1); // Create a new instance of the Wire library for Wire1

void setup() {
  Serial.begin(115200);

  Wire.begin();
  wire1.begin(18, 19); // Initialize Wire1 on ports 18 and 19
  lightMeter.begin();

  if (!bmp180.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1) {}
  }

  if (!bmp280_1.begin()) {
    Serial.println("Could not find a valid BMP280 sensor on default bus, check wiring!");
    while (1) {}
  }

  if (!bmp280_2.begin(0x76, &wire1)) { // Initialize the second BMP280 sensor with the specified I2C address and Wire1 instance
    Serial.println("Could not find a valid BMP280 sensor on Wire1 bus, check wiring!");
    while (1) {}
  }

  bh1750_a.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire);
  bh1750_b.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C, &Wire);

  pinMode(DIR, OUTPUT);
  pinMode(STP, OUTPUT);
  digitalWrite(DIR, LOW);
  delay(10);

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

int error_counter_bmp180 = 0;
int error_counter_bmp280_1 = 0;
int error_counter_bmp280_2 = 0;
int error_counter_1_a = 0;
int error_counter_2_a = 0;
int error_counter_1_b = 0;
int error_counter_2_b = 0;

void loop() {
  float temperature_bmp180 = bmp180.readTemperature();
  float pressure_bmp180 = bmp180.readPressure();
  float temperature_bmp280_1 = bmp280_1.readTemperature();
  float pressure_bmp280_1 = bmp280_1.readPressure();
  float temperature_bmp280_2 = bmp280_2.readTemperature();
  float pressure_bmp280_2 = bmp280_2.readPressure();


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

            client.println("<html><head><title>Sensor Data</title></head><body>");
            client.print("<h1>Light Sensor Data</h1>");
  
            client.print("<h1>BH1750 Data</h1>");
            client.print("<p>Light Level: " + String(light_level_a) + " lx</p>");
            client.print("<h1>BMP085 Data</h1>");


            client.print("<h1>BH1750 Data</h1>");
            client.print("<p>Light Level: " + String(light_level_b) + " lx</p>");
            client.print("<h1>BMP085 Data</h1>");

            client.print("<h1>Temperature/pressure sensors</h1>");
            client.print("<p>Temperature: " + String(temperature_bmp280_1) + " *C</p>");
            client.print("<p>Pressure: " + String(pressure_bmp280_1) + " Pa</p>");
         

            client.print("<p>Temperature: " + String(temperature_bmp180) + " *C</p>");
            client.print("<p>Pressure: " + String(pressure_bmp180) + " Pa</p>");



            client.print("<p>Temperature: " + String(temperature_bmp280_2) + " *C</p>");
            client.print("<p>Pressure: " + String(pressure_bmp280_2) + " Pa</p>");

    
            client.print("Motor Control: ");
            client.print("<a href=\"/CW\"><button>Rotate Clockwise</button></a> ");
            client.print("<a href=\"/CCW\"><button>Rotate Counterclockwise</button></a> ");
            client.print("<a href=\"/STOP\"><button>Stop Motor</button></a>");
            client.println("<br><br>");
            client.print("Motor Speed Control: ");
            client.print("<a href=\"/speed?value=1\"><button>Speed 1</button></a> ");
            client.print("<a href=\"/speed?value=2\"><button>Speed 2</button></a> ");
            client.print("<a href=\"/speed?value=3\"><button>Speed 3</button></a> ");
            client.print("<a href=\"/speed?value=4\"><button>Speed 4</button></a>");
            client.println("<br>");
            client.print("<p>Current Motor Speed: " + String(motorSpeed) + "</p>");
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
      }
    }
    if (!motorRunning) {
      noTone(STP);
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

