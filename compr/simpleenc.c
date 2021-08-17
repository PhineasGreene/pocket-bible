#include <stdio.h>

/* Dec Name ASCII
 * 0  New Verse
 * 1  Space       (32)
 * 2  Bang        (33)
 * 3  Apostrophe  (39)
 * 4  Open Paren  (40)
 * 5  Close Paren (41)
 * 6  Comma       (44)
 * 7  Hyphen      (45)
 * 8  Period      (46)
 * 9  Colon       (58)
 * 10 Semicolon   (59)
 * 11 Question    (63)
 * 12 A           (65)
 * 37 Z           (90)
 * 38 a           (97)
 * 63 z           (122)
 */

#define LEN 256
char enc[LEN];

void initEnc(){
	/* Every used character in enc (indexed by ascii value)
	 * recieves its new encoding. */
	enc[32] = 1; // " "
	enc[33] = 2; // "!"
	enc[39] = 3; // "'"
	enc[40] = 4; // "("
	enc[41] = 5; // ")"
	enc[44] = 6; // ","
	enc[45] = 7; // "-"
	enc[46] = 8; // "."
	enc[58] = 9; // ":"
	enc[59] = 10; // ";"
	enc[63] = 11; // "?"
	// A-Z
	for(int i = 65; i <= 90; i++){
		enc[i] = i - 53;
	}
	// a-z
	for(int i = 97; i <= 122; i++){
		enc[i] = i - 59;
	}
}

void printBits(char byte){
	for(int i = 7; i >= 0; i--){
		char bit = (byte >> i) & 1;
		printf("%d", bit);
	}
	printf("\n");
	printf("%d\n", byte);
}

void main(int argc, char** argv){
	FILE* bible;
	FILE* compr;
	bible = fopen(argv[1], "r");
	compr = fopen(argv[2], "w");

	initEnc();

	char c, buff[2];
	int bit = 0;

	while((c = getc(bible)) != EOF){

	}

	fclose(bible);
	fclose(compr);
}
