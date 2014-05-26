#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "database.h"

Query *dbInterpretQuery(const char *tokens)
{
    // Copy the string to avoid mutation
    char *temp = malloc(sizeof(char) * (strlen(tokens) + 1));
    char *tempAddress = temp;
    strcpy(temp, tokens);
    // Tokenize
    Query *query = NULL;
    temp = strtok(temp, " ");
    while (temp != NULL) {
        dbAddQueryToken(&query, temp);
        temp = strtok(NULL, " ");
    }
    tempAddress = realloc(tempAddress, 0);
    // Return results
    return query;
}
Query *dbAddQueryToken(Query **query, const char *token)
{
    if (!*query) {
        // Initialize the query object
        *query = malloc(sizeof(Query));
        Query *link = *query;
        link->type          = UNDEFINED;
        link->value         = NULL;
        link->token         = token;
        link->before        = NULL;
        link->after         = NULL;
        // Interpret the token
        // Find query type
        if (strcmp(token, "INSERT") == 0) {
            // Insert related query
            link->type = INTENT;
            link->value = malloc(sizeof(IntentType));
            *(IntentType *)link->value = INSERT;
        }
        return link;
    } else {
        Query *link = *query;
        dbAddQueryToken(&link->after, token);
        link->after->before = *query;
        // Interpret the token
        // Find query type
        if (strcmp(token, "TABLE") == 0) {
            // Table related query
            if (link && strcmp(link->token, "CREATE") == 0) {
                // Query type:  Intent
                link->type          = INTENT;
                link->after->type   = INTENT;
                // Value:       Create a new table
                IntentType *value = malloc(sizeof(IntentType));
                *value = CREATE_TABLE;
                link->value = value;
                link->after->value = value;
            } else {
                printf("Don't know what to do with TABLE %p\n", link->before);
            }
        } 
        return link->after;
    }
}
void dbCommit(Query *query)
{
    // Determine the type of the query
    switch (query->type) {
        case INTENT:
            // Determine the intent of the query
            switch (*(IntentType *)query->value) {
                case CREATE_TABLE:
                    printf("Go make a table\n");
                    break;
                case INSERT:
                    printf("Insert something\n");
                    break;
            }
            break;
    }
    printf("query->type: [%d]\n", query->type);
    printf("query->value: [%d]\n", *(IntentType *)query->value);
    /*
    while (query) {
        printf("query->token: [%s]\n", query->token);
        query = query->after;
    }
    */
}
/*
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
*/
