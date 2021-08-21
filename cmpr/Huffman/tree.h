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
