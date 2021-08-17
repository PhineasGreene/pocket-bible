#include <stdio.h>

#define LEN 256

int chars[LEN], total;

void main(){
	FILE* bible;
	bible = fopen("kjv.bible", "r");

	total = 0;
	for(int i = 0; i < LEN; i++) chars[i] = 0;

	char c;
	int pos=0;

	while((c = getc(bible)) != EOF){
		if(chars[c] == 0){
			total++;
		}
		if(c == 0) printf("pos %d\n", pos);
		chars[c]++;
		pos++;
	}
		printf("pos %d\n", pos);

	fclose(bible);

	for(int i = 0; i < LEN; i++){
		if(chars[i]){
			if(i == 10) printf("10 (\\n): %d\n", chars[i]);
			else printf("%d (%c): %d\n", i, i, chars[i]);
		}
	}

	printf("Unique characters: %d\n", total);
}
