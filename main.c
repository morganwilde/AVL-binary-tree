#include "database.h"

int main(void)
{
    Query *query = dbInterpretQuery(
        "CREATE TABLE elements ( "
            "number  integer, "
            "name    char32, "
            "PRIMARY number "
        ") SORT BY number;");
    dbCommit(query);

    return 0;
}
