#include <stdbool.h>
#include <stdio.h>

#define MAX_INT 2147483647

struct Tree {
	int n;
	char c;
	struct Tree* left;
	struct Tree* right;
};

/* How are you meant to format these nicely? */
bool treeEq(struct Tree a, struct Tree b);
void printTree(struct Tree n);
void sort(struct Tree* tree, int l);
struct Tree* findTree(struct Tree target, struct Tree* trees, int l);
int treeDepth(struct Tree tree);
struct Tree growHuffman(struct Tree* treeData, FILE* file);
void count(struct Tree* tree, FILE* file);

int main(int argc, char** argv){
	FILE* bible;
	
	if((bible = fopen(argv[1], "r")) == NULL){
		printf("Problem opening file %s for reading.", argv[1]);
	}

	struct Tree treeData[256*2];

	struct Tree huffman = growHuffman(treeData, bible);

	printTree(huffman);
	printf("\n");
	printf("Depth %d\n", treeDepth(huffman));
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

int treeDepth(struct Tree tree){
	if(tree.c != '\0') return 1;
	else{
		int leftD = tree.left != NULL ? treeDepth(*tree.left) : 0;
		int rightD = tree.right != NULL ? treeDepth(*tree.right) : 0;
		return 1 + (leftD > rightD ? leftD : rightD);
	}
}

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

void count(struct Tree* tree, FILE* file){
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
		tree[fi] = n;
		fi++;
	}
}
