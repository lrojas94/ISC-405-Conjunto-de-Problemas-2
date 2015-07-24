#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> N;
vector<int> SUM;
vector<pair<int,int> > intervalos_completos;	// intervalos cerrados
vector<int> no_favorables;
int cantidad_max_particiones;
int cantidad_relleno_no_favorable;
int n, K;

int minimo(int a, int b){
	return (a < b) ? a : b;
}

void IntervalosMaximos(){
	int i, j;
	pair<int, int> intervalo;
	
	for(i = 0; i < n; i++){
		
		for(j = i; N[j] > 0; j++);
		
		if(j > i){
			intervalo.first = i;
			intervalo.second = j-1;
			intervalos_completos.push_back(intervalo);
			cantidad_max_particiones += j-i;
			i = j;
		}
		
	}
}

bool cmp_no_favorable(int a, int b){
	return (a > b);
}

bool cmp_intervalo(pair<int,int> a, pair<int,int> b){
	return ( (a.second-a.first+1) < (b.second-b.first+1) );
}

int calcular_suma(unsigned cantidad_intervalos_maximo){
	unsigned i;
	int suma = 0;
	
	for(i = 0; i < cantidad_intervalos_maximo; i++){
		suma +=	SUM[intervalos_completos[i].second] -
				SUM[intervalos_completos[i].first] +
				N[intervalos_completos[i].first];
	}
	
	return suma;
}

int main(){
	int sol = 0, relleno = 0;
	cin >> n >> K;
	
	if(K > n){
		cout << endl << "Invalido" << endl;
		return 0;
	}
	
	N.reserve(n);
	SUM.reserve(n);
	for(int i = 0; i < n; i++){
		cin >> N[i];
		SUM[i] = N[i] + ((i == 0) ? 0 : SUM[i-1]);
		if(N[i] <= 0)	no_favorables.push_back(N[i]);
	}
	
	IntervalosMaximos();
	
	// Consideraciones sobre la cantidad de intervalos
	if(K < (int) intervalos_completos.size()){
		sort(intervalos_completos.begin(), intervalos_completos.end(),
			cmp_intervalo);
	}		
	
	if(K > cantidad_max_particiones){
		sort(no_favorables.begin(), no_favorables.end(), cmp_no_favorable);
		
		cantidad_relleno_no_favorable = (K - cantidad_max_particiones > 0) ? 
			K - cantidad_max_particiones : 0;
		
		for(int i = 0; i < cantidad_relleno_no_favorable; i++)
			relleno += no_favorables[i];
	}
	
	sol = calcular_suma( minimo(K, intervalos_completos.size()) );
	
	cout << sol + relleno;
	return 0;
}

// Conseguir todos los intervalos maximos compuestos por enteros positivos
// Calcular la cantidad de particiones maxima posible
// Ordenar los no_favorables en orden no descendente y rellenar con ellos
