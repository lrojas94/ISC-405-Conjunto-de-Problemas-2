/*
 * *** BIGGEST PAIRWISE COPRIME SET ***
 * 
 * takes an integer n and then reads n integers from input
 * 
 * IMPORTANT: USE NUMBERS UP TO 10^6, IF YOU NEED MORE, CHANGE THE MAX_NUMBER
 * MACRO AS YOU NEED (beware the memory usage :)
 * 
 * */
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>

#define MAX_NUMBER 1000000

using namespace std;

typedef unsigned long long int llu;

int * N;
int * MEMO[2];
int MASK[MAX_NUMBER+2];
vector<int> ** PRIME_FACTORS;
vector<int> PRIMES;

int n;

void sieve(){
	int i, j;
	
	bool * BITMAP = new bool[MAX_NUMBER];
	memset(BITMAP, false, sizeof(bool) * MAX_NUMBER);
	
	for(i = 2; i < MAX_NUMBER; i++){
		if(BITMAP[i] == true)		continue;
		
		PRIMES.push_back(i);
		for(j = i + i; j < MAX_NUMBER; j += i)
			BITMAP[j] = true;
	}
	
	delete BITMAP;
	BITMAP = NULL;
}

vector<int> * getFactors(int n){
	vector<int> * f = new vector<int>(0);
	(*f).push_back(0);
	int count = 0;
	
	for(unsigned i = 0; i < PRIMES.size() && PRIMES[i] <= n; i++){
		if(n % PRIMES[i] == 0){
			count++;
			(*f).push_back(PRIMES[i]);
		}
	}
	
	(*f)[0] = count+1;
	return f;
}

bool coprime(vector<int> * f){
	for(int i = 1; i < (*f)[0]; i++){
		if(MASK[(*f)[i]])
			return false;
	}
	
	return true;
}

void addFactors(vector<int> * f){
	for(int k = 1; k < (*f)[0]; k++)
		MASK[(*f)[k]] = 1;
}

void removeFactors(vector<int> * f){
	for(int k = 1; k < (*f)[0]; k++)
		MASK[(*f)[k]] = 0;
}

int knapsack(int i){
	if(i >= n)	return 0;
	
	bool coprime_ = coprime(PRIME_FACTORS[i]);
	
	if(coprime_  && MEMO[1][i] != -1)	return MEMO[1][i];
	if(!coprime_ && MEMO[0][i] != -1)	return MEMO[0][i];
	
	bool added = false;
	int take;
	
	if(coprime_){
		addFactors(PRIME_FACTORS[i]);
		added = true;
		take = knapsack(i+1) + 1;
	}
	else
		take = 0;
	
	if(added)
		removeFactors(PRIME_FACTORS[i]);
	
	int ignore = knapsack(i+1);
	
	MEMO[0][i] = ignore;
	MEMO[1][i] = take;
	
	return (!coprime_) ? MEMO[0][i] : MEMO[1][i];
}

void fillMap(){
	for(int i = 0; i < n; i++)
		PRIME_FACTORS[i] = getFactors(N[i]);
}

int main(){
	cin >> n;
	
	N = new int[n];
	MEMO[0] = new int[n];
	MEMO[1] = new int[n];
	PRIME_FACTORS = new vector<int> * [n];
	
	memset(MEMO[0], -1, sizeof(int) * n);
	memset(MEMO[1], -1, sizeof(int) * n);
	
	sieve();
	for(int i = 0; i < n; i++)
		cin >> N[i];
	
	memset(PRIME_FACTORS, 0, sizeof(int) * n);
	fillMap();
	
	cout << knapsack(0) << endl;

	return 0;
}

