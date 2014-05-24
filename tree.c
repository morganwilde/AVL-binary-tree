#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define INDENT 4

// Private functions
void treePrinter(Tree *tree, int level);
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
void treeInsertNode(Tree *tree, int key)
{
    if (tree) {
        // Don't insert duplicates
        if (tree->key == key) {
            return;
        }
        // Decide on the next branch
        if (tree->key > key) {
            tree->factor -= 1;
            if (tree->left) {
                treeInsertNode(tree->left, key);
            } else {
                tree->left = treeMakeNode(key, 0);
            }
        } else {
            tree->factor += 1;
            if (tree->right) {
                treeInsertNode(tree->right, key);
            } else {
                tree->right = treeMakeNode(key, 0);
            }
        }
    }
}
int treeInsertNodeAVL(Tree **tree, int key)
{
    if (!tree) {
        *tree = treeMakeNode(0, 0);
        return 1;
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
    printf("here\n");
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
