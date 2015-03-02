#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

#define WORD_SIZE 32
#define ROUND_COUNT 20
#define P32 0xB7E15163
#define Q32 0x9E3779B9

string removeWhiteSpace(string input);
void scanInput(string input);
void decrypt();
void encrypt();
long rightShift(long a, long b);
long leftShift(long a, long b);
long multMod(long a, long b);
long subMod(long a, long b);
long addMod(long a, long b);
long long lilEndian32Swap(long long input);
void keySchedule(string inKey);

long* S;
long* L;
long inA;
long inB;
long inC;
long inD;

void scanInput(string input){
	inA = stol(input.substr(0, 8),NULL,16);
	inB = stol(input.substr(8, 8),NULL,16);
	inC = stol(input.substr(16,8),NULL,16);
	inD = stol(input.substr(24,8),NULL,16);
}

void decrypt(){
	int t,u;
	long temp1, temp2, temp3, temp4;
	inC = subMod(inC,S[(2*ROUND_COUNT)+3]);
	inA = subMod(inA,S[(2*ROUND_COUNT)+2]);
	for(int i=ROUND_COUNT; i>0; i--){
		//(A,B,C,D) = (D,A,B,C)
		temp1 = inA;
		temp2 = inB;
		temp3 = inC;
		temp4 = inD;
		
		inA = temp4;
		inB = temp1;
		inC = temp2;
		inD = temp3;
	
		u = leftShift(multMod(inD,addMod(2*inD,1)),log2(WORD_SIZE));
		t = leftShift(multMod(inB,addMod(2*inB,1)),log2(WORD_SIZE));
		inC = (rightShift(subMod(inC,S[(2*i)+1]),t))^u;
		inA = (rightShift(subMod(inA,S[2*i]),u))^t;
	}
	inD = subMod(inD,S[1]);
	inB = subMod(inB,S[0]);
}

void encrypt(){
	int t,u;
	long temp1, temp2, temp3, temp4;
	inB = addMod(inB,S[0]);
	inD = addMod(inD,S[1]);
	for(int i=0;i<(ROUND_COUNT+1);i++){
		t = leftShift(multMod(inB,addMod(2*inB,1)),log2(WORD_SIZE));
		u = leftShift(multMod(inD,addMod(2*inD,1)),log2(WORD_SIZE));
		inA = addMod(leftShift((inA^t),u),S[2*i]);
		inC = addMod(leftShift((inC^u),t),S[(2*i)+1]);

		//(A,B,C,D) = (B,C,D,A)
		temp1 = inA;
		temp2 = inB;
		temp3 = inC;
		temp4 = inD;
		
		inA = temp2;
		inB = temp3;
		inC = temp4;
		inD = temp1;
	}
	
	inA = addMod(inA,S[(2*ROUND_COUNT)+2]);
	inC = addMod(inC,S[(2*ROUND_COUNT)+3]);
}

long long lilEndian32Swap(long long input){
	long long inBlock1 = (input >> 24) &  0x000000FF;
	long long inBlock2 = (input >> 8)  &  0x0000FF00;
	long long inBlock3 = (input << 8)  &  0x00FF0000;
	long long inBlock4 = (input << 24) &  0xFF000000;
	return inBlock1 | inBlock2 | inBlock3 | inBlock4;
}

