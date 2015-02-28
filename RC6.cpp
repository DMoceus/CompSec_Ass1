#include <iostream>
#include <cmath>

using namespace std;

#define WORD_SIZE 32
#define ROUND_COUNT 20
/*
int main(int argc, char** argv){
	int temp = 0b11011011;
	int shiftAmount = 0b11010011;
	cout << temp << endl;
	int temp2 = leftShift(temp,shiftAmount);
	cout << temp2 << endl;
	return 0;
}
*/
int addMod(int a, int b){
	return (a+b)%(int)pow(2,WORD_SIZE);
}

int subMod(int a, int b){
	return (a-b)%(int)pow(2,WORD_SIZE);
}

int multMod(int a, int b){
	return (a*b)%(int)pow(2,WORD_SIZE);
}

long leftShift(long a, long b){
	long long aLong = a;
	int numBits = log2(WORD_SIZE);
	int shiftCountMask = 0b0;
	for(int i=0; i<numBits; i++){
		shiftCountMask = (shiftCountMask << 1);
		shiftCountMask++;
	}	
	int shiftCount = b & shiftCountMask;
	
	long int maskA = 0b0;
	long int maskB = 0b0;

	for(int i=0; i<WORD_SIZE; i++){
		maskA = (maskA << 1);
		maskA++;
	}
	
	for(int i=0; i<shiftCount; i++){
		maskB = (maskB << 1);
		maskB++;
	}
	maskB = (maskB << WORD_SIZE);

	aLong = (a << shiftCount);
	long temp = aLong & maskB;
	aLong = (aLong & maskA);
	temp = (temp >> WORD_SIZE);
	return aLong | temp;
}

int rightShift(int a, int b){
	
}


int main(int argc, char** argv){
	long temp = 0b10000000000000000000000000000000;
	int shiftAmount = 0b001;
	cout << temp << endl;
	long temp2 = leftShift(temp,shiftAmount);
	cout << temp2 << endl;
	return 0;
}
