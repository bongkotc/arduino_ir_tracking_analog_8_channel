#include <avr/wdt.h>

#define LED_PIN 13
#define DRIVE_LED_PIN 12
#define CUT_HIGH_ADJ_PIN  A15
int aInPin[8] = {0};
int dOutPin[8] = {0};
int dOutData[8] = {0};
int cutToHigh = 512;//Init
char buf[32] = {0};
int bufSize = 0;

int aToDHighLow(int aInPin);
void writeDigOut(int dOutPin, int value);
void readCutHighAdj();
void readDigOut();
int i = 0;

int prevState = 0;
int newState = 0;

void setup() {
  wdt_enable(WDTO_8S);
  
  aInPin[0] = A0;
  aInPin[1] = A1;
  aInPin[2] = A2;
  aInPin[3] = A3;
  aInPin[4] = A4;
  aInPin[5] = A5;
  aInPin[6] = A6;
  aInPin[7] = A7;

  dOutPin[0] = 2;
  dOutPin[1] = 3;
  dOutPin[2] = 4;
  dOutPin[3] = 5;
  dOutPin[4] = 6;
  dOutPin[5] = 7;
  dOutPin[6] = 8;
  dOutPin[7] = 9;

  for (i = 0; i < 8; i++) {
    pinMode(dOutPin[i], OUTPUT);
  }
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(DRIVE_LED_PIN, OUTPUT);
  digitalWrite(DRIVE_LED_PIN, LOW);

}

int aToDHighLow(int aInPin) {
  int aVal = 0;
  aVal = analogRead(aInPin);
  if (aVal >= cutToHigh) {
    return 1; //High
  } else {
    return 0; //Low
  }
}

void writeDigOut(int dOutPin, int value) {
  digitalWrite(dOutPin, value);
}

void readDigOut() {
  int j = 0;
  for (j = 0; j < 8; j++) {
    dOutData[j] = digitalRead(dOutPin[j]);
  }
}

void readCutHighAdj() {
  static int prevCut = 0;
  cutToHigh = analogRead(CUT_HIGH_ADJ_PIN);

//  if (prevCut != cutToHigh) {
//    Serial.print("Cut High Chanage: ");
//    Serial.print(cutToHigh);
//    Serial.println();
//    prevCut = cutToHigh;
//  }
}

long prevPrintA = 0;
void loop() {
  wdt_reset();
  readCutHighAdj();

  for (i = 0; i < 8; i++) {
    newState = 0;
    dOutData[i] = aToDHighLow(aInPin[i]);
    writeDigOut(dOutPin[i], dOutData[i]);
    newState += dOutData[i];
  }

//  if(millis()-prevPrintA>100){
//    Serial.println(analogRead(aInPin[0]));
//    prevPrintA = millis();
//  }
  readDigOut();

  if (millis()-prevPrintA>100) {
    memset(buf,0,sizeof(buf));
    sprintf(buf,"%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
    dOutData[0],dOutData[1],dOutData[2],dOutData[3],
    dOutData[4],dOutData[5],dOutData[6],dOutData[7],cutToHigh);
    bufSize = strlen(buf);
    Serial.write(buf,bufSize
    );
//    digitalWrite(LED_PIN, HIGH);
//    Serial.print(dOutData[0]);
//    Serial.print(",");
//    Serial.print(dOutData[1]);
//    Serial.print(",");
//    Serial.print(dOutData[2]);
//    Serial.print(",");
//    Serial.print(dOutData[3]);
//    Serial.print(",");
//    Serial.print(dOutData[4]);
//    Serial.print(",");
//    Serial.print(dOutData[5]);
//    Serial.print(",");
//    Serial.print(dOutData[6]);
//    Serial.print(",");
//    Serial.print(dOutData[7]);
//    Serial.print(",");
//    Serial.print(cutToHigh);
//    Serial.println();
    prevPrintA = millis();
  } else {
    digitalWrite(LED_PIN, LOW);
  }

//  if (newState != prevState) {
//    digitalWrite(LED_PIN, HIGH);
//    Serial.print(dOutData[0]);
//    Serial.print(",");
//    Serial.print(dOutData[1]);
//    Serial.print(",");
//    Serial.print(dOutData[2]);
//    Serial.print(",");
//    Serial.print(dOutData[3]);
//    Serial.print(",");
//    Serial.print(dOutData[4]);
//    Serial.print(",");
//    Serial.print(dOutData[5]);
//    Serial.print(",");
//    Serial.print(dOutData[6]);
//    Serial.print(",");
//    Serial.print(dOutData[7]);
//    Serial.println();
//    prevState = newState;
//  } else {
//    digitalWrite(LED_PIN, LOW);
//  }
}
