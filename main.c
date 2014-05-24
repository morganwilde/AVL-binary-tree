#include <stdio.h>
#include "tree.h"

int main(void)
{
    // Testing
    // Insert nodes
    Tree *tree = NULL;
    treeInsertNodeAVL(&tree, 8);
    int i;
    //for (i = -1; i > -8; i--) {
    for (i = 1; i < 8; i++) {
        treeInsertNodeAVL(&tree, i);
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
