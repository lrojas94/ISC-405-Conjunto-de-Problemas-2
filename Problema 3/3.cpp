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
int sp;
vector<int> ** STACK;
vector<int> PRIMOS;

int n;

void criba(){
	int i, j;
	
	unsigned long long * BITMAP = new unsigned long long[MAX_NUMBER / 64];
	
	for(i = 2; i < MAX_NUMBER; i++){
		if( (BITMAP[(i >> 6LL)] & (1LL << (i & 6LL))) )	continue;
		
		PRIMOS.push_back(i);
		for(j = i << 1; j < MAX_NUMBER; j += i)
			BITMAP[(j >> 6LL)] |= (1LL << (j & 6LL));
	}
	
	delete BITMAP;
	BITMAP = NULL;
}

vector<int> * factores(int n){
	vector<int> * f = new vector<int>(0);
	(*f).push_back(0);
	int cant = 0;
	
	for(unsigned i = 0; i < PRIMOS.size() && PRIMOS[i] <= n; i++){
		if(n % PRIMOS[i] == 0){
			cant++;
			(*f).push_back(PRIMOS[i]);
		}
	}
	
	(*f)[0] = cant+1;
	return f;
}

/*
llu gcd(llu a, llu b){
	if(a == b)	return a;
	
	if(a < b){	// Swap without a bubble variable
		a = a + b;
		b = a - b;
		a = a - b;
	}
	
	if(a % b == 0)	return b;
	
	llu r = a % b;
	while(r != 0){
		a = b;
		b = r;
		r = a % b;
	}
	
	return b;
}
*/

bool coprime(vector<int> * factors){
	for(int i = 1; i < (*factors)[0]; i++){
		//printf("factors[%d] = %d\t", i, (*factors)[i]);
		//printf("MASK[factors[%d]] = %d\n", i, MASK[(*factors)[i]]);
		
		if(MASK[(*factors)[i]])
			return false;
	}
	
	return true;
}

void addFactors(vector<int> * f){
	for(int k = 1; k < (*f)[0]; k++){
		//printf("f[%d] = %d\n", k, (*f)[k]);
		MASK[(*f)[k]] = 1;
	}
}

void removeFactors(vector<int> * f){
	for(int k = 1; k < (*f)[0]; k++)
		MASK[(*f)[k]] = 0;
}

int knapsack(int i, int cant){
	//printf("%d\n", sp);
	if(i >= n)	return 0;
	
	vector<int> * f = factores(N[i]);
	bool coprime_ = coprime(f);
	
	if(coprime_ && MEMO[1][i] != -1)
			return MEMO[1][i];
	if(!coprime_ && MEMO[0][i] != -1)
			return MEMO[0][i];
	
	//~ printf("i=%d cant=%d producto=%lld N[i]=%lld\n", i, cant, producto, N[i]);
	
	//~ printf("Tomando el N[%d]=%lld\n", i, N[i]);
	bool agregado = false;
	int tomar;
	
	//cout << "coprime_ = " << coprime_ << endl;
	
	if(coprime_){
		addFactors(f);
		//cout << "Factors added" << endl;
		
		//for(int x = 1; x <= 5; x++)
			//cout << MASK[x] << " ";
		//cout << endl;
		
		STACK[sp++] = f;
		agregado = true;
		tomar = knapsack(i+1, cant+1) + 1;
	}
	else
		tomar = 0;
	
	//~ printf("Dejando el N[%d]=%lld\n", i, N[i]);
	if(agregado){
		removeFactors(STACK[sp-1]);
		
		//for(int x = 1; x <= 5; x++)
			//cout << MASK[x] << " ";
		//cout << endl;
		
		//cout << "Factors removed" << endl;
		sp--;
		delete STACK[sp];
	}
	int dejar = knapsack(i+1, cant);
	
	MEMO[0][i] = dejar;
	MEMO[1][i] = tomar;
	
	return (!coprime_) ? MEMO[0][i] : MEMO[1][i];
}

int main(){
	cin >> n;
	
	N = new int[n];
	MEMO[0] = new int[n];
	MEMO[1] = new int[n];
	STACK = new vector<int> * [n];
	
	memset(MEMO[0], -1, sizeof(int) * n);
	memset(MEMO[1], -1, sizeof(int) * n);
	
	criba();
	for(int i = 0; i < n; i++)
		cin >> N[i];
	
	memset(STACK, 0, sizeof(int) * n);
	int sol = knapsack(0, 0);
	
	cout << sol << endl;
	while(true);
	return 0;
}
