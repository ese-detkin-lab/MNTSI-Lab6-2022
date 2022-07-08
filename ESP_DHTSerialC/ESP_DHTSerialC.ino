// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17
#define LedGreen 5

#define BLYNK_TEMPLATE_ID "TMPLYpjMhI6c"
#define BLYNK_DEVICE_NAME "test"
#define BLYNK_AUTH_TOKEN "RSPowgJOJoxo-UopLhVCfcNFneL4VwJ4"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

String curVal;


// you should get Auth Token in the Blynk App.
// go to the Project Settings (nut icon).


// your WiFi credentials.
// set password to "" for open networks.
char ssid[] = "OnePlus";
char pass[] = "12345678";

BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LedGreen, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Blynk.virtualWrite(V1, 0);
}
void loop() {
  if(Serial2.available()){
    curVal = Serial2.readString();

    if(curVal.indexOf("!") != -1){
      Blynk.virtualWrite(V1, curVal.substring(1).toDouble());
      Serial.print("Temp: ");
      Serial.println(curVal.substring(1));
    }
    if(curVal.indexOf("#") != -1){
      Blynk.virtualWrite(V2, curVal.substring(2).toDouble());
      Serial.print("Humid: ");
      Serial.println(curVal.substring(2));
    }
  }
  Blynk.run();
}
