#define BLYNK_TEMPLATE_ID "TMPL6Cja08zAC"
#define BLYNK_TEMPLATE_NAME "AC"
#define BLYNK_AUTH_TOKEN "yN4W815O-0agTay5exvztLaVJL2ZHTX0"

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h> //Jika protocol tidak terdeteksi
#include <ir_Panasonic.h> //Protocol Panasonic (lihat library untuk protocol remote lain)
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "PANGKALAN UTAMA";       
char pass[] = "sersanadi";

//Pin IRLed TX
const uint16_t kIrLed = 4; // Menggunakan pin D5 (GPIO14)

int pushMode = 0;
int pushFan = 0;
int pushSwing = 0;

int togglePower = 0;
int toggleMode = 0;
int toggleFan = 0;
int toggleSwing = 0;
int toggleECO = 0;

int temp = 16;

int notifMode, notifFan, notifSwing, notifECO;

// Set the GPIO used for sending messages.
IRPanasonicAc ac(kIrLed);  
IRsend irsend(kIrLed);

WidgetLCD lcd(V7);

//ECO SMART ON
uint16_t rawEcoOn[263] = {3414, 1782,  392, 502,  368, 1372,  368, 502,  368, 476,  392, 476,  392, 476,  392, 502,  368, 478,  392, 504,  366, 502,  366, 502,  366, 502,  368, 502,  366, 1372,  366, 504,  366, 502,  368, 478,  392, 502,  366, 502,  366, 478,  392, 476,  392, 1372,  368, 1348,  392, 1372,  366, 504,  366, 502,  368, 1348,  392, 478,  392, 502,  368, 504,  366, 478,  392, 476,  392, 502,  368, 502,  368, 502,  368, 502,  368, 502,  368, 502,  368, 504,  366, 502,  368, 502,  366, 502,  368, 476,  392, 502,  366, 502,  368, 502,  366, 502,  368, 502,  366, 502,  368, 502,  366, 478,  390, 502,  368, 502,  366, 502,  368, 476,  394, 502,  366, 478,  392, 1348,  392, 1372,  368, 478,  392, 502,  368, 502,  368, 502,  368, 502,  366, 10428,  3440, 1782,  392, 476,  392, 1374,  366, 500,  368, 478,  392, 476,  392, 478,  392, 502,  366, 502,  368, 502,  366, 502,  366, 502,  368, 502,  368, 502,  366, 1372,  366, 502,  368, 502,  366, 476,  394, 502,  368, 476,  392, 504,  366, 502,  368, 1372,  366, 1372,  368, 1372,  368, 502,  368, 502,  366, 1348,  392, 504,  366, 478,  392, 502,  366, 502,  368, 502,  368, 502,  366, 504,  366, 502,  368, 502,  368, 478,  392, 502,  366, 502,  366, 1348,  392, 502,  368, 1346,  392, 1372,  366, 1372,  368, 504,  366, 476,  394, 502,  368, 502,  368, 502,  366, 1372,  366, 478,  392, 502,  366, 1346,  392, 1348,  392, 1348,  392, 1372,  368, 502,  366, 1348,  392, 1346,  394, 502,  366, 502,  368, 502,  366, 478,  392, 1372,  366};
//ECO SMART OFF
uint16_t rawEcoOff[263] = {3386, 1782,  392, 502,  366, 1350,  390, 502,  366, 502,  368, 502,  366, 504,  366, 502,  366, 504,  366, 502,  368, 502,  366, 502,  366, 504,  366, 504,  366, 1372,  366, 478,  392, 502,  366, 504,  366, 478,  392, 478,  392, 502,  366, 504,  366, 1350,  390, 1348,  392, 1348,  392, 504,  366, 504,  366, 1372,  366, 502,  368, 476,  392, 476,  394, 502,  368, 502,  366, 502,  366, 502,  368, 476,  392, 502,  366, 478,  392, 502,  366, 502,  368, 502,  368, 502,  368, 502,  366, 504,  366, 502,  366, 502,  366, 476,  394, 502,  366, 478,  390, 476,  394, 502,  368, 502,  368, 502,  366, 502,  368, 502,  366, 478,  392, 478,  392, 504,  366, 1372,  366, 1372,  366, 504,  366, 502,  366, 502,  366, 502,  366, 476,  392, 10430,  3416, 1784,  366, 502,  366, 1348,  392, 502,  368, 502,  368, 502,  366, 502,  366, 502,  368, 502,  368, 502,  366, 504,  366, 502,  366, 502,  366, 502,  366, 1372,  366, 504,  366, 502,  368, 504,  366, 502,  366, 504,  366, 502,  366, 502,  368, 1372,  366, 1374,  366, 1348,  392, 502,  366, 502,  368, 1372,  368, 502,  368, 502,  368, 502,  366, 502,  366, 502,  368, 504,  366, 502,  368, 502,  366, 504,  366, 504,  366, 502,  366, 502,  368, 1372,  366, 1372,  366, 1372,  366, 504,  366, 1372,  366, 502,  366, 1372,  366, 1372,  368, 502,  366, 504,  366, 502,  366, 502,  366, 1374,  364, 1372,  366, 1372,  366, 1372,  366, 504,  366, 1348,  392, 1346,  392, 502,  368, 502,  366, 502,  368, 502,  366, 1372,  366};
//TIMER_ID timerID;
//uint16_t rawPowerOff[67] = {3334, 1606,  394, 1198,  386, 478,  394, 394,  398, 394,  398, 394,  398, 394,  400, 394,  398, 394,  398, 394,  400, 394,  398, 394,  398, 396,  396, 396,  398, 396,  394, 396,  396, 396,  398, 394,  398, 394,  400, 394,  398, 394,  400, 394,  398, 394,  398, 394,  400, 392,  398, 396,  398, 394,  398, 394,  400, 394,  398, 392,  400, 392,  398, 394,  398, 392,  400, 394,  398, 392,  400};
//uint16_t rawPowerOn[67] = {3334, 1606,  394, 1198,  386, 478,  394, 394,  398, 394,  398, 394,  398, 394,  400, 394,  398, 394,  398, 394,  400, 394,  398, 394,  398, 396,  396, 396,  398, 396,  394, 396,  396, 396,  398, 394,  398, 394,  400, 394,  398, 394,  400, 394,  398, 394,  398, 394,  400, 392,  398, 396,  398, 394,  398, 394,  400, 394,  398, 392,  400, 392,  398, 394,  398, 392,  400, 394,  398, 392,  400};

