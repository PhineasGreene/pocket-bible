#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_WORD 32

struct entry {
	char word[MAX_WORD];
	struct entry* next;
};

struct dict {
	struct entry* head;
	long int len;
};

// encode a file from a word list
void encFile(char** wl, int wllen, FILE* input, FILE* output);
// write a file heading including the word list and encoded word size
void writeHeader(char** wl, int wllen, FILE* output);
// return the index of a word in a word list
unsigned int encWord(char** wl, int wllen, char* word);
// write a word [length] bits to [file], containing the value [value],
// using [byte] as a buffer, with [pos] being the current position in [byte]
void writeWord(unsigned int value, int length, char* byte, int* pos, FILE* file);
// calculate the minimum number of bits needed to encode n words
int minBits(int words); 
// create an array from a dictionary
void buildWordList(char** wl, struct dict d); 
// free dictionary
void destroyDict(struct dict* d);
// fill a dictionary with the unique words in file
void buildDict(struct dict* d, FILE* file); 
// is [word] in dictionary
bool inDict(struct dict d, char* word);
// append a word to a dictionary
void appendWord(struct dict* d, char* word); 
void printDict(struct dict d);
// letters and digits
bool isWordChar(char c); 
// strictly word terminators (newline and space)
bool isWordTerminator(char c); 
// remaining characters (punctuation and misc)
bool isSingleCharWord(char c); 

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

	// populate a dictionary based on input
	struct dict dictionary;
	dictionary.head = NULL;
	dictionary.len = 0;
	buildDict(&dictionary, input);

	// populate a word list array based on dictionary
	int length = dictionary.len;
	char* wordList[length + 1];
	buildWordList(wordList, dictionary);

	// free the memory we allocated for dictionary
	destroyDict(&dictionary);

	rewind(input);
	encFile(wordList, length, input, output);

	fclose(input);
	fclose(output);

	return 0;
}

void encFile(char** wl, int wllen, FILE* input, FILE* output){
	writeHeader(wl, wllen, output);

	char c;
	char wordBuff[MAX_WORD];
	int wordIndex = 0;

	int size = minBits(wllen);

	char byte;
	int pos = 0;

	while((c = getc(input)) != EOF){
		if(isWordChar(c)){
			// add char to word buffer
			wordBuff[wordIndex] = c;
			wordBuff[wordIndex + 1] = '\0';
			wordIndex++;
		}else if(isWordTerminator(c)){
			// add word to dictionary
			if(wordIndex > 0){
				unsigned int code = encWord(wl, wllen, wordBuff);
				writeWord(code, size, &byte, &pos, output);
				wordIndex = 0;
				wordBuff[0] = '\0';
			}
		}else if(isSingleCharWord(c)){
			// add word buffer word and single char word
			if(wordIndex > 0){
				unsigned int code = encWord(wl, wllen, wordBuff);
				writeWord(code, size, &byte, &pos, output);
			}

			wordBuff[0] = c;
			wordBuff[1] = '\0';

			unsigned int code = encWord(wl, wllen, wordBuff);
			writeWord(code, size, &byte, &pos, output);

			wordIndex = 0;
			wordBuff[0] = '\0';
		}
	}

	writeWord(wllen, size, &byte, &pos, output);
	putc(byte, output);
}

void writeHeader(char** wl, int wllen, FILE* output){
	fprintf(output, "%d\n", wllen);

	for(int i = 0; i < wllen; i++){
		fprintf(output, "%s ", wl[i]);
	}

	putc('\n', output);
}

void writeWord(unsigned int value, int length, char* byte, int* pos, FILE* file){
	int intBits = sizeof(int) * 8;
	unsigned int mask = 1 << (intBits - 1);

	value <<= intBits - length;

	for(int i = 0; i < length; i++){
		*byte <<= 1;
		*pos += 1;
		*byte += (value & mask) >> (intBits - 1);
		value <<= 1;

		if(*pos == 8){
			putc(*byte, file);
			*pos = 0;
			*byte = 0;
		}
	}
}

unsigned int encWord(char** wl, int wllen, char* word){
	for(int i = 0; i < wllen; i++){
		if(strcmp(wl[i], word) == 0) return i;
	}

	printf("Fatal: Word \"%s\" not present in word list.\n", word);
	exit(1);
}

int minBits(int words){
	int n = 2;
	int b = 1;

	while(n < words){
		b++;
		n *= 2;
	}

	return b;
}

void buildWordList(char** wl, struct dict d){
	struct entry* e = d.head;
	int i = 0;

	while(e != NULL){
		wl[i] = malloc(MAX_WORD);
		strcpy(wl[i], e->word);

		i++;
		
		e = e->next;
	}
}

void destroyDict(struct dict* d){
	struct entry* e = d->head;

	while(e != NULL){
		struct entry* n = e->next;

		free(e);
		
		e = n;
	}

	d->head = NULL;
	d->len = 0;
}

void buildDict(struct dict* d, FILE* file){
	char c;
	char wordBuff[MAX_WORD];
	int wordIndex = 0;

	while((c = getc(file)) != EOF){
		if(isWordChar(c)){
			// add char to word buffer
			wordBuff[wordIndex] = c;
			wordBuff[wordIndex + 1] = '\0';
			wordIndex++;
		}else if(isWordTerminator(c)){
			// add word to dictionary
			if(wordIndex > 0){
				appendWord(d, wordBuff);
				wordIndex = 0;
				wordBuff[0] = '\0';
			}
		}else if(isSingleCharWord(c)){
			// add word buffer word and single char word
			if(wordIndex > 0){
				appendWord(d, wordBuff);
			}

			wordBuff[0] = c;
			wordBuff[1] = '\0';
			appendWord(d, wordBuff);

			wordIndex = 0;
			wordBuff[0] = '\0';
		}
	}
}

bool inDict(struct dict d, char* word){
	struct entry* e = d.head;

	while(e != NULL){
		if(strcmp(e->word, word) == 0) return true;
		e = e->next;
	}

	return false;
}

void appendWord(struct dict* d, char* word){
	if(inDict(*d, word)) return; // skip words already in the dictionary

	struct entry* e = malloc(sizeof (struct entry));

	strcpy(e->word, word);

	e->next = d->head;
	d->head = e;
	d->len++;
}

void printDict(struct dict d){
	struct entry* e = d.head;

	while(e != NULL){
		printf("%s\n", e->word);
		e = e->next;
	}
}

bool isWordChar(char c){
	if(c >= 48 && c <= 57) return true; // digits
	if(c >= 65 && c <= 90) return true; // caps
	if(c >= 97 && c <= 122) return true; // smalls
	if(c == 39) return true; // apostrophe

	return false;
}

bool isWordTerminator(char c){
	if(c <= 32) return true;
	return false;
}

bool isSingleCharWord(char c){
	if(!isWordTerminator(c) && !isWordChar(c)) return true;
	return false;
}
