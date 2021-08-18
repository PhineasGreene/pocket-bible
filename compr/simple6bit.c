/*
 * Simple 6 Bit
 *
 * A 6 bit character and chapter/verse encoding
 * for the King James Bible.
 *
 * Usage: simple6bit [ASCII input file] [output file]
 *
 * Encoding
 * Code  Name        ASCII Value
 * 0     New Verse
 * 1     Space       (32)
 * 2     Bang        (33)
 * 3     Apostrophe  (39)
 * 4     Open Paren  (40)
 * 5     Close Paren (41)
 * 6     Comma       (44)
 * 7     Hyphen      (45)
 * 8     Period      (46)
 * 9     Colon       (58)
 * 10    Semicolon   (59)
 * 11    Question    (63)
 * 12    A           (65)
 * 37    Z           (90)
 * 38    a           (97)
 * 63    z           (122)
 *
 * Two new verses = New Chapter
 * Three new verses = New Book
 * Four new verses = EOF
 *
 * There are likey a few bits after the EOF marker,
 * due to our character size being < 1 byte.
*/

#include <stdio.h>

#define LEN 256

char enc[LEN];

void initEnc(){
	/* Give every used character in encoding array "enc"
	 * (indexed by ascii value) its new value. */
	for(int i = 0; i < LEN; i++) enc[i] = 0; // For errors
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
	for(int i = 65; i <= 90; i++) enc[i] = i - 53;
	// a-z
	for(int i = 97; i <= 122; i++) enc[i] = i - 59;
}

void writeChar(char c, char* buff, int* bit, FILE* file){
	/* Write a 6 bit character to a 2 byte buffer
	 * starting at a given bit. If the first byte
	 * is full, write it to a file, set byte 1 to
	 * byte 2, and set byte 2 to 0. Also update the
	 * current bit.
	*/
	switch(*bit){
		case 0:
			buff[0] += c << 2;
			*bit = 6;
			break;
		case 6:
			buff[0] += (c >> 4);
			buff[1] += c << 4;
			putc(buff[0], file);
			buff[0] = buff[1];
			buff[1] = 0;
			*bit = 4;
			break;
		case 4:
			buff[0] += (c >> 2);
			buff[1] += c << 6;
			putc(buff[0], file);
			buff[0] = buff[1];
			buff[1] = 0;
			*bit = 2;
			break;
		case 2:
			buff[0] += c;
			putc(buff[0], file);
			buff[0] = buff[1];
			buff[1] = 0;
			*bit = 0;
			break;
	}
}

void main(int argc, char** argv){
	FILE* bible;
	FILE* compr;

	if((bible = fopen(argv[1], "r")) == NULL){
		printf("Can't open file %s for reading.\n", argv[1]);
		return;
	}
	if((compr = fopen(argv[2], "w")) == NULL){
		printf("Can't open file %s for writing.\n", argv[2]);
		return;
	}

	initEnc();

	char c, buff[2];
	int bit = 0;
	int chap, vers;

	while((c = getc(bible)) != EOF){
		if(c == 10){
			if(fscanf(bible, "%d:%d ", &chap, &vers)){
				/* Verse start. */
				writeChar(0, buff, &bit, compr);
				if(vers == 1){
					/* Chapterstart. */
					writeChar(0, buff, &bit, compr);
				}
			}else{
				/* Book start. */
				writeChar(0, buff, &bit, compr);
				writeChar(0, buff, &bit, compr);
				writeChar(0, buff, &bit, compr);
			}
		}else if(c >= LEN || c < 0 || (c != 0 && !enc[c])){
			printf("Error: Unknown character %d.\n", c);
		}else{
			writeChar(enc[c], buff, &bit, compr);
		}
	}

	/* EOF */
	for(int i = 0; i < 4; i++) writeChar(0, buff, &bit, compr);
	if(bit != 0) putc(buff[0], compr);

	fclose(bible);
	fclose(compr);
}
