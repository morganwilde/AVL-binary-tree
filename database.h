#ifndef DATABASE_H
#define DATABASE_H

#define TOKEN_MAX_LENGTH 256

typedef enum QueryType {
    UNDEFINED,
    INTENT,
    TABLE,
    FIELD,
    CONSTRAINT
} QueryType;

typedef enum IntentType {
    CREATE_TABLE,
    INSERT,
    SELECT,
    UPDATE,
    DELETE
} IntentType;

typedef struct Query {
    QueryType       type;   // What does this link of the query corresponds to
    void *          value;  // The value to be interpreted
    const char *    token;  // Raw information
    struct Query *  before; // Previous link
    struct Query *  after;  // Next link
} Query;

Query *dbAddQueryToken(Query **query, const char *token);
Query *dbInterpretQuery(const char *query);
void dbCommit(Query *query);

#endif /* DATABASE_H  */
