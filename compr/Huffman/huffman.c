#include "tree.h"

void writeHeader(struct HuffChar* table, FILE* out);

void printBits(char c){
	for(int i = 0; i < 8; i++){
		char mask = 1 << (7 - i);
		char bit = c & mask;
		printf(bit ? "1" : "0");
	}
	printf("\n");
}

char setBit(char c, int bit, int v);

void writeHuffChar(struct HuffChar c, int* bit, char* byte,  FILE* file);

int main(int argc, char** argv){
	FILE* bible;
	FILE* out;

	if(argc < 3){
		printf("Not enough arguments.\n");
		return 1;
	}
	
	if((bible = fopen(argv[1], "r")) == NULL){
		printf("Problem opening file %s for reading.", argv[1]);
		return 1;
	}

	if((out = fopen(argv[2], "w")) == NULL){
		printf("Problem opening file %s for writing.", argv[2]);
		return 1;
	}

	struct Tree treeData[256*2];
	struct Tree huffman = growHuffman(treeData, bible);
	struct HuffChar table[256];

	for(int i = 0; i < 256; i++){
		struct HuffChar hc;
		hc.l = 0;
		table[i] = hc;
	}

	huffTable(table, huffman, "", 0);

	writeHeader(table, out);

	char c;
	int bit;
	char byte = 0;

	rewind(bible);

	while((c = getc(bible)) != EOF){
		writeHuffChar(table[c], &bit, &byte, out);	
	}
	//
	// EOF
	writeHuffChar(table[4], &bit, &byte, out);
	putc(byte, out);

	fclose(bible);
	fclose(out);
}

void writeHeader(struct HuffChar* table, FILE* out){
	for(int i = 0; i < 256; i++){
		if(table[i].l > 0){
			fprintf(out, "%c%s", i, table[i].bits);
		}
	}

	putc(3, out);
}

char setBit(char c, int bit, int v){
	char mask = 1 << (7 - bit);
	return v ? c | mask : c & (~mask);
}

void writeHuffChar(struct HuffChar c, int* bit, char* byte, FILE* file){
	int b = *bit;

	for(int i = 0; i < c.l; i++){
		if(b >= 8){
			putc(*byte, file);
			b = 0;
			*byte = 0;
		}
		*byte = setBit(*byte, b, c.bits[i] == '0' ? 0 : 1);
		b++;
	}

	*bit = b;
}
