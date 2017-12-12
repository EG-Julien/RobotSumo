#include <Arduino.h>
#include <PinChangeInterrupt.h>

#define DEBUG true

//Déclaration des pins

#define A1 8 // Pont en H
#define A2 9
#define B2 10
#define B1 11

#define BP1 4 // Bouton de démarrage
#define BP2 3

#define SENSORBR A3 // Capteur de ligne
#define SENSORBL A4
#define SENSORBB A5

#define GREEN 6 // Led d'état
#define BLUE  5
#define RED   7

#define RSENS A1
#define LSENS A2

#define BTRY A0

int side = 0;

void initHBridge(int p1, int p2, int p3, int p4); // Initialisation pins H Bridge

void controlMotor(int __speeda, int __speedb); // speed : -255 to 255

void changeState(int newState);

void sensorBLInterrupt(void);
void sensorBRInterrupt(void);
void sensorBBInterrupt(void);

int frontLeftSensor(void);
int frontRightSensor(void);

int start = 0;

void setup(void) {
    initHBridge(A1, A2, B1, B2);

    pinMode(BP1, INPUT);
    pinMode(BP2, INPUT);

    pinMode(RED, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(GREEN, OUTPUT);

    pinMode(SENSORBB, INPUT);
    pinMode(SENSORBL, INPUT);
    pinMode(SENSORBR, INPUT);

    attachPCINT(digitalPinToPinChangeInterrupt(SENSORBB), sensorBBInterrupt, RISING);
    attachPCINT(digitalPinToPinChangeInterrupt(SENSORBR), sensorBRInterrupt, RISING);
    attachPCINT(digitalPinToPinChangeInterrupt(SENSORBL), sensorBLInterrupt, RISING);

    if (DEBUG)
        Serial.begin(115200);

    changeState(0);

    do {
        if (digitalRead(BP1) == 0 || digitalRead(BP2) == 0) {
            start = 1;
            changeState(1);
        }
    } while (start != 1);

    delay(5000);

    changeState(2);
}

int found = 0;

void loop(void) {

    if (found == 0) {
        while (found != 1) {
            controlMotor(0, 50);
            if (frontRightSensor() > 80) {
                found = 1;
                controlMotor(150, 150);
            } else if (frontLeftSensor() > 80) {
                found = 1;
                controlMotor(150, 150);
            }
        }
    }

    int difMesure = frontLeftSensor() - frontRightSensor();
    
    Serial.print("Left : ");
    Serial.println(frontLeftSensor());
    Serial.print("Right : ");
    Serial.println(frontRightSensor());
    
    if (frontLeftSensor() > 300 || frontRightSensor() > 300) {
      controlMotor(255, 255);
    } else {
      difMesure *= 3;

      if (difMesure == 0) {
          controlMotor(150, 150);
      } else if (difMesure > 0) {
          controlMotor(150, 150 - difMesure);
      } else if (difMesure < 0) {
        difMesure = (int)sqrt(pow(difMesure, 2));
        controlMotor(150 - difMesure, 150);
      }
    }
    delay(50);
}

void sensorBBInterrupt(void) { // TODO : régler la reposition du robot
    Serial.println("BB");
    if (!start)
        return;
    controlMotor(255, 255);
}

void sensorBRInterrupt(void) {
    Serial.println("BR");
    if (!start)
        return;
    controlMotor(-255, -255);
}

void sensorBLInterrupt(void) {
    Serial.println("BL");
    if (!start)
        return;
    controlMotor(-255, -255);
}

void changeState(int newState) { // TODO : state of the led in function of the state variable
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, HIGH);
    switch (newState) {
        case 0:
            digitalWrite(BLUE, LOW);
            break;
        case 1:
            digitalWrite(GREEN, LOW);
            break;
        case 2:
            digitalWrite(RED, LOW);
            break;
    }
}

void initHBridge(int p1, int p2, int p3, int p4) {
    pinMode(p1, OUTPUT);
    pinMode(p2, OUTPUT);
    pinMode(p3, OUTPUT);
    pinMode(p4, OUTPUT);
}

int frontLeftSensor(void) {
    int m = analogRead(LSENS);
    if (m < 40)
        return 0;
    return m;
}

int frontRightSensor(void) {
    int m = analogRead(RSENS);
    if (m < 40)
        return 0;
    return m;
}

void controlMotor(int __speeda, int __speedb) {
    if (__speeda < 0) {
        __speeda = sqrt(pow(__speeda, 2));
        digitalWrite(A1, HIGH);
    } else {
        digitalWrite(A1, LOW);
    }

    analogWrite(A2, __speeda);

    if (__speedb < 0) {
        __speedb = sqrt(pow(__speedb, 2));
        digitalWrite(B1, HIGH);
    } else {
        digitalWrite(B1, LOW);
    }

    analogWrite(B2, __speedb);

}
