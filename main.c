#include <stdio.h>
#include "tree.h"

int main(void)
{
    // Testing
    // Insert nodes
    Tree *tree = NULL;
    treeInsertNode(&tree, 20);
    int i;
    //for (i = -1; i > -8; i--) {
    for (i = 1; i < 20; i++) {
        treeInsertNode(&tree, i);
    }
    for (i = 21; i < 41; i++) {
        treeInsertNode(&tree, i);
    }
    // Find a node
    Tree *found = treeFindNode(tree, 22);
    //treePrintNode(found);
    // Count nodes
    printf("Number of nodes in *tree: %d\n", treeCountNodes(tree));
    // Print out the tree
    treePrint(tree);

    return 0;
}
