#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "database.h"

// Private interface
Query *dbQueryFind(Query *query, QueryType type, void *value, DirectionType direction);
FILE *dbTableCreate(const char *name);
FILE *dbTableOpen(const char *name);

// Implementations
Query *dbInterpretQuery(const char *tokens)
{
    // Copy the string to avoid mutation
    char *temp = malloc(sizeof(char) * (strlen(tokens) + 1));
    char *tempAddress = temp;
    strcpy(temp, tokens);
    // Tokenize
    Query *query = NULL;
    QueryType queryType = UNDEFINED;
    IntentType queryIntent = UNKNOWN;
    temp = strtok(temp, " ");
    while (temp != NULL) {
        printf("token: [%s]\n", temp);
        dbAddQueryToken(&query, temp, &queryType, &queryIntent);
        temp = strtok(NULL, " ");
    }
    tempAddress = realloc(tempAddress, 0);
    // Return results
    return query;
}
Query *dbAddQueryToken(Query **query, const char *token, QueryType *continued, IntentType *intent)
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
        // Fast forward to last element in list
        while (link->after) {
            link = link->after;
        }
        dbAddQueryToken(&link->after, token, continued, intent);
        link->after->before = link;
        // Interpret the token
        // Find query type
        if (strcmp(token, "TABLE") == 0) {
            // Table related query
            if (link && strcmp(link->token, "CREATE") == 0) {
                // Query type:  Intent
                link->type          = INTENT;
                link->after->type   = INTENT;
                // Value:       Create a new table
                link->value         = malloc(sizeof(IntentType));
                link->after->value  = malloc(sizeof(IntentType));
                *(IntentType *)link->value          = CREATE_TABLE;
                *(IntentType *)link->after->value   = CREATE_TABLE;
                *intent = CREATE_TABLE;
            } else {
                printf("Don't know what to do with TABLE %p\n", link->before);
            }
        } else if (strcmp(token, "(") == 0) {
            // This is a delimeter
            link->after->type = DELIMITER;
            link->after->value = malloc(sizeof(DelimiterType));
            *(DelimiterType *)link->after->value = BEGIN;
            // Before the brace there must be the table name
            if (link && strlen(link->token)) {
                link->type = TABLE;
                link->value = malloc(sizeof(char) * (strlen(link->token) + 1));
                strcpy((char *)link->value, link->token);
            }
            // After the brace is a list of fields
            *continued = FIELD;
        } else if (strcmp(token, ")") == 0) {
            if (*intent == CREATE_TABLE) {
                // Record the type of constraint
                Query *constraint = link;
                while (constraint && constraint->type != CONSTRAINT) {
                    constraint = constraint->before;
                }
                constraint->value = malloc(sizeof(char) * (strlen(constraint->after->token) + 1));
                strcpy((char *)constraint->value, constraint->after->token);
                // And it's value
                link->type = CONSTRAINT_VALUE;
                link->value = malloc(sizeof(char) * (strlen(link->token) + 1));
                strcpy(link->value, link->token);
            }
            *continued = UNDEFINED;
        } else {
        // Find fields
            if (*continued == FIELD) {
                // CREATE TABLE field format: field_name type,
                // INSERT field format: field_name = value,
                if (*intent == CREATE_TABLE) {
                    if (strstr(token, "PRIMARY") == NULL) {
                        if (strstr(token, ",") == NULL) {
                            // field_name
                            link->type = FIELD_NAME;
                            link->value = malloc(sizeof(char) * (strlen(token) + 1));
                            strcpy((char *)link->value, token);
                        } else {
                            // type
                            link->type = FIELD_TYPE;
                            link->value = malloc(sizeof(FieldType));
                            if (strstr(token, "char1") != NULL) {
                                *(FieldType *)link->value = CHAR1;
                            } else if (strstr(token, "char32") != NULL) {
                                *(FieldType *)link->value = CHAR32;
                            } else if (strstr(token, "integer") != NULL) {
                                *(FieldType *)link->value = INTEGER;
                            }
                        }
                    } else {
                        // Define constraint - key
                        link->type = CONSTRAINT;
                    }
                }
            }
        }
        return NULL;
    }
}
void dbCommit(Query *query)
{
    // Determine the type of the query
    if (query->type == INTENT) {
        // Determine the intent of the query
        if (*(IntentType *)query->value == CREATE_TABLE) {
            Query *queryTable = dbQueryFind(query, TABLE, NULL, TOEND);
            char *tableName = (char *)queryTable->value;
            // Check if the table doesn't already exist
            FILE *table = dbTableOpen(tableName);
            if (table) {
                printf("Table '%s' already exists!\n", tableName);
                return;
            } else {
                table = dbTableCreate(tableName);
                fprintf(table, "%s\n", tableName);
            }
            // Find the primary field
            Query *queryPrimary = dbQueryFind(query, CONSTRAINT, "PRIMARY", TOEND);
            Query *queryPrimaryValue = queryPrimary->after;
            char *fieldPrimary = (char *)queryPrimaryValue->value;
            printf("[%s]\n", fieldPrimary);
            fprintf(table, "[%s]\n", fieldPrimary);
            // Check all the rest
            int fieldCount = 0;
            Query *fields = query->after->after->after;
            while (fields->type == FIELD_NAME) {
                char *      fieldName = (char *)fields->value;
                FieldType   fieldType = *(FieldType *)fields->after->value;
                // Save to table
                fprintf(table, "%-16s:%d\n", fieldName, fieldType);
                // On to the next one
                fields = fields->after->after;
                fieldCount++;
            }
            fprintf(table, "#%d\n", fieldCount);
            fclose(table);
        }
        if (*(IntentType *)query->value == INSERT) {
            printf("Insert something\n");
        }
    }
}
Query *dbQueryFind(Query *query, QueryType type, void *value, DirectionType direction)
{
    if (!query) {
        return NULL;
    } else if (query->type == type) {
        switch (type) {
            case TABLE:
                return query;
            case CONSTRAINT:
                if (strcmp((char *)query->value, (char *)value) == 0) {
                    return query;
                } else {
                    goto skipQuery;
                }
        }
    } else {
skipQuery:
        if (direction == TOEND) {
            return dbQueryFind(query->after, type, value, direction);
        } else if (direction == TOSTART) {
            return dbQueryFind(query->before, type, value, direction);
        }
    }
}
FILE *dbTableCreate(const char *name)
{
    // Assemble the file
    char *filename = malloc(sizeof(char) * (strlen("data/") + strlen(name) + strlen(".db") + 1));
    strcat(filename, "data/");
    strcat(filename, name);
    strcat(filename, ".db");
    // Test for it
    FILE *file = fopen(filename, "w");
    
    return file;
}
FILE *dbTableOpen(const char *name)
{
    // Assemble the file
    char *filename = malloc(sizeof(char) * (strlen("data/") + strlen(name) + strlen(".db") + 1));
    strcat(filename, "data/");
    strcat(filename, name);
    strcat(filename, ".db");
    // Test for it
    FILE *file = fopen(filename, "r");
    
    return file;
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
