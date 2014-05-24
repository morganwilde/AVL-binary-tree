#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define INDENT 4

// Private functions
int     treeInsertAVL(Tree **tree, int key);
void    treeFixLeftImbalance(Tree **tree);
void    treeFixRightImbalance(Tree **tree);
void    treeRotateLeft(Tree **tree);
void    treeRotateRight(Tree **tree);
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
void treeInsertNode(Tree **tree, int key)
{
   treeInsertAVL(tree, key);
}
int treeInsertAVL(Tree **tree, int key)
{
    if (*tree) {
        // Don't insert duplicates
        if ((*tree)->key == key) {
            return;
        }
        // Decide on the next branch
        if ((*tree)->key > key) {
            if ((*tree)->left) {
                int delta = treeInsertAVL(&((*tree)->left), key);
                if (!delta) {
                    return 0;
                } else {
                    switch((*tree)->factor) {
                        case 1: 
                            (*tree)->factor = 0;
                            return 0;
                        case 0:
                            (*tree)->factor = -1;
                            return 1;
                        case -1:
                            treeFixLeftImbalance(tree);
                            return 0;
                    }
                }
            } else {
                (*tree)->left = treeMakeNode(key, 0);
                return 1;
            }
        } else {
            if ((*tree)->right) {
                int delta = treeInsertAVL(&((*tree)->right), key);
                if (!delta) {
                    return 0;
                } else {
                    switch((*tree)->factor) {
                        case -1: 
                            (*tree)->factor = 0;
                            return 0;
                        case 0:
                            (*tree)->factor = 1;
                            return 1;
                        case 1:
                            treeFixRightImbalance(tree);
                            return 0;
                    }
                }
            } else {
                (*tree)->right = treeMakeNode(key, 0);
                return 1;
            }
        }
    }
}
void treeFixLeftImbalance(Tree **tree)
{
    Tree *child = (*tree)->left;
    if (child->factor != (*tree)->factor) {
        int factor = child->right->factor;
        treeRotateLeft(&(*tree)->left);
        treeRotateRight(tree);
        (*tree)->factor = 0;
        switch(factor) {
            case -1: 
                (*tree)->left->factor  = 0;
                (*tree)->right->factor = 1;
                break;
            case 0:
                (*tree)->left->factor  = 0;
                (*tree)->right->factor = 0;
                break;
            case 1:
                (*tree)->left->factor  = -1;
                (*tree)->right->factor = 0;
        }
    } else {
        treeRotateRight(tree);
        (*tree)->factor = 0;
        (*tree)->right->factor = 0;
    }
}
void treeFixRightImbalance(Tree **tree)
{
    Tree *child = (*tree)->right;
    if (child->factor != (*tree)->factor) {
        int factor = child->left->factor;
        treeRotateRight(&(*tree)->right);
        treeRotateLeft(tree);
        (*tree)->factor = 0;
        switch(factor) {
            case -1: 
                (*tree)->left->factor  = 1;
                (*tree)->right->factor = 0;
                break;
            case 0:
                (*tree)->left->factor  = 0;
                (*tree)->right->factor = 0;
                break;
            case 1:
                (*tree)->left->factor  = 0;
                (*tree)->right->factor = -1;
        }
    } else {
        treeRotateLeft(tree);
        (*tree)->factor = 0;
        (*tree)->left->factor = 0;
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
        printf("[%p:%d] -> [%p][%p]\n", node, node->key, node->left, node->right);
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
