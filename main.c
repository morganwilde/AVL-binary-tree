#include <stdio.h>
#include "tree.h"

int main(void)
{
    Tree *tree = treeMakeNode(0, 0);
    treePrintNode(tree);
    // Testing
    // Insert nodes
    int i;
    for (i = 0; i < 10; i++) {
        treeInsertNode(tree, i);
    }
    /*
    treeInsertNode(&tree, 26);
    treeInsertNode(&tree, 16);
    treeInsertNode(&tree, 21);
    treeInsertNode(&tree, 1);
    treeInsertNode(&tree, 17);
    */
    // Find a node
    Tree *found = treeFindNode(tree, 22);
    treePrintNode(found);
    // Count nodes
    printf("Number of nodes in *tree: %d\n", treeCountNodes(tree));
    // Print out the tree
    treePrint(tree);

    return 0;
}
