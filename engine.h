#include "error_codes.h"

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

typedef char *Element;
typedef char *Name;



typedef unsigned int Type;

/*
database types
string
char
double
float*/

#define DB_STRING  0
#define DB_CHAR    1
#define DB_DOUBLE  2
#define DB_FLOAT   3


/*
database typesize in words
not needed?
*/
#define DB_STRING_S  0
#define DB_CHAR_S    1
#define DB_DOUBLE_S  2
#define DB_FLOAT_S   3




typedef struct{
   Element  elem;
}Value;

typedef struct{
    Type    type;
    Value   **elements;
}Column;
/*
elements[1] row 1 in column
*/

typedef struct{
    Name name;
    Column **columns;
    int nrOfColumns;
}Table;
//column[1]

typedef struct{
    Name    name;
    Table **tables;
    int nrOfTables;
}Database;
//tables[1]->column



void db_Create(Database **db, Name db_Database_Name);
int db_AddTables(Database *db, Name *db_TableNames,int nrOfTables);
int db_AddColumn(Database *db, Name table, Name column, Type columnType);
#endif // ENGINE_H_INCLUDED
