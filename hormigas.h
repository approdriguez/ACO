#ifndef hormiga
#define hormiga
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "random_ppio.c"
#include <queue>

using namespace std;
//const int tam_cadena_ozono = 72;
//const int num_cadenas_ozono = 185;
//const int tam_cadena_sonar = 60;
//const int num_cadenas_sonar = 208;
//const int tam_cadena_spam = 57;
//const int num_cadenas_spam = 4601;


//Datos para la tabla ozono
//const int tam_cadena_tabla = 72;
//const int num_cadenas_tabla = 185;
//const char * path = "./tablas/ozone.arff";
//Datos para la tabla sonar
const int tam_cadena_tabla = 60;
const int num_cadenas_tabla = 208;
const char * path = "./tablas/sonar.arff";
//Datos para la tabla spam
//const int tam_cadena_tabla = 57;
//const int num_cadenas_tabla = 460;
//const char * path = "./tablas/spambase.arff";

//const char path = "./tablas/spambase.arff";
//const char path = "./tablas/spambase.arff";
//float distancias_ozono[num_cadenas_ozono][num_cadenas_ozono];
//float datos_ozono[num_cadenas_ozono][tam_cadena_ozono];
//int clases_ozono[num_cadenas_ozono];
//float distancias_sonar[num_cadenas_sonar][num_cadenas_sonar];
//float datos_sonar[num_cadenas_sonar][tam_cadena_sonar];
//int clases_sonar[num_cadenas_sonar];
//float distancias_spam[num_cadenas_spam][num_cadenas_spam];
//float datos_spam[num_cadenas_spam][tam_cadena_spam];
//int clases_spam[num_cadenas_spam];

float distancias_tabla[num_cadenas_tabla][num_cadenas_tabla];
float datos_tabla[num_cadenas_tabla][tam_cadena_tabla];
int clases_tabla[num_cadenas_tabla];

typedef bool Caracteristica;
typedef Caracteristica *CadenaBinaria;

typedef struct {
	CadenaBinaria s;
	float tasa_clas;
	float tasa_red;
    int tam;
} STRUCT_CARACTERISTICAS;





STRUCT_CARACTERISTICAS sol;
STRUCT_CARACTERISTICAS mejorSol;
float feromonasCaracteristicas[tam_cadena_tabla];
float feromonasNoCaracteristicas[tam_cadena_tabla];

/*
class AntFarm{
public:
	//inicializamos cada hormiga 
	AntFarm(int nhormiga, Random* r, STRUCT_CARACTERISTICAS sol); 
	void actualizarFeomona();
	void evaluar();
	void localsearch();




private:
	float feromonasCaracteristicas[sol.tam_cadena_tabla];
	float feromonasNoCaracteristicas[sol.tam_cadena_tabla];
	STRUCT_CARACTERISTICAS sol;
	int nhormiga;
	Random* r_number;


*/
























#endif