//Fan mode: 0 = Auto, 1 = Low, 2 = Medium, 3 = High, 4 = Max
int modeFan = 0;
int modeTemp = 25;

// BlynkTimer timer;

BLYNK_WRITE(V5)
{
  int increment = param.asInt();
  if (increment == 1)
  {
    temp += 1; // Menaikkan suhu sebesar 1 derajat
    if (temp > 30) temp = 30; // Batas atas suhu
    ac.setTemp(temp);
    ac.send();
    Serial.print("Temperature increased: "); // Menampilkan suhu yang dinaikkan
    Serial.println(temp);
  }
}

BLYNK_WRITE(V6)
{
  int decrement = param.asInt();
  if (decrement == 1)
  {
    temp -= 1; // Menurunkan suhu sebesar 1 derajat
    if (temp < 16) temp = 16; // Batas bawah suhu
    ac.setTemp(temp);
    ac.send();
    Serial.print("Temperature decreased: "); // Menampilkan suhu yang diturunkan
    Serial.println(temp);
  }
}

BLYNK_WRITE(V8)
{
  temp = param.asInt();
  ac.setTemp(temp);
  ac.send();
  Serial.print("Temperature: "); // Menampilkan temperatur
  Serial.println(temp);
}

BLYNK_WRITE(V2)
{
  pushMode = param.asInt();
  if(pushMode == 1)
  {
    toggleMode++;
    if(toggleMode > 4) toggleMode = 0;

    switch(toggleMode) {
      case 0:
        ac.setMode(kPanasonicAcAuto);
        break;
      case 1:
        ac.setMode(kPanasonicAcCool);
        break;
      case 2:
        ac.setMode(kPanasonicAcDry);
        break;
      case 3:
        ac.setMode(kPanasonicAcFan);
        break;
      case 4:
        ac.setMode(kPanasonicAcHeat);
        break;
    }
    ac.send();
    Serial.print("Mode changed: "); // Menampilkan mode
    Serial.println(toggleMode);
  }
}

BLYNK_WRITE(V3)
{
  pushFan = param.asInt();
  if(pushFan == 1)
  {
    toggleFan++;
    if(toggleFan > 5) toggleFan = 0;

    switch(toggleFan) {
      case 0:
        ac.setFan(kPanasonicAcFanAuto);
        break;
      case 1:
        ac.setFan(kPanasonicAcFanLow);
        break;
      case 2:
        ac.setFan(kPanasonicAcFanMed);
        break;
      case 3:
        ac.setFan(kPanasonicAcFanHigh);
        break;
      case 4:
        ac.setFan(kPanasonicAcFanMax);
        break;
    }
    ac.send();
    Serial.print("Fan Speed: "); // Menampilkan kecepatan kipas
    Serial.println(toggleFan);
  }
}

BLYNK_WRITE(V4)
{
  pushSwing = param.asInt();
  if(pushSwing == 1)
  {
    toggleSwing++;
    if(toggleSwing > 3) toggleSwing = 0;

    // No equivalent functions available in IRPanasonicAc library for swing
    Serial.print("Swing Mode: "); // Menampilkan mode ayunan
    Serial.println(toggleSwing);
  }
}

BLYNK_WRITE(V0)
{
  pushFan = param.asInt();
  if(pushFan == 1)
  {
    togglePower++;
    if(togglePower > 1) togglePower = 0;

    ac.setPower(togglePower == 1);
    ac.send();
    Serial.print("Power: "); // Menampilkan status daya
    Serial.println(togglePower == 1 ? "ON" : "OFF");
  }
}

BLYNK_WRITE(V9)
{
  toggleECO = param.asInt();
  if(toggleECO == 1)
  {
    irsend.sendRaw(rawEcoOn, sizeof(rawEcoOn) / sizeof(rawEcoOn[0]), 38);
    Serial.println("ECO Mode: ON"); // Menampilkan status ECO Mode
  }
  else
  {
    irsend.sendRaw(rawEcoOff, sizeof(rawEcoOff) / sizeof(rawEcoOff[0]), 38);
    Serial.println("ECO Mode: OFF"); // Menampilkan status ECO Mode
  }
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  irsend.begin();
  ac.begin();
  ac.setMode(kPanasonicAcCool);
  ac.setTemp(24);
  ac.setFan(kPanasonicAcFanAuto);
  ac.send();
  Serial.println("Setup complete!"); // Menampilkan bahwa setup selesai
  // // timer.setInterval(1000L, []()
  // {
  //   Blynk.virtualWrite(V1, temp);
  //   Blynk.virtualWrite(V2, toggleMode);
  //   Blynk.virtualWrite(V3, toggleFan);
  //   Blynk.virtualWrite(V4, toggleSwing);
  //   Blynk.virtualWrite(V5, togglePower);
  // });
}

void loop()
{
  Blynk.run();
  // timer.run();
}
