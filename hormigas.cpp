#include "hormigas.h"
#include <iostream>
//#include "random_ppio.c"

#define M_LOG2E 1.44269504088896340736 //log2(e)

void leerTabla(){
    string linea;
    bool leyendo_datos=false;

    int i=0,j=0,aux=0,ind=0;
    float *maximos, *minimos;
    ifstream fich_tabla(path,fstream::in);
    if(!fich_tabla.good()){
        cout<<"Error al abrir el fichero: "<<path<<endl;
        exit(-1);
    }
    maximos = new float[tam_cadena_tabla];
    minimos = new float[tam_cadena_tabla];
    bool spambase = false;
    int num_datos = num_cadenas_tabla;
    if(string(path).compare("./tablas/spambase.arff")==0){
        spambase = true;
        num_datos = 4601;
    }

    while(!fich_tabla.eof() && i<num_datos){
        if(!leyendo_datos){
            //Pasamos de las lineas hasta llegar a los datos
            getline(fich_tabla,linea);
            if(linea.compare(0,5,"@data")==0 || linea.compare(0,5,"@DATA")==0){ //Paramos de descartar lineasporque hemos llegado a los datos
                leyendo_datos=true;
            }
        }else{
            if(spambase && i>num_cadenas_tabla){
                if(aux == 0){
                    getline(fich_tabla,linea);
                    j=0;
                    i++;
                    aux = Randint(0,1);
                }else{
                    fich_tabla >> datos_tabla[ind][j];
                    if(i==0){
                        maximos[j] = datos_tabla[ind][j];
                        minimos[j] = datos_tabla[ind][j];
                    }else{
                        if(maximos[j] < datos_tabla[ind][j])
                            maximos[j] = datos_tabla[ind][j];
                        if(minimos[j] > datos_tabla[ind][j])
                            minimos[j] = datos_tabla[ind][j];
                    }
                    fich_tabla.ignore(1,',');
                    j++;
                }
            }else{
                //Ahora que hemos encontrado los datos los leemos y guardamos en la matriz datos
                fich_tabla >> datos_tabla[i][j];
                if(i==0){
                    maximos[j] = datos_tabla[i][j];
                    minimos[j] = datos_tabla[i][j];
                }else{
                    if(maximos[j] < datos_tabla[i][j])
                        maximos[j] = datos_tabla[i][j];
                    if(minimos[j] > datos_tabla[i][j])
                        minimos[j] = datos_tabla[i][j];
                }
                fich_tabla.ignore(1,',');
                j++;
            }
            //Una vez que j>tam_cadena significa que ya hemos leido los datos de la cadena, el último valor es la clase así que las clases las almacenamos en el array clases
            if (j==tam_cadena_tabla){
                if(string(path).compare("./tablas/sonar.arff")==0){
                    getline(fich_tabla, linea);
                    if (linea.compare("Rock")==0)
                        clases_tabla[i]=0;
                    else
                        clases_tabla[i]=1;
                }else{
                    if(i>num_cadenas_tabla && aux==1)
                        fich_tabla >> clases_tabla[ind];
                    else
                        fich_tabla >> clases_tabla[i];
                }
                j=0;
                i++;
                aux = Randint(0,1);
                ind = Randint(0,tam_cadena_tabla-1);
            }
        }
    }
    fich_tabla.close();

    for(int i=0; i<num_cadenas_tabla; i++)
        for(int j=0; j<tam_cadena_tabla; j++){
            if ((maximos[j]-minimos[j]) != 0)
                datos_tabla[i][j] = (datos_tabla[i][j] - minimos[j]) / (maximos[j]-minimos[j]);
        }
}

float distancia(float * v1, float *v2, int n){
    float d = 0.0;
    for(int i=0; i<n; i++){
        if(sol.s[i])
            d = d + ((v2[i]-v1[i]) * (v2[i]-v1[i]));
    }
    return sqrt(d);
}

int pos_max(float* d_v, int n){
    int max = 0;
    for(int i=1; i<n; i++){
        if(d_v[max]<d_v[i])
            max = i;
    }
    return max;
}

