#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

#define ONE_WIRE_BUS 9
#define POWER_PIN  8
#define SIGNAL_PIN A1
#define PH_SENSOR_PIN A3

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
SoftwareSerial mySerial(5, 6);
LiquidCrystal_I2C lcd(0x27, 20, 4);

int value = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);

  lcd.init();
  lcd.backlight();

  sensors.begin();

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1");
  updateSerial();
}

void loop() {
  sensors.requestTemperatures();
  float waterTemperature = sensors.getTempCByIndex(0);

  digitalWrite(POWER_PIN, HIGH);
  delay(10);
  value = analogRead(SIGNAL_PIN);
  digitalWrite(POWER_PIN, LOW);

  float acidity = measureAcidity(); 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water Temp: ");
  lcd.print(waterTemperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Acidity (pH): ");
  lcd.print(acidity);

  Serial.print("Water Temperature: ");
  Serial.print(waterTemperature);
  Serial.print(" °C - Water Level: ");
  Serial.println(value);

  Serial.print("Acidity (pH): ");
  Serial.println(acidity);

  // Check if any condition is true, and send SMS
  if (value == 0 || waterTemperature > 25 || acidity > 7) {
    sendSMS(value, waterTemperature, acidity);
  }

  delay(5000);
}

float measureAcidity() {
  int phTot = 0;
  for (int x = 0; x < 10; x++) {
    phTot += analogRead(PH_SENSOR_PIN);
    delay(10);
  }
  float phAvg = phTot / 10.0;
  float phVoltage = phAvg * (5.0 / 1023.0);
  float pHValue = phVoltage * -5.70 + 21.34; 
  return pHValue;
}

void sendSMS(int waterLevel, float temperature, float acidity) {
  mySerial.println("AT+CMGS=\"+639503379211\"");
  updateSerial();

  Serial.print("Water Level: ");
  Serial.println(waterLevel);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Acidity (pH): ");
  Serial.println(acidity);

  mySerial.print("Critical condition - Water Level: ");
  mySerial.print(waterLevel);
  mySerial.print(", Temperature: ");
  mySerial.print(temperature);
  mySerial.print(", Acidity (pH): ");
  mySerial.print(acidity);
  mySerial.write(26);
  updateSerial();

  delay(1000);
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
