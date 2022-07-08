// Make sure these are installed via library manager
#include <ArduinoJson.h>
#include <Adafruit_LiquidCrystal.h>

// These should be included
#include <WiFi.h>
#include "Wire.h"
#include <time.h>

const char* ssid = "HP Setup";    //  your network SSID (name)
const char* passw = "detkin101";  //  your network PASSWORD ()

//open weather map api key
// You need to sign up and get your own key
String apiKey = "";

//the city you want the weather for
String location = "19104,US";

int status = WL_IDLE_STATUS;
char server[] = "api.openweathermap.org";

WiFiClient client;

// TODO: Connect manually (individual wires) on Arduino; Send data as a block over serial
// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

StaticJsonDocument<128> filter;

JsonObject filter_list_0 = filter["list"].createNestedObject();


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  // set up the LCD's number of rows and columns:
  Serial2.begin(9600);

//Setup JSON Filter
  filter_list_0["dt"] = true;
  filter_list_0["main"]["temp"] = true;
  filter_list_0["weather"][0]["description"] = true;
  filter["city"]["name"] = true;

  lcd.begin(16, 2);
// Print a message to the LCD.

  lcd.setBacklight(HIGH);

  WiFi.begin(ssid, passw);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  getWeather();
  delay(10000);
}


void getWeather() {

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET /data/2.5/forecast?");
    client.print("q=" + location);
    client.print("&appid=" + apiKey);
    client.print("&cnt=1");
    client.println("&units=imperial");
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("unable to connect");
  }

  delay(1000);
  String line = "";

 while (client.connected()) {
    
    StaticJsonDocument<256> docum;
    DeserializationError error = deserializeJson(docum, client, DeserializationOption::Filter(filter));
    JsonObject list_0 = docum["list"][0];
    time_t dt = list_0["dt"];
    float temperature = list_0["main"]["temp"]; // 74.91
    lcd.setCursor(0, 0);
    lcd.print(temperature);
    lcd.print((char) 0xDF);
    lcd.print("F ");
    const char* weather_desc = list_0["weather"][0]["description"]; // "light rain"

    Serial.print("Temperature: ");
    Serial.println(temperature);
    
    lcd.setCursor(0,1);
    lcd.print(weather_desc);
    lcd.print("                 ");

    Serial.print("Weather: ");
    Serial.println(weather_desc);

  }
}
