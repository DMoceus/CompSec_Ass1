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
unsigned long rightShift(unsigned long a, unsigned long b);
unsigned long leftShift(unsigned long a, unsigned long b);
unsigned long multMod(unsigned long a,unsigned  long b);
unsigned long subMod(unsigned long a,unsigned  long b);
unsigned long addMod(unsigned long a,unsigned  long b);
unsigned long lilEndian32Swap(unsigned long input);
void keySchedule(string inKey);

unsigned long* S;
unsigned long* L;
unsigned long inA;
unsigned long inB;
unsigned long inC;
unsigned long inD;

void scanInput(string input){
	inA = stoul(input.substr(0, 8),NULL,16);
	inB = stoul(input.substr(8, 8),NULL,16);
	inC = stoul(input.substr(16,8),NULL,16);
	inD = stoul(input.substr(24,8),NULL,16);
}

void decrypt(){
	unsigned int t,u;
	unsigned long temp1, temp2, temp3, temp4;
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
	unsigned int t,u;
	unsigned long temp1, temp2, temp3, temp4;
	inB = addMod(inB,S[0]);
	inD = addMod(inD,S[1]);
	for(int i=1;i<(ROUND_COUNT+1);i++){
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

unsigned long lilEndian32Swap(unsigned  long input){
	unsigned long inBlock1 = (input >> 24) &  0x000000FF;
	unsigned long inBlock2 = (input >> 8)  &  0x0000FF00;
	unsigned long inBlock3 = (input << 8)  &  0x00FF0000;
	unsigned long inBlock4 = (input << 24) &  0xFF000000;
	return inBlock1 | inBlock2 | inBlock3 | inBlock4;
}

void keySchedule(string inKey){
	unsigned int inKeyBytes = inKey.length()/2;
	if(inKeyBytes == 16){
		L = (unsigned long*)malloc(4*32);
/*		L[0] = stoul(inKey.substr(0,8),NULL,16);
		L[1] = stoul(inKey.substr(8,8),NULL,16);
		L[2] = stoul(inKey.substr(16,8),NULL,16);
		L[3] = stoul(inKey.substr(24,8),NULL,16);
*/
		L[0] = lilEndian32Swap(stoul(inKey.substr(0,8), NULL,16));
		L[1] = lilEndian32Swap(stoul(inKey.substr(8,8), NULL,16));
		L[2] = lilEndian32Swap(stoul(inKey.substr(16,8),NULL,16));
		L[3] = lilEndian32Swap(stoul(inKey.substr(24,8),NULL,16));
cout << "L[0]: " << hex << L[0] << endl;
cout << "L[1]: " << hex << L[1] << endl;
cout << "L[2]: " << hex << L[2] << endl;
cout << "L[3]: " << hex << L[3] << endl;
	}
	else if(inKeyBytes == 24){
		L = (unsigned long*)malloc(6*32);
/*		L[0] = stoul(inKey.substr(0,8),NULL,16);
		L[1] = stoul(inKey.substr(8,8),NULL,16);
		L[2] = stoul(inKey.substr(16,8),NULL,16);
		L[3] = stoul(inKey.substr(24,8),NULL,16);
		L[4] = stoul(inKey.substr(32,8),NULL,16);
		L[5] = stoul(inKey.substr(40,8),NULL,16);
*/
		L[0] = lilEndian32Swap(stoul(inKey.substr(0,8), NULL,16));
		L[1] = lilEndian32Swap(stoul(inKey.substr(8,8), NULL,16));
		L[2] = lilEndian32Swap(stoul(inKey.substr(16,8),NULL,16));
		L[3] = lilEndian32Swap(stoul(inKey.substr(24,8),NULL,16));
		L[4] = lilEndian32Swap(stoul(inKey.substr(32,8),NULL,16));
		L[5] = lilEndian32Swap(stoul(inKey.substr(40,8),NULL,16));

cout << "L[0]: " << hex << L[0] << endl;
cout << "L[1]: " << hex << L[1] << endl;
cout << "L[2]: " << hex << L[2] << endl;
cout << "L[3]: " << hex << L[3] << endl;
cout << "L[4]: " << hex << L[4] << endl;
cout << "L[5]: " << hex << L[5] << endl;
	}
	else if(inKeyBytes == 32){
		L = (unsigned long*)malloc(8*32);
/*		L[0] = stoul(inKey.substr(0,8),NULL,16);
		L[1] = stoul(inKey.substr(8,8),NULL,16);
		L[2] = stoul(inKey.substr(16,8),NULL,16);
		L[3] = stoul(inKey.substr(24,8),NULL,16);
		L[4] = stoul(inKey.substr(32,8),NULL,16);
		L[5] = stoul(inKey.substr(40,8),NULL,16);
		L[6] = stoul(inKey.substr(48,8),NULL,16);
		L[7] = stoul(inKey.substr(56,8),NULL,16);
*/
		L[0] = lilEndian32Swap(stoul(inKey.substr(0,8), NULL,16));
		L[1] = lilEndian32Swap(stoul(inKey.substr(8,8), NULL,16));
		L[2] = lilEndian32Swap(stoul(inKey.substr(16,8),NULL,16));
		L[3] = lilEndian32Swap(stoul(inKey.substr(24,8),NULL,16));
		L[4] = lilEndian32Swap(stoul(inKey.substr(32,8),NULL,16));
		L[5] = lilEndian32Swap(stoul(inKey.substr(40,8),NULL,16));
		L[6] = lilEndian32Swap(stoul(inKey.substr(48,8),NULL,16));
		L[7] = lilEndian32Swap(stoul(inKey.substr(56,8),NULL,16));

cout << "L[0]: " << hex << L[0] << endl;
cout << "L[1]: " << hex << L[1] << endl;
cout << "L[2]: " << hex << L[2] << endl;
cout << "L[3]: " << hex << L[3] << endl;
cout << "L[4]: " << hex << L[4] << endl;
cout << "L[5]: " << hex << L[5] << endl;
cout << "L[6]: " << hex << L[6] << endl;
cout << "L[7]: " << hex << L[7] << endl;
	}
	else{
		cout << "ERROR WITH KEYS\n";
		exit(-1);
	}
	S = (unsigned long*)malloc(WORD_SIZE*((2*ROUND_COUNT)+4));
	S[0] = P32;
	unsigned int i,j,v;
	unsigned long A,B;
	for(i=1;i<((2*ROUND_COUNT)+4);i++){
		S[i] = addMod(S[i-1],Q32);
	}
	A = B = i = j = 0;
	if((inKeyBytes/4) >= ((2*ROUND_COUNT)+4)){
		v = 3*(inKeyBytes/4);
	}
	else{
		v = 3*((2*ROUND_COUNT)+4);
	}
//	v = 3*max((int)(inKeyBytes/4),((2*ROUND_COUNT)+4));
	for(int s=1; s<(v+1); s++){
		A = S[i] = leftShift(addMod(addMod(S[i],A),B),3);
		B = L[j] = leftShift(addMod(addMod(L[j],A),B),addMod(A,B));
		i = addMod(i,1)%((2*ROUND_COUNT)+4);
		j = addMod(j,1)%(inKeyBytes/4);
	}
}

unsigned long addMod(unsigned long a, unsigned long b){
	return (a+b)%4294967296;//(long)pow(2,WORD_SIZE);
}

unsigned long subMod(unsigned long a, unsigned long b){
	return (a-b)%4294967296;//(long)pow(2,WORD_SIZE);
}

unsigned long multMod(unsigned long a, unsigned long b){
	return (a*b)%4294967296;//(long)pow(2,WORD_SIZE);
}

 unsigned long leftShift(unsigned long a, unsigned long b){
	unsigned long long aLong = a;
	unsigned int numBits = log2(WORD_SIZE);
	unsigned int shiftCountMask = 0b0;
	for(int i=0; i<numBits; i++){
		shiftCountMask = (shiftCountMask << 1);
		shiftCountMask++;
	}	
	unsigned int shiftCount = b & shiftCountMask;
	
	unsigned long int maskA = 0b0;
	unsigned long int maskB = 0b0;

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

 unsigned long rightShift(unsigned long a, unsigned long b){
	unsigned long long aLong = a;
	unsigned int numBits = log2(WORD_SIZE);
	unsigned int shiftCountMask = 0b0;
	for(int i=0; i<numBits; i++){
		shiftCountMask = (shiftCountMask << 1);
		shiftCountMask++;
	}
	unsigned int shiftCount = b & shiftCountMask;
	unsigned long maskA = 0b0;
	unsigned long maskB = 0b0;
	for(int i=0; i<(WORD_SIZE-shiftCount); i++){
		maskA = (maskA << 1);
		maskA++;
	}
	for(int i=0; i<shiftCount; i++){
		maskB = (maskB << 1);
		maskB++;
	}
	
	unsigned long remainder = (aLong >> shiftCount) & maskA;
	unsigned long cutoff = (aLong & maskB);
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

		inA = lilEndian32Swap(inA);
		inB = lilEndian32Swap(inB);
		inC = lilEndian32Swap(inC);
		inD = lilEndian32Swap(inC);
cout << "word1: " << hex << inA << endl;		
cout << "word2: " << hex << inB << endl;		
cout << "word3: " << hex << inC << endl;		
cout << "word4: " << hex << inD << endl;		
//		cout << "input: " << hex << inA << hex << inB << hex << inC << hex << inD << endl;
		cout << "key as string: " << inString << endl;
		keySchedule(inString);
		encrypt();
		inFile.close();
		inA = lilEndian32Swap(inA);
		inB = lilEndian32Swap(inB);
		inC = lilEndian32Swap(inC);
		inD = lilEndian32Swap(inC);
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
		
		inA = lilEndian32Swap(inA);
		inB = lilEndian32Swap(inB);
		inC = lilEndian32Swap(inC);
		inD = lilEndian32Swap(inC);
		keySchedule(inString);
		decrypt();
		inFile.close();
		
		inA = lilEndian32Swap(inA);
		inB = lilEndian32Swap(inB);
		inC = lilEndian32Swap(inC);
		inD = lilEndian32Swap(inC);
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