void keySchedule(string inKey){
	int inKeyBytes = inKey.length()/2;
	if(inKeyBytes == 16){
		L = (long*)malloc(4*32);
/*		L[0] = stol(inKey.substr(0,8),NULL,16);
		L[1] = stol(inKey.substr(8,8),NULL,16);
		L[2] = stol(inKey.substr(16,8),NULL,16);
		L[3] = stol(inKey.substr(24,8),NULL,16);
*/
		L[0] = lilEndian32Swap(stol(inKey.substr(0,8), NULL,16));
		L[1] = lilEndian32Swap(stol(inKey.substr(8,8), NULL,16));
		L[2] = lilEndian32Swap(stol(inKey.substr(16,8),NULL,16));
		L[3] = lilEndian32Swap(stol(inKey.substr(24,8),NULL,16));
	}
	else if(inKeyBytes == 24){
		L = (long*)malloc(6*32);
/*		L[0] = stol(inKey.substr(0,8),NULL,16);
		L[1] = stol(inKey.substr(8,8),NULL,16);
		L[2] = stol(inKey.substr(16,8),NULL,16);
		L[3] = stol(inKey.substr(24,8),NULL,16);
		L[4] = stol(inKey.substr(32,8),NULL,16);
		L[5] = stol(inKey.substr(40,8),NULL,16);
*/
		L[0] = lilEndian32Swap(stol(inKey.substr(0,8), NULL,16));
		L[1] = lilEndian32Swap(stol(inKey.substr(8,8), NULL,16));
		L[2] = lilEndian32Swap(stol(inKey.substr(16,8),NULL,16));
		L[3] = lilEndian32Swap(stol(inKey.substr(24,8),NULL,16));
		L[4] = lilEndian32Swap(stol(inKey.substr(32,8),NULL,16));
		L[5] = lilEndian32Swap(stol(inKey.substr(40,8),NULL,16));

	}
	else if(inKeyBytes == 32){
		L = (long*)malloc(8*32);
/*		L[0] = stol(inKey.substr(0,8),NULL,16);
		L[1] = stol(inKey.substr(8,8),NULL,16);
		L[2] = stol(inKey.substr(16,8),NULL,16);
		L[3] = stol(inKey.substr(24,8),NULL,16);
		L[4] = stol(inKey.substr(32,8),NULL,16);
		L[5] = stol(inKey.substr(40,8),NULL,16);
		L[6] = stol(inKey.substr(48,8),NULL,16);
		L[7] = stol(inKey.substr(56,8),NULL,16);
*/
		L[0] = lilEndian32Swap(stol(inKey.substr(0,8), NULL,16));
		L[1] = lilEndian32Swap(stol(inKey.substr(8,8), NULL,16));
		L[2] = lilEndian32Swap(stol(inKey.substr(16,8),NULL,16));
		L[3] = lilEndian32Swap(stol(inKey.substr(24,8),NULL,16));
		L[4] = lilEndian32Swap(stol(inKey.substr(32,8),NULL,16));
		L[5] = lilEndian32Swap(stol(inKey.substr(40,8),NULL,16));
		L[6] = lilEndian32Swap(stol(inKey.substr(48,8),NULL,16));
		L[7] = lilEndian32Swap(stol(inKey.substr(56,8),NULL,16));

	}
	else{
		cout << "ERROR WITH KEYS\n";
		exit(-1);
	}
	S = (long*)malloc(WORD_SIZE*((2*ROUND_COUNT)+4));
	S[0] = P32;
	int i,j,v;
	long A,B;
	for(i=1;i<((2*ROUND_COUNT)+4);i++){
		S[i] = addMod(S[i-1],Q32);
	}
	A = B = i = j = 0;
	v = 3*max(inKeyBytes/4,((2*ROUND_COUNT)+4));
	for(int s=1; s<(v+1); s++){
		A = S[i] = leftShift(addMod(addMod(S[i],A),B),3);
		B = L[j] = leftShift(addMod(addMod(L[j],A),B),addMod(A,B));
		i = addMod(i,1)%((2*ROUND_COUNT)+4);
		j = addMod(j,1)%(inKeyBytes/4);
	}
}

long addMod(long a, long b){
	return (a+b)%4294967296;//(long)pow(2,WORD_SIZE);
}

long subMod(long a, long b){
	return (a-b)%4294967296;//(long)pow(2,WORD_SIZE);
}

