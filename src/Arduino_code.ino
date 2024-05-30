void setup() {
  Serial.begin(9600);
  
  while (!Serial) {
    ; // Oczekuj na dostępność portu szeregowego
  }
}

void loop() {
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n'); // Odczytaj wiadomość do znaku nowej linii
    Serial.print("Received from ESP32: ");
    Serial.println(receivedMessage);
    
  }

  String messageToESP32 = "Hello from Arduino!";

  Serial.print("Sending to ESP32: ");
  Serial.println(messageToESP32);

  Serial.println();
  delay(2000);
}
