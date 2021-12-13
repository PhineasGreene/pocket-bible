/* Usage: huffman [ASCII file] [Compressed file] */

#include "tree.h"

/* Write the encodings for every character used to the
 * top of the output file. */
void writeHeader(struct HuffChar* table, FILE* out);
/* Set bit "bit" of byte "c" to value "v". */
char setBit(char c, int bit, int v);
/* Write huffman character "c" to a file. */
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

	char c;
	int bit;
	char byte = 0;

	rewind(bible);
	
	writeHeader(table, out);

	while((c = getc(bible)) != EOF){
		writeHuffChar(table[c], &bit, &byte, out);	
	}
	//
	writeHuffChar(table[4], &bit, &byte, out);	
	putc(byte, out);

	fclose(bible);
	fclose(out);
}

void writeHeader(struct HuffChar* table, FILE* out){
	for(int i = 0; i < 256; i++){
		if(table[i].l > 0){
			printf("%d, \"%c\" : %s\n", i, i, table[i].bits);
		}
	}
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
