/* Usefull for debugging. */

void printBits(char c){
	for(int i = 0; i < 8; i++){
		char mask = 1 << (7 - i);
		char bit = c & mask;
		printf(bit ? "1" : "0");
	}
}
