#include <DHTesp.h> // Library for the DHT11 sensor
#include <SoftwareSerial.h> // Library for SoftwareSerial communication

#define DHTPIN 9 // Pin connected to the DHT sensor
#define RX_PIN 10 // RX pin for SoftwareSerial communication
#define TX_PIN 11 // TX pin for SoftwareSerial communication

DHTesp dht; // Create an object for the DHT sensor
SoftwareSerial mySerial(RX_PIN, TX_PIN); // Create a SoftwareSerial object with defined RX and TX pins

void setup() {
  Serial.begin(115200); // Initialize the hardware serial port with a baud rate of 115200
  mySerial.begin(115200); // Initialize the software serial port with a baud rate of 115200
  dht.setup(DHTPIN, DHTesp::DHT11); // Initialize the DHT sensor
}

void loop() {
  delay(2000); // Wait for 2 seconds
  TempAndHumidity th = dht.getTempAndHumidity(); // Read humidity and temperature from the sensor

  if (dht.getStatus() != DHTesp::ERROR_NONE) { // Check if there's an error with the sensor reading
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Prepare data in the format "temperature;humidity" with values separated by ';'
  String Data = String(th.temperature, 1) + ";" + String(th.humidity, 1);
  mySerial.println(Data); // Send temperature and humidity values through SoftwareSerial
  Serial.println(Data); // Print the sent data to the Serial Monitor
}
