#ifndef INVIO_H
#define INVIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../LIBR/pid/pid.h"
#include "../LIBR/fuzzy/fuzzy.h"

/* Controller parameters */
#define PID_KP  2
#define PID_KI  0.5
#define PID_KD  0.25
#define PID_LIM_MIN -100
#define PID_LIM_MAX  100
#define SAMPLE_TIME 0.01


void invio_tx(float,float, double, UART_HandleTypeDef);
double calcola_parametri(float, float, int, int);


#endif /* INVIO_H */