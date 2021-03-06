/*
 * DeSimple 6 Bit
 *
 * A 6 bit character and chapter/verse decoder
 * for the King James Bible.
 *
 * Usage: simple6bit [6 bit input file] [ASCII output file]
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

#define LEN 64

char enc[LEN];

void initEnc(){
	/* Map characters to their ASCII values. */
	enc[1] = ' ';
	enc[2] = '!';
	enc[3] = 39; // "'"
	enc[4] = '(';
	enc[5] = ')';
	enc[6] = ',';
	enc[7] = '-';
	enc[8] = '.';
	enc[9] = ':';
	enc[10] = ';';
	enc[11] = '?';
	// A-Z
	for(int i = 65; i <= 90; i++) enc[i - 53] = i;
	// a-z
	for(int i = 97; i <= 122; i++) enc[i - 59] = i;
}

void decodeChar(char* out, char* c, int* bit, FILE* file){
	/* Read from current character pulled from stream
	 * from bit "bit" to the end of the byte. Then (if
	 * needed) get a new byte, finish reading the 6 bit
	 * character, and update "bit".
	*/
	*out = 0;
	switch(*bit){
		case 0:
			*c = getc(file);
			*out += *c >> 2;
			*bit = 6;
			break;
		case 6:
			*out += (*c << 6) >> 2;
			*c = getc(file);
			*out += (*c >> 4) & 15;
			*bit = 4;
			break;
		case 4:
			*out += (*c << 4) >> 2;
			*c = getc(file);
			*out += (*c >> 6) & 3;
			*bit = 2;
			break;
		case 2:
			*out = (*c << 2) >> 2;
			*bit = 0;
			break;
	}
	*out = *out & 63;
}

void main(int argc, char** argv){
	FILE* bible;
	FILE* compr;

	if((bible = fopen(argv[2], "w")) == NULL){
		printf("Can't open file %s for writing.\n", argv[2]);
		return;
	}
	if((compr = fopen(argv[1], "r")) == NULL){
		printf("Can't open file %s for reading.\n", argv[1]);
		return;
	}

	initEnc();

	int bit = 0;
	int cvc = 0; // chapter/verse character counter
	int chapter = 0;
	int verse = 1;
	char out; // output character
	char c; // byte read from stream

	while(1){
		decodeChar(&out, &c, &bit, compr);

		if(out == 0){
			cvc++;
			if(cvc == 4) break; // EOF
			continue;
		}else if(cvc){
			switch(cvc){
				case 1: // verse
					verse++;
					fprintf(bible, "\n%d:%d ", chapter, verse);
					break;
				case 2: // chapter
					chapter++;
					verse = 1;
					fprintf(bible, "\n%d:%d ", chapter, verse);
					break;
				case 3: // book
					fprintf(bible, "\n");
					chapter = 0;
					break;
			}
			cvc = 0;
		}
		putc(enc[out], bible);
	}

	putc('\n', bible);

	fclose(bible);
	fclose(compr);
}
