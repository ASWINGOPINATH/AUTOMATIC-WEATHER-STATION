#include <SPI.h>
#include <LoRa.h>
int sound_threshod = 800;
int threshod = 400;
int smoke = 0, sound = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(454E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  smoke = analogRead(A0);
  sound = analogRead(A1);
  if (smoke >= threshod)
  {
    // send packet
    LoRa.beginPacket();
    LoRa.print("S");
    //LoRa.print(counter);
    LoRa.endPacket();
    Serial.print("Smoke Detected");
  }
  if (sound <= sound_threshod)
  {
    // send packet
    LoRa.beginPacket();
    LoRa.print("F");
    //LoRa.print(counter);
    LoRa.endPacket();
    Serial.print("Sound Detected");
  }
  if (smoke < threshod && sound > sound_threshod)
  {
    // send packet
    LoRa.beginPacket();
    LoRa.print("N");
    //LoRa.print(counter);
    LoRa.endPacket();

  }
  Serial.print("smoke: ");
  Serial.print(smoke);
  Serial.print(" Sound: ");
  Serial.println(sound);
  delay(1000);
}
