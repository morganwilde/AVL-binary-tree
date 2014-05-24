#ifndef TREE_H
#define TREE_H

typedef struct Tree {
    int             key;    // Holds the primary organizing value
    int             factor; // The balance factor
    struct Tree *   left;   // key > left->key
    struct Tree *   right;  // key < right->key
} Tree;

// Creation and insertion
Tree *  treeMakeNode(int key, int factor);
void    treeInsertNode(Tree *tree, int key);
int     treeInsertNodeAVL(Tree **tree, int key);
// To be made private
void    treeRotateLeft(Tree **tree);
void    treeRotateRight(Tree **tree);
void    treeFixLeftImbalance(Tree **tree);
void    treeFixRightImbalance(Tree **tree);
// Search
Tree *  treeFindNode(Tree *tree, int key);
// Printing
void    treePrintNode(Tree *node);
void    treePrint(Tree *tree);
// Statistics
int     treeCountNodes(Tree *tree);
int     treeCountHeight(Tree *tree);

#endif /* TREE_H */
