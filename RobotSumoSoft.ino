#define DEBUG true

//Déclaration des pins

#define A_1 8 // Pont en H
#define A_2 9
#define B_2 10
#define B_1 11

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

#define BACK 0
#define RIGHT 1
#define LEFT 2

void initHBridge(int p1, int p2, int p3, int p4); // Initialisation pins H Bridge

void controlMotor(int __speeda, int __speedb); // speed : -255 to 255

void changeState(int newState);

int frontLeftSensor(void);
int frontRightSensor(void);

void myDelay(int __ms);

int start = 0;

void setup(void) {
    initHBridge(A_1, A_2, B_1, B_2);

    pinMode(BP1, INPUT);
    pinMode(BP2, INPUT);

    pinMode(RED, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(GREEN, OUTPUT);

    pinMode(SENSORBB, INPUT);
    pinMode(SENSORBL, INPUT);
    pinMode(SENSORBR, INPUT);

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
            controlMotor(0, 60);
            if (frontRightSensor() > 150 && frontLeftSensor() > 150) {
                found = 1;
                controlMotor(150, 150);
            }/*else if (frontLeftSensor() > 100) {
                found = 1;
                controlMotor(150, 150);
            }*/
        }
    }

    int difMesure = frontLeftSensor() - frontRightSensor();

    Serial.println(analogRead(SENSORBL));

    difMesure *= 3;

    if (frontLeftSensor() > 400 || frontRightSensor() > 400) {
        controlMotor(255, 255);
    } else if (frontLeftSensor() == 0 && frontRightSensor() == 0) {
        found = 0;
    } else if (analogRead(SENSORBB) > 100) {
        controlMotor(255, 255);
        myDelay(1000);
    } else if (analogRead(SENSORBL) > 100) {
        controlMotor(-255, -255);
        myDelay(1000);
    } else if (analogRead(SENSORBR) > 100) {
        controlMotor(-255, -255);
        myDelay(1000);
    } else {
        if (difMesure < 20 && difMesure > -20) {
            controlMotor(150, 150);
        } else if (difMesure > 0) {
            controlMotor(150, 150 - difMesure);
        } else if (difMesure < 0) {
            difMesure = (int)sqrt(pow(difMesure, 2));
            controlMotor(150 - difMesure, 150);
        }
    }

    delay(100);
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
    if (m < 20)
        return 0;
    return m;
}

int frontRightSensor(void) {
    int m = analogRead(RSENS);
    if (m < 20)
        return 0;
    return m;
}

void controlMotor(int __speeda, int __speedb) {
    if (__speeda < 0) {
        __speeda = 255 - __speeda;
        digitalWrite(A_1, HIGH);
    } else {
        digitalWrite(A_1, LOW);
    }

    analogWrite(A_2, __speeda);

    if (__speedb < 0) {
        __speedb = 255 - __speedb;
        digitalWrite(B_1, HIGH);
    } else {
        digitalWrite(B_1, LOW);
    }

    analogWrite(B_2, __speedb);

}

void myDelay(int __ms) {
    for (int i = 0; i < __ms; ++i) {
        delayMicroseconds(1000);
    }
}
