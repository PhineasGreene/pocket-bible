/* Get information about the characters (ASCII) present
 * in a file. Usage: data [ASCII file]. */

#include <stdio.h>

#define LEN 256

int chars[LEN], total, unique;

void main(int argc, char **argv){
	FILE* bible;
	bible = fopen(argv[1], "r");

	total = 0;
	unique = 0;
	for(int i = 0; i < LEN; i++) chars[i] = 0;

	char c;

	while((c = getc(bible)) != EOF){
		if(chars[c] == 0){
			unique++;
		}
		total++;
		chars[c]++;
	}

	fclose(bible);

	for(int i = 0; i < LEN; i++){
		if(chars[i]){
			if(i == 10) printf("10 (\\n): %d\n", chars[i]);
			else printf("%d (%c): %d\n", i, i, chars[i]);
		}
	}

	printf("Unique characters: %d\n", unique);
	printf("Total characters: %d\n", total);
}