long multMod(long a, long b){
	return (a*b)%4294967296;//(long)pow(2,WORD_SIZE);
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
	unsigned long temp = aLong & maskB;
	aLong = (aLong & maskA);
	temp = (temp >> WORD_SIZE);
	return (aLong | temp)&0xFFFFFFFF;
}

long rightShift(long a, long b){
	long long aLong = a;
	int numBits = log2(WORD_SIZE);
	int shiftCountMask = 0b0;
	for(int i=0; i<numBits; i++){
		shiftCountMask = (shiftCountMask << 1);
		shiftCountMask++;
	}
	int shiftCount = b & shiftCountMask;
	long maskA = 0b0;
	long maskB = 0b0;
	for(int i=0; i<(WORD_SIZE-shiftCount); i++){
		maskA = (maskA << 1);
		maskA++;
	}
	for(int i=0; i<shiftCount; i++){
		maskB = (maskB << 1);
		maskB++;
	}
	
	long remainder = (aLong >> shiftCount) & maskA;
	long cutoff = (aLong & maskB);
	cutoff = (cutoff << (WORD_SIZE-shiftCount));
	return (cutoff | remainder)&0xFFFFFFFF;
}


string removeWhiteSpace(string input){
	for(int i=0; i<input.length(); i++){
		if(' ' == input[i]){
			input.erase(i,1);
		}
	}
	return input;
}

