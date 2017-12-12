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

void setup(void) {
    initHBridge(A1, A2, B1, B2);

    pinMode(BP1, OUTPUT);
    pinMode(BP2, OUTPUT);

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

    int start = 0;

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



void loop(void) {
    int difMesure = frontLeftSensor() - frontRightSensor();

    if (difMesure == 0) {
        controlMotor(255, 255);
    } else if (difMesure < 0) {
        controlMotor(255, 255 - difMesure);
    } else if (difMesure > 0) {
        controlMotor(255 - difMesure, 255);
    }
}

void sensorBBInterrupt(void) { // TODO : régler la reposition du robot
    controlMotor(255, 255);
    delay(1000);
    controlMotor(0, 0);
}

void sensorBRInterrupt(void) {
    controlMotor(-255, -255);
    delay(1000);
    controlMotor(-255, 0);
    delay(1000);
    controlMotor(0, 0);
}

void sensorBLInterrupt(void) {
    controlMotor(-255, -255);
    delay(1000);
    controlMotor(0, -255);
    delay(1000);
    controlMotor(0, 0);
}

void changeState(int newState) { // TODO : state of the led in function of the state variable
    switch (newState) {
        case 0:
            digitalWrite(BLUE, HIGH);
            break;
        case 1:
            digitalWrite(GREEN, HIGH);
            break;
        case 2:
            digitalWrite(RED, HIGH);
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
    return map(analogRead(LSENS), 0, 450, 0, 255);
}

int frontRightSensor(void) {
    return map(analogRead(RSENS), 0, 450, 0, 255);
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

void walkLeft(int speed, int dir) { // dir 0 -> Avance TODO : Correction de vitesse
    int _speed = map(speed, 0, 100, 0, 255);
    if (dir == 0) {
        digitalWrite(A1, LOW);
    } else {
        digitalWrite(A1, HIGH);
    }
    analogWrite(A2, _speed);
}

void walkRight(int speed, int dir) {
    int _speed = map(speed, 0, 100, 0, 255);
    if (dir == 0) {
        digitalWrite(B1, LOW);
    } else {
        digitalWrite(B1, HIGH);
    }
    analogWrite(B2, _speed);
}