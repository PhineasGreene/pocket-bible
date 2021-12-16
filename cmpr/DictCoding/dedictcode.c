#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_WORD 32

void decFile(FILE* input, FILE* output, char** wl, int wllen);
int readWord(FILE* file, int size, unsigned char* byte, int* pos);
void readWordList(FILE* file, char** wl, int wllen);
int wordSize(int words);
int readWordListLength(FILE* file);

int main(int argc, char** argv){
	FILE* input;
	FILE* output;

	if(argc < 3){
		printf("Usage: dictcode [input file] [output file].\n");
		return 1;
	}

	if((input = fopen(argv[1], "r")) == NULL){
		printf("Can't open file %s for reading.\n", argv[2]);
		return 1;
	}

	if((output = fopen(argv[2], "w")) == NULL){
		printf("Can't open file %s for writing.\n", argv[3]);
		return 1;
	}

	int length = readWordListLength(input);
	char* wordList[length];
	readWordList(input, wordList, length);

	decFile(input, output, wordList, length);

	fclose(input);
	fclose(output);

	return 0;
}

void decFile(FILE* input, FILE* output, char** wl, int wllen){
	unsigned char byte = getc(input);
	int pos = 0;
	int size = wordSize(wllen);
	int word = 0;

	while((word = readWord(input, size, &byte, &pos)) != wllen){
		if(word < 0 || word > wllen){
			printf("Bad word.\n");
			continue;
		}
		// put a space for words containing letters
		if((wl[word][0] >= 65 && wl[word][0] <= 90) ||
		   (wl[word][0] >= 97 && wl[word][0] <= 122)){
			putc(' ', output);
		}
		// write word to output
		fprintf(output, "%s", wl[word]);
	}
}

int readWord(FILE* file, int size, unsigned char* byte, int* pos){
	char mask = 1 << 7;
	int word = 0;

	for(int i = 0; i < size; i++){
		word <<= 1;
		word += ((*byte & mask) ? 1 : 0);
		
		*pos += 1;
		*byte <<= 1;

		if(*pos == 8){
			*byte = getc(file);
			*pos = 0;
		}
	}

	return word;
}

void readWordList(FILE* file, char** wl, int wllen){
	for(int i = 0; i < wllen; i++){
		wl[i] = malloc(MAX_WORD);
		fscanf(file, "%s ", wl[i]);
	}
}

int readWordListLength(FILE* file){
	int l;
	fscanf(file, "%d\n", &l);
	return l;
}

int wordSize(int words){
	int n = 2;
	int b = 1;

	while(n < words){
		b++;
		n *= 2;
	}

	return b;
}

