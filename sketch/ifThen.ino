#include <Arduino.h>
#include <Wire.h>
#include <SmeSFX.h>
#include <HTS221.h>

// Toggle for USBSerial Debug
#define DEBUG 1

// Configure your target values
#define TEMP 20 // Degree Celsius
#define HUM 20  // Percentage

struct Sensor {
  float data;
  bool valid;
};

struct Sensor checkTemp(){
  bool valid = false;
  float temp = smeHumidity.readTemperature();

  if(DEBUG){
  SerialUSB.print("Temperature: ");
  SerialUSB.print(temp);
  SerialUSB.print(" C");
  SerialUSB.print(" | Target: ");
  SerialUSB.print(TEMP);
  SerialUSB.println(" C");
  }

  if(temp > TEMP){ // Logic for Send/No Send Temperature
    valid = true;
  }

  Sensor tempData = { temp, valid };
  return tempData;
}

struct Sensor checkHum(){
  bool valid = false;
  float hum = smeHumidity.readHumidity();

  if(DEBUG){
  SerialUSB.print("Humidity   : ");
  SerialUSB.print(hum);
  SerialUSB.print(" %");
  SerialUSB.print(" | Target: ");
  SerialUSB.print(HUM);
  SerialUSB.println(" %");
  }

  if(hum > HUM){ // Logic for Send/No Send Humidity
    valid = true;
  }

  Sensor humData = { hum, valid };
  return humData;
}

int sendData(float temp, float hum, bool valid){

  if(valid == false){
    ledRedLight(HIGH);
    return 0;
  }

  char payload[4];
  sprintf(payload, "%d%d", (int8_t)hum, (int8_t)temp); // Converts floats to ints for SME Sigfox Lib

  if(DEBUG){
  SerialUSB.println("Sending data over Sigfox...");
  }

  ledGreenLight(HIGH);
  sfxAntenna.sfxSendData(payload, strlen((char*)payload));

  bool answerReady = sfxAntenna.hasSfxAnswer();

  if (answerReady) {
    if (sfxAntenna.getSfxMode() == sfxDataMode) {
      switch (sfxAntenna.sfxDataAcknoledge()) {
        case SFX_DATA_ACK_START:
          if(DEBUG){
          SerialUSB.println("Message is being transmitted...");
          }
          break;

        case SFX_DATA_ACK_PROCESSING:
          if(DEBUG){
          SerialUSB.print('.');
          }
          break;

        case SFX_DATA_ACK_OK:
          ledGreenLight(LOW);
          if(DEBUG){
          SerialUSB.println(' ');
          SerialUSB.println("Send Complete!");
          }
          break;

        case SFX_DATA_ACK_KO:
          ledGreenLight(LOW);
          ledRedLight(HIGH);
          if(DEBUG){
          SerialUSB.println(' ');
          SerialUSB.println("Error: message couldn't be sent.");
          }
          break;
      }
    }
  }

  return 1;
}

void setup() {
  if(DEBUG) {
    while(!SerialUSB);
  }

  if(DEBUG) {
  SerialUSB.begin(9600);
  }

  Wire.begin();
  smeHumidity.begin();

  /* ------- SIGFOX INIT - BEGIN ------- */
  sfxAntenna.begin();
  int initFinish=1;

  if(DEBUG){
  SerialUSB.println("SFX in Command mode");
  }
  sfxAntenna.setSfxConfigurationMode(); // Enter into configuration Mode

  do {
    uint8_t answerReady = sfxAntenna.hasSfxAnswer();
    if (answerReady){
      switch (initFinish){
      case 1:
        if(DEBUG){
        SerialUSB.println("SFX in Data mode");
        }
        sfxAntenna.setSfxDataMode();
        initFinish++;
        break;

      case 2:
        initFinish++; // exit
        break;
      }
    }
  }
  while (initFinish!=3);
  /* ------- SIGFOX INIT - END ------- */
  ledGreenLight(LOW);
}

void loop() {
  bool valid = false;
  int success;

  if(DEBUG){
  SerialUSB.println("Checking temp and humidity...");
  }

  // Load structs with Temp/Humidity
  struct Sensor t = checkTemp();
  struct Sensor h = checkHum();

  if(t.valid && h.valid){
    valid = true;
    ledGreenLight(HIGH);
  }

  success = sendData(t.data,h.data,valid);

  if(DEBUG){
    if(success){
      SerialUSB.println("Successful!");
      SerialUSB.println(" ");
    }
    else{
      SerialUSB.println("Target not met...");
      SerialUSB.println(" ");
    }
  }

  delay(10000); // Should be set to 10 minute interval to meet ETSI requirement for Sigfox Duty Cycle
}
