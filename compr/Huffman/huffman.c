#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_INT 2147483647

struct Tree {
	int n;
	char c; // If branch, null.
	struct Tree* left;
	struct Tree* right;
};

struct HuffChar {
	char* bits; // A string of the characters '1' and '0'
	int l; // Length of bits.
};

/* How are you meant to format these nicely? */

/* Return true Tree a is identical to Tree b. */
bool treeEq(struct Tree a, struct Tree b);
/* Print a tree to stdout. Format: {left, right} */
void printTree(struct Tree n);
/* Sort a list of trees from highest n to lowest. */
void sort(struct Tree* tree, int l);
/* Find a tree in a list and return its pointer. */
struct Tree* findTree(struct Tree target, struct Tree* trees, int l);
//int treeDepth(struct Tree tree);
/* Create a huffman tree from a file, using treeData
 * to store nodes and leaves. */
struct Tree growHuffman(struct Tree* treeData, FILE* file);
/* Populate list trees with leaves for every value
 * of tree.c. Tree.n is the number of times that
 * character occurs in the file. */
void count(struct Tree* trees, FILE* file);
/* Create a lookup table for characters to find
 * their huffman encoding. */
void huffTable(struct HuffChar* table, struct Tree tree, char* bits, int len);

int main(int argc, char** argv){
	FILE* bible;

	if(argc < 2){
		printf("Not enough arguments.\n");
		return 1;
	}
	
	if((bible = fopen(argv[1], "r")) == NULL){
		printf("Problem opening file %s for reading.", argv[1]);
		return 1;
	}

	struct Tree treeData[256*2];
	struct HuffChar table[256];

	for(int i = 0; i < 256; i++){
		struct HuffChar hc;
		hc.l = 0;
		table[i] = hc;
	}

	huffTable(table, growHuffman(treeData, bible), "", 0);

	for(int i = 0; i < 256; i++){
		if(table[i].l) printf("%c: %s %d\n", i, table[i].bits, table[i].l);
	}
}

bool treeEq(struct Tree a, struct Tree b){
	return a.n == b.n && a.c == b.c && a.left == b.left && a.right == b.right;
}

void printTree(struct Tree n){
	if(n.c != '\0') printf("'%c'", n.c);
	else if(n.left != NULL && n.right != NULL){
		printf("{");
		printTree(*n.left);
		printf(", ");
		printTree(*n.right);
		printf("}");
	}
}

void sort(struct Tree* tree, int l){
	/* Only used to generate the table, so a
	 * bubble sort gets the job done. */

	int done = 0;
	
	while(!done){
		done = 1;

		struct Tree last;
		last.n = MAX_INT;

		for(int i = 0; i < l; i++){
			if(tree[i].n > last.n){
				tree[i - 1] = tree[i];
				tree[i] = last;
				done = 0;
			}

			last = tree[i];
		}
	}
}

struct Tree* findTree(struct Tree target, struct Tree* trees, int l){
	for(int i = 0; i < l; i++){
		if(treeEq(target, trees[i])) return &trees[i];
	}

	return NULL;
}

/*
int treeDepth(struct Tree tree){
	if(tree.c != '\0') return 1;
	else{
		int leftD = tree.left != NULL ? treeDepth(*tree.left) : 0;
		int rightD = tree.right != NULL ? treeDepth(*tree.right) : 0;
		return 1 + (leftD > rightD ? leftD : rightD);
	}
}*/

struct Tree growHuffman(struct Tree* treeData, FILE* file){
	int dataIndex = 256;

	count(treeData, file);

	struct Tree tree[256];
	int len = 0;

	for(int i = 0; i < 256; i++){
		if(treeData[i].n > 0){
			tree[len] = treeData[i];
			len++;
		}
	}

	sort(tree, len);

	while(len > 1){
		struct Tree t;
		t.n = tree[len - 1].n + tree[len - 2].n;
		t.c = '\0';
		t.left = findTree(tree[len - 2], treeData, dataIndex);
		t.right = findTree(tree[len - 1], treeData, dataIndex);

		treeData[dataIndex] = t;
		tree[len - 2] = t;

		dataIndex++;
		len--;

		sort(tree, len);
	}

	return tree[0];
}

void count(struct Tree* trees, FILE* file){
	int counts[256];
	for(int i = 0; i < 256; i++) counts[i] = 0;

	char c;
	while((c = getc(file)) != EOF){
		counts[c]++;
	}

	int fi = 0;

	for(int i = 0; i < 256; i++){
		struct Tree n;
		n.n = counts[i];
		n.c = i;
		n.left = NULL;
		n.right = NULL;
		trees[fi] = n;
		fi++;
	}
}

void huffTable(struct HuffChar* table, struct Tree tree, char* bits, int len){
	if(tree.c != '\0'){
		table[tree.c].l = len;
		table[tree.c].bits = (char *) malloc(len);
		strcpy(table[tree.c].bits, bits);
		table[tree.c].bits[len] = '\0';
	}else{
		char b[len + 1];
		strcpy(b, bits);

		b[len] = '0';
		huffTable(table, *tree.left, b, len + 1);
		b[len] = '1';
		huffTable(table, *tree.right, b, len + 1);
	}
}
