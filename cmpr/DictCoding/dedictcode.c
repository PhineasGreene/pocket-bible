#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_WORD 32

void decFile(FILE* input, FILE* output, char** wl, int wllen);
char* decWord(int code);
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

	unsigned char byte = getc(input);
	int pos = 0;
	int w = readWord(input, wordSize(length), &byte, &pos);

	printf("%d", w);
	printf(" %s\n", wordList[w]);
	w = readWord(input, wordSize(length), &byte, &pos);
	printf("%d", w);
	printf(" %s\n", wordList[w]);
	w = readWord(input, wordSize(length), &byte, &pos);
	printf("%d", w);
	printf(" %s\n", wordList[w]);

	fclose(input);
	fclose(output);

	return 0;
}

int readWord(FILE* file, int size, unsigned char* byte, int* pos){
	char mask = 1 << 7;
	int word = 0;

	for(int i = 0; i <= size; i++){
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


bool isSingleCharWord(char c){
	if(c >= 48 && c <= 57) return false; // digits
	if(c >= 65 && c <= 90) return false; // caps
	if(c >= 97 && c <= 122) return false; // smalls
	if(c == 39) return false; // apostrophe
	if(c <= 32) return false; // space, newline, control

	return true;
}
