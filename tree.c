#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define INDENT 4

// Private functions
void    treeInsertNodeDumb(Tree **node, int key);
// AVL insertion
int     treeInsertNodeAVL(Tree **tree, int key);
void    treeRotateLeft(Tree **tree);
void    treeRotateRight(Tree **tree);
void    treeFixLeftImbalance(Tree **tree);
void    treeFixRightImbalance(Tree **tree);
// Printing
void    treePrinter(Tree *tree, int level);
// /Private functions

Tree *treeMakeNode(int key, int factor)
{
    Tree *node = malloc(sizeof(Tree));
    node->key       = key;
    node->factor    = factor;
    node->left      = NULL;
    node->right     = NULL;
    return node;
}
void treeInsertNodeDumb(Tree **node, int key)
{
    Tree *tree = *node;
    if (tree) {
        // Don't insert duplicates
        if (tree->key == key) {
            return;
        }
        // Decide on the next branch
        if (tree->key > key) {
            tree->factor -= 1;
            if (tree->left) {
                treeInsertNodeDumb(&tree->left, key);
            } else {
                tree->left = treeMakeNode(key, 0);
            }
        } else {
            tree->factor += 1;
            if (tree->right) {
                treeInsertNodeDumb(&tree->right, key);
            } else {
                tree->right = treeMakeNode(key, 0);
            }
        }
    } else {
        *node = treeMakeNode(key, 0);
    }
}
void treeInsertNode(Tree **tree, int key)
{
    // The dumb implementation
    //treeInsertNodeDumb(tree, key);
    
    // The AVL version
    treeInsertNodeAVL(tree, key);
}
int treeInsertNodeAVL(Tree **tree, int key)
{
    Tree *  node    = *tree;
    int     delta   = 0;
    if (!node) {
        *tree = treeMakeNode(key, 0);
        return 1;
    } else {
        // No duplicates allowed
        if (node->key == key) {
            return 0;
        }
        // Figure which branch to take
        // Return the change in depth of the branch
        if (node->key > key) {
            // Left
            delta = treeInsertNodeAVL(&node->left, key);
            if (delta == 0) {
                return 0;
            } else {
                // TODO change switch to two if statements
                switch (node->factor) {
                    case 1:
                        node->factor = 0;
                        return 0;
                    case 0:
                        node->factor = -1;
                        return 1;
                    case -1:
                        treeFixLeftImbalance(tree);
                        return 0;
                }
            }
        } else {
            // Right
            delta = treeInsertNodeAVL(&node->right, key);
            if (delta == 0) {
                return 0;
            } else {
                switch (node->factor) {
                    case 1:
                        treeFixRightImbalance(tree);
                        return 0;
                    case 0:
                        node->factor = 1;
                        return 1;
                    case -1:
                        node->factor = 0;
                        return 0;
                }
            }
        }
    }
}
void treeFixLeftImbalance(Tree **tree)
{
    Tree *child = (*tree)->left;
    if (child->factor != (*tree)->factor) {
        // Double rotation
        int factor = child->right->factor;
        treeRotateLeft(&(*tree)->left);
        treeRotateRight(tree);
        (*tree)->factor = 0;
        switch (factor) {
            case 1:
                (*tree)->left->factor   = -1;
                (*tree)->right->factor  = 0;
                break;
            case 0:
                (*tree)->left->factor   = 0;
                (*tree)->right->factor  = 0;
                break;
            case -1:
                (*tree)->left->factor   = 0;
                (*tree)->right->factor  = 1;
                break;
        }
    } else {
        // Single rotation
        treeRotateRight(tree);
        (*tree)->right->factor  = 0;
        (*tree)->factor         = 0;
    }
}
void treeFixRightImbalance(Tree **tree)
{
    Tree *child = (*tree)->right;
    if (child->factor != (*tree)->factor) {
        // Double rotation
        int factor = child->left->factor;
        treeRotateRight(&(*tree)->right);
        treeRotateLeft(tree);
        (*tree)->factor = 0;
        // TODO figure out if this is OK
        switch (factor) {
            case 1:
                (*tree)->left->factor   = -1;
                (*tree)->right->factor  = 0;
                break;
            case 0:
                (*tree)->left->factor   = 0;
                (*tree)->right->factor  = 0;
                break;
            case -1:
                (*tree)->left->factor   = 0;
                (*tree)->right->factor  = 1;
                break;
        }
    } else {
        // Single rotation
        treeRotateLeft(tree);
        (*tree)->left->factor  = 0;
        (*tree)->factor        = 0;
    }

}
void treeRotateLeft(Tree **tree)
{
    Tree *child = (*tree)->right;
    (*tree)->right = child->left;
    child->left = *tree;
    *tree = child;
}
void treeRotateRight(Tree **tree)
{
    Tree *child = (*tree)->left;
    (*tree)->left = child->right;
    child->right = *tree;
    *tree = child;
}
Tree *treeFindNode(Tree *tree, int key)
{
    // Base case: tree is empty
    if (!tree) {
        return NULL;
    }
    // Base case: tree contains the key itself
    if (tree->key == key) {
        return tree;
    }
    // Inductive steps
    if (tree->key > key) {
        return treeFindNode(tree->left, key);
    } else {
        return treeFindNode(tree->right, key);
    }
}
void treePrintNode(Tree *node)
{
    if (node) {
        printf("[%p:%d] -> l[%p] r[%p]\n", node, node->key, node->left, node->right);
    } else {
        printf("[%p]\n", node);
    }
}
void treePrint(Tree *tree)
{
    treePrinter(tree, 0);
}
void treePrinter(Tree *tree, int level)
{
    char *arrow = malloc(sizeof(char) * INDENT * level + 1);
    int i;
    for (i = 0; i < INDENT * level; i++) {
        if (i < INDENT * (level - 1)) {
            arrow[i] = ' ';
        } else if (i == INDENT * (level - 1)) {
            arrow[i] = '+';
        } else {
            arrow[i] = '-';
        }
    }
    arrow[i] = '\0';
    if (tree) {
        if (tree->left) {
            treePrinter(tree->left, level+1);
        }
        //printf("\x1b[37m%s\x1b[0m%d|\x1b[1;30;47m %d \x1b[0m\n", arrow, treeCountHeight(tree), tree->key);
        printf("\x1b[37m%s\x1b[0m%d|\x1b[1;30;47m %d \x1b[0m\n", arrow, tree->factor, tree->key);
        if (tree->right) {
            treePrinter(tree->right, level+1);
        }
    }
    arrow = realloc(arrow, 0);
}
int treeCountNodes(Tree *tree)
{
    if (!tree) {
        return 0;
    } else {
        return treeCountNodes(tree->left) + 1 + treeCountNodes(tree->right);
    }
}
int treeCountHeight(Tree *tree)
{
    if (tree) {
        int heightLeft = treeCountHeight(tree->left);
        int heightRight = treeCountHeight(tree->right);
        if (heightLeft >= heightRight) {
            return 1 + heightLeft;
        } else {
            return 1 + heightRight;
        }
    } else {
        return 0;
    }
}
