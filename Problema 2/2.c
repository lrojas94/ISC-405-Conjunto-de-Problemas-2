#include <stdio.h>
#include <string.h>

#define MAX_SIZE 1000

int N[MAX_SIZE + 2];
int MEMO[MAX_SIZE + 2][2];
int n;

int max(int a, int b){
	return (a > b) ? a : b;
}

// flag == 1, FORCEFULLY using the last element
// flag == 0, FORCEFULLY NOT using the last element
int dp(int i, int flag){
	if(i >= n)	return 0;
	if(MEMO[i][flag] != -1)
		return MEMO[i][flag];
	
	int tomar = 0, dejar = 0;
	
	// Not using the last element
	tomar = N[i] + dp(i+2,0);
	dejar = dp(i+1, 0);
	MEMO[i][0] = max(tomar, dejar);
	
	// Using the last element
	tomar = (i == 0 ? 0 : N[i]) + dp(i+2, 1);
	dejar = dp(i+1, 1);
	MEMO[i][1] = max(tomar, dejar);	
	
	return MEMO[i][flag];
}

int solucionar(){
	int sol;
	
	// Preparing the memorization matrix
	memset(MEMO, -1, sizeof(MEMO));
	MEMO[n-1][1] = N[n-1];
	MEMO[n-1][0] = 0;
	
	dp(0, 0);
	
	sol = max(MEMO[0][0], MEMO[0][1]);
	
	return sol;
}

int main(){
	int i;
	
	scanf("%d", &n);
	
	for(i = 0; i < n; i++)	
		scanf("%d", &N[i]);
	
	memset(MEMO, -1, sizeof(MEMO));
	MEMO[n-1][1] = N[n-1];
	MEMO[n-1][0] = 0;
	int solucion = solucionar();
	
	printf("%d\n", solucion);
	
	return 0;
}

// SAMPLE TEST CASE, ANSWER = 468
// 16
// 90 1 2 89 3 4 88 5 6 87 5 4 86 3 28 5
