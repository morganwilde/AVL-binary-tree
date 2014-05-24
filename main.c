#include <stdio.h>
#include "tree.h"

int main(void)
{
    // Testing
    // Insert nodes
    Tree *tree = NULL;
    treeInsertNodeAVL(&tree, 0);
    int i;
    for (i = 0; i < 3; i++) {
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