int knn(int ini, int n, int k, int e){
    int clase= 0;
    float vmax=0;
    int p_vmax=0;
    int vecinos[k];
    float dist_vecinos[k], dist;
    int i,j;
    for(i=ini, j=0; j<k; i++, j++){
        if(i>=num_cadenas_tabla)
            i=0;
        vecinos[j] = i;
        dist_vecinos[j] = distancia(datos_tabla[i], datos_tabla[e], tam_cadena_tabla);
        if (vmax<dist_vecinos[j]){
            vmax = dist_vecinos[j];
            p_vmax = j;
        }
    }
    for(j=k; j<n; j++, i++){
        if(i>=num_cadenas_tabla)
            i=0;
        dist = distancia(datos_tabla[i], datos_tabla[e], tam_cadena_tabla);
        if(vmax>dist){
            vecinos[p_vmax] = i;
            dist_vecinos[p_vmax] = dist;
            p_vmax = pos_max(dist_vecinos, k);
            vmax = dist_vecinos[p_vmax];
        }
    }
    for (int i=0; i<k; i++){
        clase = clase + clases_tabla[vecinos[i]];
    }
    if(clase <= k*0.5)
        return 0;
    else
        return 1;
}



float calcularCoste(int ini, int tam_conjunto, int k){
    float acierto=0;
    int clase_propuesta;
    for (int cont=0,i=ini; cont<tam_conjunto; cont++,i++){
        if(i==num_cadenas_tabla)
            i=0;
        clase_propuesta = knn(ini,tam_conjunto,k,i);
        if(clase_propuesta==clases_tabla[i])
            acierto++;
    }
    return (acierto/tam_conjunto)*100.0;
}



void shuffle(){


    int ind =0;
    float copia;
    for(int i =0; i< num_cadenas_tabla; i++){
        ind = Randint(0,num_cadenas_tabla-1);
        for(int j = 0; j<tam_cadena_tabla; j++){
            copia = datos_tabla[i][j];
            datos_tabla[i][j] = datos_tabla[ind][j];
            datos_tabla[ind][j] = copia;
        }
    }
}
//Busqueda local basada en el primero el mejor



float calcularTasaRed(){
    int n =0;
    for(int i=0; i<tam_cadena_tabla; i++)
        if(sol.s[i])
            n++;
    return (float)(tam_cadena_tabla - n) * 100.0/tam_cadena_tabla;
}

STRUCT_CARACTERISTICAS LocalSearch(int ini,int tam ,int k){

    cout << "Comienza la LocalSearch" << endl;
    int ind = Randint(0,tam_cadena_tabla-1), cont = 0;
    bool mejora=false;
    float task_min_max = calcularCoste(ini, tam, k);

    for(unsigned int i=0; i<1; i++){

        sol.s[ind]=!sol.s[ind];
        sol.tasa_clas = calcularCoste(ini, tam, k);

        if(sol.tasa_clas > task_min_max){
            sol.tasa_red = calcularTasaRed();
            mejora = true;
        }
    }
        return sol;

}
4y7uj8
inline long double log2(const long double x){
    return  log(x) * M_LOG2E;
}

//Random con Randint()
/*
AntFarm::AntFarm(int nhormiga, int random, STRUCT_CARACTERISTICAS sol){

	this.nhormiga=nhormiga;
	r=r_number;
	this.sol=sol;

	//Inicializamos el vector de feromonas

} */


void inicializarFeromonas(){
	//Inicializamos  τ-carfi
    for (unsigned i=0;i<tam_cadena_tabla -1;i++)
		feromonasCaracteristicas[i]=0,000001;
	//Inicializamos
    for (unsigned i=0;i<tam_cadena_tabla-1;i++)
        feromonasNoCaracteristicas[i]=1/(tam_cadena_tabla -1);



	//Cada hormiga actualiza la matriz de feromonas al acabar la bl

}

