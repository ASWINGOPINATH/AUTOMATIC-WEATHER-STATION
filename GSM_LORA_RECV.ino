#include <SoftwareSerial.h>                 // Include the SoftwareSerial library for GSM communication
#include <SPI.h>
#include <LoRa.h>
#define GSM_RX 2                            // Define the GSM module RX pin
#define GSM_TX 3                            // Define the GSM module TX pin
#define RELAY_PIN 13                         // Define the pin connected to the relay module
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);   // Create a software serial object for GSM communication

char message[20], smoke;
char c;
int i = 0, flag = 0;
void setup() {
  pinMode(8, INPUT);
  pinMode(7, OUTPUT);
  pinMode(4, OUTPUT);

  Serial.begin(9600);
  gsmSerial.begin(9600);
  // Initialize LCD display

  Serial.print("System ready");
  // Authenticate with the GSM network

  //------------GSM--------------
  //  while (!isGSMReady()) {
  //    delay(1000);
  //  }
  delay(2000);
  Serial.println("GSM Detected");
  // initGSM();

  Serial.println("Initialization");
  Serial.println("GSM complete");

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(454E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

}


void loop()
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet ");

    // read packet
    while (LoRa.available()) {
      smoke = ((char)LoRa.read());
      Serial.println(smoke);
    }
    if (smoke == 'N')
    {
      digitalWrite(7, LOW);
      digitalWrite(4, LOW);
      Serial.println("No smoke and sound");
    }
    if (smoke == 'S')
    {
      digitalWrite(7, HIGH);
      Serial.println("smoke");
    }

    if (smoke == 'F')
    {
      digitalWrite(4, HIGH);
      Serial.println("Sound");
    }



  }

  if (digitalRead(8) == HIGH && flag == 0)
  {
    flag = 1;
    gsmSerial.println("ATD+917012033155;");
    // Message();
  }
  if (digitalRead(8) == LOW && flag == 1)
  {
    flag = 0;


  }

}



void initGSM() {
  gsmSerial.println("AT"); // Check communication with GSM module
  while (gsmSerial.read() != 'K');
  delay(1000);

  gsmSerial.println("AT+CMGF=1"); // Set the GSM module to text mode
  while (gsmSerial.read() != 'K');
  delay(1000);

  gsmSerial.println("AT+CNMI=2,2,0,0,0"); // Configure SMS notifications
  while (gsmSerial.read() != 'K');
  delay(1000);
}
bool isGSMReady() {
  gsmSerial.println("AT");
  delay(500);
  while (gsmSerial.available()) {
    char response = gsmSerial.read();
    if (response == 'O' || response == 'K') {
      return true;
    }
  }
  return false;
}


void Message() {

  digitalWrite(RELAY_PIN, LOW);
  delay(2000);

  // Send confirmation SMS
  gsmSerial.println("AT+CMGF=1");  // Set SMS mode to text
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+917012033155\"");  // Replace with recipient's phone number
  delay(1000);
  gsmSerial.println("Soil Water Content is High");  // Message content
  delay(1000);
  gsmSerial.write(0x1A);  // Send Ctrl+Z to terminate message
  delay(1000);
}
