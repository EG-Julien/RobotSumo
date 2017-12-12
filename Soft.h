//
// Created by Akyno on 12/12/2017.
//

#ifndef ROBOTSUMO_SOFT_H
#define ROBOTSUMO_SOFT_H

#endif //ROBOTSUMO_SOFT_H

#include <arduino.h>

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