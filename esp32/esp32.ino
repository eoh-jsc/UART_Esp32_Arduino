#define ERA_LOCATION_VN
// #define ERA_LOCATION_SG

// Obtain the Auth Token from the ERa App or ERa Dashboard
#define ERA_AUTH_TOKEN ""

#include <Arduino.h>
#include <ERa.hpp>

const char ssid[] = "YOUR_SSID";
const char pass[] = "YOUR_PASSWORD";

void setup() {
  ERa.begin(ssid, pass);
  Serial.begin(115200); // Initialize UART0 (default serial port) with a baud rate of 115200
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // Initialize UART2 with a baud rate of 115200, RX2 (GPIO16), TX2 (GPIO17)
  /* Initialize the ERa library */
  /* Set up a timer to call the function every second */
  ERa.addInterval(100L, timerEvent);
}

/* This function is called every time ERa successfully connects */
ERA_CONNECTED() {
    ERA_LOG("ERa", "ERa connected!");
}

/* This function is called every time ERa disconnects */
ERA_DISCONNECTED() {
    ERA_LOG("ERa", "ERa disconnected!");
}

float temperature = 0; // Variable to store temperature value
float humidity = 0;    // Variable to store humidity value

void timerEvent() {
    ERa.virtualWrite(V0, temperature); // Write temperature data to virtual pin V0
    ERa.virtualWrite(V1, humidity);    // Write humidity data to virtual pin V1
    ERA_LOG("Timer", "Uptime: %d seconds", ERaMillis() / 1000L); // Log the uptime
}

void loop() {
  ERa.run();
  delay(1500); // Delay for 1.5 seconds
  if (Serial2.available()) { // Check if data is available from UART2
    String data = Serial2.readString(); // Read the data string from UART2
    Serial.println(data); // Print the data string to UART0 (Serial)
    String value1 = getValue(data, ';', 0); // Extract the first value from the data string (delimited by ';')
    String value2 = getValue(data, ';', 1); // Extract the second value from the data string (delimited by ';')
    Serial.println("Temperature: " + value1); // Print the first value (Temperature)
    Serial.println("Humidity: " + value2);    // Print the second value (Humidity)

    // Convert the temperature string to a float
    temperature = value1.toFloat();
    humidity = value2.toFloat();
  }
}

// Function to split a string based on a delimiter and return the specified index element
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
