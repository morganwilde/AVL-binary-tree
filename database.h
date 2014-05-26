#ifndef DATABASE_H
#define DATABASE_H

#define TOKEN_MAX_LENGTH 256

typedef enum QueryType {
    UNDEFINED   = 0,
    INTENT      = 1,
    TABLE       = 2,
    FIELD       = 3,
    FIELD_NAME  = 4,
    FIELD_TYPE  = 5,
    CONSTRAINT  = 6,
    CONSTRAINT_VALUE = 7,
    DELIMITER   = 8
} QueryType;

typedef enum DelimiterType {
    BEGIN,
    END
} DelimiterType;

typedef enum DirectionType {
    TOEND,      // Left to right
    TOSTART,    // Right to left
} DirectionType;

typedef enum IntentType {
    UNKNOWN,
    CREATE_TABLE,
    INSERT,
    SELECT,
    UPDATE,
    DELETE
} IntentType;

typedef enum FieldType {
    NONE,
    INTEGER,
    CHAR1,
    CHAR32
} FieldType;

typedef struct Query {
    QueryType       type;   // What does this link of the query corresponds to
    void *          value;  // The value to be interpreted
    const char *    token;  // Raw information
    struct Query *  before; // Previous link
    struct Query *  after;  // Next link
} Query;

Query *dbAddQueryToken(Query **query, const char *token, QueryType *continued, IntentType *intent);
Query *dbInterpretQuery(const char *query);
void dbCommit(Query *query);
// To be made private
Query *dbQueryFind(Query *query, QueryType type, void *value, DirectionType direction);

#endif /* DATABASE_H  */
