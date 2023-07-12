/*
 * fuzzy.h
 *
 *  Created on: Jun 20, 2023
 *      Author: Utente
 */

#ifndef FUZZY_FUZZY_H_
#define FUZZY_FUZZY_H_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Definizioni delle etichette linguistiche
#define N_TERMS 3
enum { LOW, MEDIUM, HIGH };

// Struttura di un termine linguistico
typedef struct {
  char *name;
  float a, b, c;
} FuzzyTerm;

// Struttura di una variabile fuzzy
typedef struct {
  char *name;
  FuzzyTerm terms[N_TERMS];
} FuzzyVariable;

// Struttura di una regola fuzzy
typedef struct {
  int input1, input2;
  int output;
} FuzzyRule;


//prototipi
float triangular_membership(float x, FuzzyTerm term);
float fuzzy_inference(float input1, float input2, FuzzyVariable var1, FuzzyVariable var2, FuzzyVariable outputVar, FuzzyRule rules[], int nRules);
int regulate_servomotor(float temperature, float humidity);
#endif /* FUZZY_FUZZY_H_ */