int main(int argc, char** argv){
	inA = inB = inC = inD = 0b0;

	if(argc != 3){
		cout << "ERROR: EXPECTED INPUT\n./RC6 <input> <output>\n";
		exit(-1);
	}
	
	ifstream inFile;
	inFile.open(argv[1]);
	if(!inFile.is_open()){
		cout << "ERROR: FILE NOT OPENED CORRECTLY\n";
		exit(-1);
	}
	string inString;
	getline(inFile,inString);
	if(0 == inString.compare("Encryption")){
		cout << "Encryption!\n";
		getline(inFile,inString);
		inString = inString.substr(11);
		cout << inString << endl;
		string tempString = removeWhiteSpace(inString);
//		cout << "inString: " << inString << endl;
//		cout << "tempString: " << tempString << endl;
		scanInput(tempString);
		getline(inFile,inString);
		inString = inString.substr(9);
		inString = removeWhiteSpace(inString);
		keySchedule(inString);
		encrypt();
		inFile.close();
		ofstream outFile;
		outFile.open(argv[2]);
		string outString = "ciphertext: ";
		string outputHandler = "";
		stringstream aa;
		aa << hex << inA;
		aa >> outputHandler;
		if(inA == 0x0FFFFFFF&inA){
			outputHandler.insert(0,1,'0');
		}
	
		cout << "inA: " << outputHandler << endl;
	
		outString.append(outputHandler.substr(0,2));
		outString.append(" ");
		outString.append(outputHandler.substr(2,2));
		outString.append(" ");
		outString.append(outputHandler.substr(4,2));
		outString.append(" ");
		outString.append(outputHandler.substr(6,2));
		outString.append(" ");
		
		stringstream bb;
		bb << hex << inB;
		outputHandler = "";
		bb >> outputHandler;
		if(inB == 0x0FFFFFFF&inB){
			outputHandler.insert(0,1,'0');
		}
		cout << "inB: " << outputHandler << endl;
		
		outString.append(outputHandler.substr(0,2));
		outString.append(" ");
		outString.append(outputHandler.substr(2,2));
		outString.append(" ");
		outString.append(outputHandler.substr(4,2));
		outString.append(" ");
		outString.append(outputHandler.substr(6,2));
		outString.append(" ");	

		stringstream cc;
		cc << hex << inC;
		outputHandler = "";
		cc >> outputHandler;
	
		if(inC == 0x0FFFFFFF&inC){
			outputHandler.insert(0,1,'0');
		}

		cout << "inC: " << outputHandler << endl;
		
		outString.append(outputHandler.substr(0,2));
		outString.append(" ");
		outString.append(outputHandler.substr(2,2));
		outString.append(" ");
		outString.append(outputHandler.substr(4,2));
		outString.append(" ");
		outString.append(outputHandler.substr(6,2));
		outString.append(" ");	
	
		stringstream dd;
		dd << hex << inD;
		outputHandler = "";
		dd >> outputHandler;
		if(inD == 0x0FFFFFFF&inD){
			outputHandler.insert(0,1,'0');
		}
		cout << "inD: " << outputHandler << endl;		

		outString.append(outputHandler.substr(0,2));
		outString.append(" ");
		outString.append(outputHandler.substr(2,2));
		outString.append(" ");
		outString.append(outputHandler.substr(4,2));
		outString.append(" ");
		outString.append(outputHandler.substr(6,2));
		cout << outString << endl;
		outFile << outString << endl;
		outFile.close();
	}
	else if(0 == inString.compare("Decryption")){

		cout << "Decryption!\n";
		getline(inFile,inString);
		inString = inString.substr(12);
		cout << inString << endl;
		string tempString = removeWhiteSpace(inString);
//		cout << "inString: " << inString << endl;
//		cout << "tempString: " << tempString << endl;
		scanInput(tempString);
		getline(inFile,inString);
		inString = inString.substr(9);
		inString = removeWhiteSpace(inString);
		keySchedule(inString);
		decrypt();
		inFile.close();
		ofstream outFile;
		outFile.open(argv[2]);
		string outString = "plaintext: ";
		string outputHandler = "";
		stringstream aa;
		aa << hex << inA;
		aa >> outputHandler;
		if(inA == 0x0FFFFFFF&inA){
			outputHandler.insert(0,1,'0');
		}
	
		cout << "inA: " << outputHandler << endl;
	
		outString.append(outputHandler.substr(0,2));
		outString.append(" ");
		outString.append(outputHandler.substr(2,2));
		outString.append(" ");
		outString.append(outputHandler.substr(4,2));
		outString.append(" ");
		outString.append(outputHandler.substr(6,2));
		outString.append(" ");
		
		stringstream bb;
		bb << hex << inB;
		outputHandler = "";
		bb >> outputHandler;
		if(inB == 0x0FFFFFFF&inB){
			outputHandler.insert(0,1,'0');
		}
		cout << "inB: " << outputHandler << endl;
		
		outString.append(outputHandler.substr(0,2));
		outString.append(" ");
		outString.append(outputHandler.substr(2,2));
		outString.append(" ");
		outString.append(outputHandler.substr(4,2));
		outString.append(" ");
		outString.append(outputHandler.substr(6,2));
		outString.append(" ");	

		stringstream cc;
		cc << hex << inC;
		outputHandler = "";
		cc >> outputHandler;
	
		if(inC == 0x0FFFFFFF&inC){
			outputHandler.insert(0,1,'0');
		}

		cout << "inC: " << outputHandler << endl;
		
		outString.append(outputHandler.substr(0,2));
		outString.append(" ");
		outString.append(outputHandler.substr(2,2));
		outString.append(" ");
		outString.append(outputHandler.substr(4,2));
		outString.append(" ");
		outString.append(outputHandler.substr(6,2));
		outString.append(" ");	
	
		stringstream dd;
		dd << hex << inD;
		outputHandler = "";
		dd >> outputHandler;
		if(inD == 0x0FFFFFFF&inD){
			outputHandler.insert(0,1,'0');
		}
		cout << "inD: " << outputHandler << endl;		

		outString.append(outputHandler.substr(0,2));
		outString.append(" ");
		outString.append(outputHandler.substr(2,2));
		outString.append(" ");
		outString.append(outputHandler.substr(4,2));
		outString.append(" ");
		outString.append(outputHandler.substr(6,2));
		cout << outString << endl;
		outFile << outString << endl;
		outFile.close();
	}
	


	return 0;
}
