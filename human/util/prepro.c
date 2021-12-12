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
	int chap, vers;

	while((c = getc(in)) != EOF){
		if(c == '\n'){
			if(fscanf(in, "%d:%d ", &chap, &vers) == 2){
				if(vers == 1){
					/* Chapter start. */
					putc('#', out);
				}else{
					/* Verse start. */
					putc('@', out);
				}
			}else{
				/* Book start. */
				putc('$', out);
			}
		}else{
			putc(c, out);
		}
	}

	fclose(in);
	fclose(out);
}