float funcionHeuristica(int start,int caracteristica){

	//Calcularemos primero los intervalos

    float aux=0;
	int num_intervalos = 11;


	//Declaramos los vectores y los inicializamos

	int clase0[num_intervalos];
	int clase1[num_intervalos];

	for(int i=0;i<num_intervalos;i++){
		clase0[i]=0;
		clase1[i]=0;
	}

	//Calculamos los vectores de probabilidades para cada intervalo

	int p[2];
	p[0]=p[1]=0;

	//Calculamos la cantidad de muestras de cada clase en el train

	for(unsigned int i=start;i<((num_cadenas_tabla/2) +start) %num_cadenas_tabla;i++){

		if(datos_tabla[i][num_cadenas_tabla-1]==0)
			p[0]+=1;;
		if(datos_tabla[i][num_cadenas_tabla-1]==1)
			p[1]+=1;

	}

    cout << "he salido "<< endl;
	//Total observaciones de esa característica
    int totalObservacionesCaracteristica=0;

	for(int i=0;i<10;i++){
        totalObservacionesCaracteristica += (clase0[i] + clase1[i]);
	}

    for(unsigned j=0; j< tam_cadena_tabla -2; j++){	//Recorremos todas las caracteristicas

		//Calculamos los intervalos para cada carácterística

        float max=0,min=0,intervalo=0;float pfc=0, pc=0,fi=0;



		for(unsigned int i=start;i<((num_cadenas_tabla/2) +start) %num_cadenas_tabla ;i++){
            if(i>num_cadenas_tabla)
                i=0;

			if(datos_tabla[i][j]<min)
				min=datos_tabla[i][j];
			if(datos_tabla[i][j]>min)
				max=datos_tabla[i][j];
		}

		intervalo = (max+min)/10;

		//Calculamos la cantidad de muestras en cada intervalo

		for(unsigned int i=start;i<((num_cadenas_tabla/2) +start) %num_cadenas_tabla ;i++){
            if(i>num_cadenas_tabla)
                    i=0;

            if(datos_tabla[i][num_cadenas_tabla-1]==0){

                for(int inte=1;inte<11;inte++)
                    for(unsigned int w=start;i<((num_cadenas_tabla/2) +start) %num_cadenas_tabla ;w++){
                        if(w>num_cadenas_tabla)
                            w =0;
                        cout << "eo" << endl;
                        if(datos_tabla[w][j]>=intervalo*inte)
                            clase0[inte]+=1;
                    }
            }
            if(datos_tabla[i][num_cadenas_tabla-1]==1){

                for(int inte=1;inte<11;inte++)
                    for(unsigned int w=start;w<((num_cadenas_tabla/2) +start) %num_cadenas_tabla ;w++){
                        if(w>num_cadenas_tabla)
                            w =0;
                        if(datos_tabla[w][j]>=intervalo*inte)
                            clase1[inte]+=1;
                    }
            }


		cout << "llegue" << endl;

	}
		for(int c=0; c<2; c++)						//Para cada clase
			for(int inte=1;inte<11;inte++){			//Para cada intervalo

				//Calculamos pfc
                for(unsigned int i=0;i<10; i++){
					if(c=0)
                         pfc += (clase0[i] / (clase1[i] + clase0[i]));

					if(c=1)
                         pfc += (clase1[i] / (clase1[i] + clase0[i]));
				//Calculamos pc

                 pc = p[c] / p[0] + p[1];

				//Calculamos fi
                 fi=(clase0[inte]+clase1[inte])/totalObservacionesCaracteristica;
                 aux += pfc / log2(pc + fi);
                }
		}
	}

	return aux;
    cout << aux;
}

 void actualizarFeromonas(int inicio, int tam, int k){

	float aumento=0;

    for(int i=0; i<tam_cadena_tabla -1; i++){
		if(sol.s[i]==1)
            aumento = calcularCoste(inicio, tam, k); //coste knn
		feromonasCaracteristicas[i]=(1-0,2)*feromonasCaracteristicas[i]+aumento;
		aumento=0;
	}

	//el segundo vector de feromona, el del numero de caracteristicas, se actualiza siempre con la regla del SH, es decir,
	//EVAPORANDO TODOS LOS RASTROS Y APORTANDO CON TODAS LAS SOLUCIONES GENERADAS POR LAS HORMIGAS EN LA ITERACION ACTUAL.

    aumento=0;
    for(int i=0; i<tam_cadena_tabla -1; i++){
		if(sol.s[i]==1)
            aumento = calcularCoste(inicio, tam, k); //coste knn
	feromonasNoCaracteristicas[i]=(1-0,2)*feromonasCaracteristicas[i]+aumento;
	aumento=0;
	}

}

void actualizacionGlobalFeromonas(int inicio, int tam, int k){

	float aumento=0;
    for(int i=0; i<tam_cadena_tabla -1; i++){
		if(sol.s[i]==1)
            aumento = calcularCoste(inicio, tam, k); //coste knn

		feromonasCaracteristicas[i]=(1-0,2)*feromonasCaracteristicas[i]+((0,2)*(1/aumento));
		aumento=0;
	}

}

