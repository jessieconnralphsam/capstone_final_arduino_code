int relay1 = 9;
int relay2 = 12;

const int triggerType = LOW;

int relayOFF, relayON;

void setup() {
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);

    if(triggerType == LOW){
      relayON = LOW;
      relayOFF = HIGH;
      digitalWrite(relay1, relayOFF);
      digitalWrite(relay2, relayOFF);
    } else {
      relayON = HIGH;
      relayOFF = LOW; 
      digitalWrite(relay1, relayOFF);
      digitalWrite(relay2, relayOFF);
    }
  
  Serial.begin(9600);
  Serial.println("Robojax Motor Direction of Rotation");
  Serial.println("Using 2 Relays");  
  delay(2000);
}

void loop() {
  motorOn(); 
  delay(3000);

  motorStop();
  delay(2000);
  
  Serial.println("===============");
}

void motorOn() {
  digitalWrite(relay1, relayON);
  Serial.println("Motor ON");    
}

void motorStop() {
  digitalWrite(relay1, relayOFF);
  Serial.println("Motor Stopped");    
}
