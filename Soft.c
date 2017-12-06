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

// Variables globales

int start = 0;
int state = 0;
int found = 0;

// Entete des fonctions

void initHBridge(int p1, int p2, int p3, int p4); // Initialisation pins H Bridge
void walkRight(int speed, int dir); // speed 0 - 100 et dir 0 - 1
void walkLeft(int speed, int dir); // Idem

void changeState(int newState);
void checkBTRY(void);

void sensorBLInterrupt(void);
void sensorBRInterrupt(void);
void sensorBBInterrupt(void);

void standBy(void);
void scan(void);
void track(void);
void avoidVoid(void);

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

    standBy();
    scan();
}



void loop(void) {

}

void track(void) {

}

void scan(void) {

    changeState(1);

    if (start == 1) {
        walkLeft(100, 0);
        walkRight(100, 1);
        delay(250);
        walkLeft(100, 1);
        walkRight(100, 1);
        delay(1000);
    } else if (start == 0) {
        walkLeft(100, 1);
        walkRight(100, 0);
        delay(250);
        walkLeft(100, 1);
        walkRight(100, 1);
        delay(1000);
    }

    found = 0;
    int doOnce = 0;

    do {
        int __right = analogRead(RSENS);
        int __left  = analogRead(LSENS);

        if (__right > 100 && __left > 100 && doOnce == 0) {
            doOnce = 1;
            found = 1;
        } else if (doOnce == 1) {

        }


        delay(10);
    } while (found == 0);

}

void sensorBBInterrupt(void) {
    walkLeft(100, 0);
    walkRight(100, 0);
    delay(1000);
}

void sensorBRInterrupt(void) {
    walkLeft(100, 1);
    walkRight(100, 1);
    delay(500);
    walkLeft(100, 1);
    delay(250);
}

void sensorBLInterrupt(void) {
    walkLeft(100, 1);
    walkRight(100, 1);
    delay(500);
    walkRight(100, 1);
    delay(250);
}

void checkBTRY(void) {
    int __btry = analogRead(BTRY);
    double __btryTension = (__btry * 5.00) / 1023.00;

    if (__btryTension < 6.8) {
        changeState(9);
    }
}

void standBy(void) {
    changeState(0);
    do {
        if (digitalRead(BP1) == 0)
            start = 1;
        if (digitalRead(BP2) == 0)
            start = 2;
    } while (start == 0);
}

void changeState(int newState) { // TODO : state of the led in function of the state variable
    switch (newState) {
        case 0:
            digitalWrite(BLUE, HIGH);
            break;
        case 1:
            digitalWrite(GREEN, HIGH);
            break;
        case 9:
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
    return analogRead(LSENS);
}

int frontRightSensor(void) {
    return analogRead(RSENS);
}

void walkLeft(int speed, int dir) { // dir 0 -> Avance
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