void Hormiga(int n, int inicio, int num_elementos){

	cout << "Soy la hormiga: "<< n << endl;


	int NcaracteristicasACoger=0, sum=0;

    for (unsigned i=0;i<(tam_cadena_tabla-1);i++){
		if(sum<1){
			sum += feromonasNoCaracteristicas[i];
			NcaracteristicasACoger++;
		}
	}

	//Seleccionamos las características para el vector solucion

    cout << "selecciono las caracteristicas para mi solución" << endl;

	float L[tam_cadena_tabla -1];

	for(int i=0; i<tam_cadena_tabla-1;i++){
		L[i]=0;
	}

    cout << "asejei" << endl;

	for(int i=0; i<tam_cadena_tabla-1;i++){
		L[i]=feromonasCaracteristicas[i]*(funcionHeuristica(inicio,i));
		cout << "valor para la heuristica: "<< L[i] << endl;
	}


	for(int contador=0;contador<NcaracteristicasACoger;contador++){

        int max=0;


		for(int i=0; i<tam_cadena_tabla-1;i++){
			if(L[i]>max)
				max=L[i];
		}

		for(int i=0; i<tam_cadena_tabla-1;i++){
			if(L[i]=max){
				sol.s[i]=1;
				contador++;
				L[i]=0;

			}
		}
	}

	//Ya tenemos el vector sol para la hormiga
	//Ejecutamos la búsqueda local

    cout << "ejecuto la BL" << endl;

    sol = LocalSearch(inicio,num_elementos,3);

	actualizarFeromonas(inicio, num_elementos,3);

	if(sol.tasa_clas>mejorSol.tasa_clas){
		mejorSol=sol;
		actualizacionGlobalFeromonas(inicio, num_elementos,3);
	}

}


void SchBl(int inicio,int num_elementos){

	//Ejecutamos la primera hormiga

	inicializarFeromonas();

    Hormiga(1,inicio,num_elementos);

	mejorSol=sol;

	for(unsigned int i=0; i< 1500; i++){

		for(unsigned int n=2;n<=10;n++)

            Hormiga(n,inicio,num_elementos);

	}

}



int main(int argc, char **argv){
  using namespace std;

 Set_random(210594);

 float tasa_red[10];
 float tasa_clas_test[10];
 float tasa_clas_entrena[10];
 double time[10];
 int num_elementos = (int)(num_cadenas_tabla*0.5);
 clock_t crono;

 leerTabla();
 shuffle();

 double aciertos = 0.0;
 int entrenamiento, test;



	 for(int i=0; i<5; i++){
		 crono = clock();
         entrenamiento = Randint(0,num_cadenas_tabla-1);
        test = (int)(entrenamiento + (num_cadenas_tabla*0.5))%(int)num_cadenas_tabla;

         SchBl(entrenamiento,num_elementos);
		 tasa_red[i+i] = sol.tasa_red;
 		tasa_clas_entrena[i+i] = sol.tasa_clas;
		 tasa_clas_test[i+i] = calcularCoste(entrenamiento, num_elementos, 3);
		 time[i+i] = ((double)(clock()- crono)/CLOCKS_PER_SEC);

         SchBl(test,num_elementos);
		 tasa_red[i+i+1] = sol.tasa_red;
 		tasa_clas_entrena[i+i+1] = sol.tasa_clas;
 		tasa_clas_test[i+i+1] = calcularCoste(test, num_elementos, 3);
 		time[i+i+1] = ((double)(clock()- crono)/CLOCKS_PER_SEC);
	 }
	 float tasa_clas_media=0.0;
 	float tasa_red_media=0.0;
    /*for(int i=0; i<10; i++){
		 cout«"Particion "«((int)((i*0.5)+1))«"-"«(i%2)+1«": ";
		 cout«"clas_test:"«tasa_clas_test[i]«"; clas_entrena:"«tasa_clas_entrena[i]«"; red:"«tasa_red[i]«"; T:"«time[i]«endl;
		 tasa_clas_media += tasa_clas_test[i];
		 tasa_red_media += tasa_red[i];
    }*/
	 tasa_clas_media = tasa_clas_media / 10;
	 tasa_red_media = tasa_red_media / 10;
 /*	cout«"tasa_red: "«tasa_red_media«endl;
 	cout«"tasa_clas: "«tasa_clas_media«endl;
    */

}





