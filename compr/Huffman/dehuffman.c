#include "tree.h"

/* Read a character from the header. */
struct HuffChar readChar(FILE* file);
/* Read the header into a table. */
void readTable(struct HuffChar* table, FILE* file);
/* Turn a huffman character into an ASCII character. */
char huffToChar(struct HuffChar* table, struct HuffChar hc);
/* Get bit b of a byte. Return 0 if 0, or something else if 1. */
int getBit(char c, int b);
/* Decode a huffman file. */
void readContent(FILE* in, FILE* out);

int main(int argc, char** argv){
	FILE* out;
	FILE* in;

	if(argc < 3){
		printf("Not enough arguments.\n");
		return 1;
	}
	
	if((out = fopen(argv[2], "w")) == NULL){
		printf("Problem opening file %s for writing.", argv[2]);
		return 1;
	}

	if((in = fopen(argv[1], "r")) == NULL){
		printf("Problem opening file %s for reading.", argv[1]);
		return 1;
	}

	readContent(in, out);

	fclose(in);
	fclose(out);
}

struct HuffChar readChar(FILE* file){
	int l = 0;
	char c;
	char* buf;
	struct HuffChar hc;

	while(1){
		c  = getc(file);

		if(c != '1' && c != '0'){
			hc.bits[hc.l] = '\0';
			fseek(file, -1, SEEK_CUR);
			return hc;
		}

		hc.l = l + 1;
		hc.bits = (char*) malloc(l + 1);
		strcpy(hc.bits, buf);
		hc.bits[l] = c;
		buf = (char*) malloc(l + 1);
		strcpy(buf, hc.bits);

		l++;
	}
}

void readTable(struct HuffChar* table, FILE* file){
	rewind(file);

	char c;
	int l = 0;

	while((c = getc(file)) != 3){
		if(c == '#'){
			char hcid = getc(file);
			table[hcid] = readChar(file);
		}
	}
}

char huffToChar(struct HuffChar* table, struct HuffChar hc){
	for(int i = 0; i < 256; i++){
		if(table[i].l == hc.l && strcmp(table[i].bits, hc.bits) == 0){
			return (char) i;
		}
	}
	return '\0';
}

int getBit(char c, int b){
	char mask = 1 << (7 - b);
	return c & mask;
}

void readContent(FILE* in, FILE* out){
	struct HuffChar table[256];

	for(int i = 0; i < 256; i++){
		struct HuffChar hc;
		hc.l = 0;
		hc.bits = "\0";
		table[i] = hc;
	}

	readTable(table, in);

	char c = getc(in);

	struct HuffChar hc;
	hc.bits = (char*) malloc(256);
	hc.l = 0;
	int bit = 0;

	while(1){
		if(bit == 8){
			bit = 0;
			c = getc(in);
		}

		hc.l++;
		hc.bits[hc.l - 1] = getBit(c, bit) ? '1' : '0';
		hc.bits[hc.l] = '\0';
		bit++;

		if(hc.l == 256){
			printf("ERROR: Unknown character.\n");
			return;
		}
		
		char outc = huffToChar(table, hc);

		if(outc != '\0'){
			if(outc == 4) return;
			putc(outc, out);
			hc.l = 0;
		}
	}
	printf("\n");
}
