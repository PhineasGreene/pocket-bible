#include <stdio.h>

int main(int argc, char** argv){
	FILE* input;

	if(argc < 2){
		printf("Not enough arguments.\n");
		return 1;
	}
	
	if((input = fopen(argv[1], "r")) == NULL){
		printf("Problem opening file %s for reading.\n", argv[1]);
		return 1;
	}

	unsigned char c;
	unsigned char bit1mask = 128;
	unsigned char chunk = 0;
	int bit = 0;

	while((c = getc(input)) != EOF){
		for(int i = 0; i < 8; i++){
			if(bit == 5){
				printf("%d\n", chunk);
				bit = 0;
				chunk = 0;
			}
			chunk = chunk << 1;
			chunk += ((c & bit1mask) >> 7);
			c = c << 1;
			bit++;
		}
	}

	fclose(input);

	return 0;
}
