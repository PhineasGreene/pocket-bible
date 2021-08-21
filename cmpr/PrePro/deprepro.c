#include <stdio.h>

int main(int argc, char** argv){
	FILE* out;
	FILE* in;

	if(argc < 3){
		printf("Not enough arguments.\n");
		return 1;
	}
	
	if((out = fopen(argv[2], "w")) == NULL){
		printf("Problem opening file %s for writing.", argv[2]);
		return 1;
	}

	if((in = fopen(argv[1], "r")) == NULL){
		printf("Problem opening file %s for reading.", argv[1]);
		return 1;
	}

	char c;
	int verse = 1;
	int chapter = 0;

	while((c = getc(in)) != EOF){
		if(c == '@'){
			/* Verse start. */
			verse++;
			fprintf(out, "\n%d:%d ", chapter, verse);
		}else if(c == '#'){
			/* Chapter start. */
			chapter++;
			verse = 1;
			fprintf(out, "\n%d:%d ", chapter, verse);
		}else if(c == '$'){
			/* Book start. */
			fprintf(out, "\n");
			chapter = 0;
		}else{
			putc(c, out);
		}
	}

	fclose(in);
	fclose(out);
}
