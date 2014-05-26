#include "database.h"

int main(void)
{
    Query *query = dbInterpretQuery(
        "CREATE TABLE elements ( "
            "number  integer, "
            "name    char[2], "
            "PRIMARY number "
        ");");
    dbCommit(query);
    dbCommit(dbInterpretQuery("INSERT"));

    return 0;
}