/* Explicacion:
 * 
 * El algoritmo se basa en la recurrencia del problema 01knapsack para decidir 
 * si tomar o no un elemento de acuerdo en base a las restricciones de que todos
 * los elementos en el conjunto final sean coprimos entre si (es decir que para
 * cualquier par de numeros en el conjunto, esos numeros deberan de tener como
 * su maximo comun divisor a la unidad, es decir, el numero 1).
 * 
 * Para lograr esto se genera una lista de numeros primos hasta un limite dado
 * (indicado por la macro MAX_NUMBER) los cuales se utilizaran para encontrar 
 * los factores primos de cada numero. Esos numeros primos son generados con el
 * metodo de la criba de Eratostenes (Erathosthenes sieve) cuya complejidad es
 * de O(N * log(log N)).
 * 
 * Los factores de cada i-esimo numero primo son almacenados en forma de puntero
 * a vector en su respectiva posicion en la matriz PRIME_FACTORS. La funcion
 * fillMap() se encarga de llenar esta matriz utilizando la funcion 
 * getFactors(numero), la cual retorna los factores primos del numero dado.
 * 
 * La funcion knapsack es la encargada de manejar todos los elementos a fin de
 * alcanzar la solucion final, esta es la encargada de la matriz de memoizacion,
 * lo que permite ahorrar grandes cantidades de tiempo y poder de computo al 
 * almacenar soluciones a subproblemas ya computados en el transcurso del 
 * computo de la solucion total ... es decir, si el algoritmo se encontrase con
 * la necesidad de recomputar algo que ya habia computado, este solo habra de 
 * tomar el valor computado de la matriz de memoizacion en vez de recalcular
 * todo.
 * 
 * Como tal, la manera por la cual la funion knapsack obtiene la solucion es 
 * revisando los distintos casos donde, se puede agregar o no un elemento dado
 * al conjunto coprimo que en ese momento se esta construyendo y almacenando la
 * cantidad maxima de elementos que puede tener el mayor conjunto comprimo desde
 * una posicion i hasta el ultimo elemento del arreglo N (numeros de la entrada),
 * ese valor calculado se almacena en MEMO[x][i] ddonde x puede ser 0 o 1, x = 0
 * quiere decir "cantidad de elementos del maximo conjunto coprimo posible desde
 * i hasta el ultimo elemento SIN tomar el elemento N[i]" y cuando x = 1 es lo 
 * mismo con la diferencia de que si se toma el elemento N[i].
 * 
 * La funcion coprime(factores) recibe un puntero a vector con los factores 
 * primos de un entero dado y determina si puede o no agregarse al conjunto que
 * se esta construyendo en ese momento. Si se habia computado previamente la
 * solucion, es decir, si MEMO[coprimo(factores)][i] es diferente de -1 (valor
 * inicial para los elementos en la matriz de memoizacion a fin de distinguir 
 * los valores computados y no computados) entonces solo debes retornar ese 
 * valor y no recomputar de nuevo. La memoizacion es muy util !.
 * 
 * Sin embargo, si no se ha computado esa respuesta entonces habra que hacerlo:
 * Para computar la respuesta hay que tomar en cuenta el elemento actual a 
 * revisar, cuyo indice en el arreglo N es i, por lo que el elemento en cuestion
 * es N[i] y lo que queremos hacer con el es saber cual es el mejor conjunto 
 * coprimo que podemos obtener agregando los elementos desde i hasta el final
 * (solo se agregan los elementos que pueden pertenecer al actual conjunto).
 * 
 * Para ilustrar, un caso de prueba con los primeros 5 numeros primos:
 * 
 * 	5
 * 	2 3 5 7 11
 * 
 * el algoritmo empieza con i = 0 (denotando el primer elemento en base-0) y se
 * pregunta si el numero N[0], osea, si el numero 2 puede pertenecer al actual
 * conjunto coprimo que se esta construyendo (como es el inicio, el conjunto 
 * esta vacio), entonces pide las soluciones desde el siguiente elemento,
 * es decir, desde el numero 3 pero antes el marca en la mascara (arreglo MASK)
 * a los factores primos del elemento que acaba de agregar al conjunto, en el 
 * caso de 2 solo se marca el numero 2 en la mascara por ser 2 un numero primo.
 * 
 * En la solucion para i = 1 (segundo elemento), N[1] = 3, se considera si el
 * numero 3 puede agregarse al conjunto actualmente compuesto por el numero 2,
 * dado que ambos son primos es un si, entonces se marca al 3 en la mascara y
 * se pide la solucion del siguiente elemento (N[2] = 5) ... esto sigue hasta 
 * que en la ultima posicion (N[4] = 11) se dice pedir la solucion siguiente, 
 * pero ya no hay mas numeros por lo que esa solucion retornara 0 queriendo
 * significar que despues del ultimo elemento, la mayor cantidad de elementos
 * que se pueden agregar al conjunto es 0, es entonces que volvemos al elemento
 * anterior (N[4] == 11) y dado que la solucion para i = 5 fue 0, la solucion
 * para N[4] = 11 TOMANDOLO es de 0 + 1, notese el + 1 en el knapsack(i+1) + 1
 * de la linea 97.
 * 
 * Ya hemos calculado el mejor conjunto desde 11 incluyendole, ahora calculamos 
 * el mejor conjunto desde 11 sin tomarle, como ya no hay mas elementos esto 
 * sera 0 y volveremos al numero antes del 11, el 7, el elemento 7 habia llamado
 * a la solucion del elemento 11 cuando el 7 se incluia en el conjunto, y la 
 * solucion del elemento 11 es 1 (notese en la linea 110 como se considera que
 * valor calculado retornar, lo que significa esa linea es "si se pudo tomar el
 * elemento actual entonces retorna la solucion tomandolo, sino, retorna la 
 * solucion sin tomarlo"
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * */
