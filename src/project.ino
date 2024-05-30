#define TEMT6000 34 //PIN on TEMT6000

void setup()
{
  // PIN MODE
  pinMode(TEMT6000, INPUT);
  
  Serial.begin(9600);
}

void loop() {

  // Light Reading - TEMT6000
    analogReadResolution(10);
    
    float volts =  analogRead(TEMT6000) * 5 / 1024.0; // Convert reading to VOLTS
    float VoltPercent = analogRead(TEMT6000) / 1024.0 * 100; //Reading to Percent of Voltage
    
    //Conversions from reading to LUX
    float amps = volts / 10000.0;  // em 10,000 Ohms
    float microamps = amps * 1000000; // Convert to Microamps
    float lux = microamps * 2.0; // Convert to Lux */
    delay(1000);

  // Output Serial

  // Output Serial
  Serial.print("LUX - ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial.print(VoltPercent);
  Serial.println("%");
  Serial.print(volts);
  Serial.println(" volts");
  Serial.print(amps);
  Serial.println(" amps");
  Serial.print(microamps);
  Serial.println(" microamps");
  delay(1000);
}