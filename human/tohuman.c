#include <stdio.h>

int main(int argc, char** argv){
	FILE* input;
	FILE* output;

	if(argc < 3){
		printf("Not enough arguments.\n");
		return 1;
	}
	
	if((input = fopen(argv[1], "r")) == NULL){
		printf("Problem opening file %s for reading.\n", argv[1]);
		return 1;
	}

	if((output = fopen(argv[2], "w")) == NULL){
		printf("Problem opening file %s for writing.\n", argv[2]);
		return 1;
	}

	unsigned char enc[64] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*?";
	unsigned char c;
	unsigned char bit1mask = 128;
	unsigned char chunk = 0;
	int bit = 0;
	int charcount = 0;
	int chunksize = 6;
	int filesize = 2204780;
	int bytecount = 0;

	while(1){
		c = getc(input);
		bytecount++;

		if(bytecount >= filesize) c = 0;

		for(int i = 0; i < 8; i++){
			if(bit == chunksize){
				putc(enc[chunk], output);

				bit = 0;
				chunk = 0;

				charcount++;
				if(charcount % 70 == 0){
					putc(10, output);
				}
			}
			chunk = chunk << 1;
			chunk += ((c & bit1mask) >> 7);
			c = c << 1;
			bit++;
		}

		if(bytecount >= filesize) break;
	}

	fclose(input);
	fclose(output);

	return 0;
}
