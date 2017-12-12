//
// Created by Guillaume FRANCOLON on 12/12/2017.
//
#include <Soft.c>

void debug_ESS02(){
    /* Essai d'autonomie
     *
     * Simple programme d'avance du robot
     *
     */
    walkLeft(100, 1);
    walkRight(100, 1);
}

void debug_ESS03() {
        /* Essai des roues motorisées et indépendantes
         * Entraîne le processus consistant à :
         * 1) Tous les moteurs en avance
         * 2) Tous les moteurs en recul
         * 3) Rotation horaire
         * 4) Rotation antihoraire
         * 5) Avance du seul moteur gauche
         * 6) Avance du seul moteur droit
         *
         * Pause de 2 secondes entre chaque phase
         */

    // Phase 1
    walkLeft(100, 1);
    walkRight(100, 1);
    delay(2000);

    // Phase 2
    walkLeft(100, 0);
    walkRight(100, 0);
    delay(2000);

    // Phase 3
    walkLeft(100, 1);
    walkRight(100, 0);
    delay(2000);

    // Phase 4
    walkLeft(100, 0);
    walkRight(100, 1);
    delay(2000);

    // Phase 5
    walkLeft(100, 1);
    delay(2000);

    // Phase 6
    walkRight(100, 1);
    delay(2000);
};

void debug_ESS04(){

}

void debug_ESS05(){

}

void debug_ESS06(){

}

void debug_ESS07(){

}