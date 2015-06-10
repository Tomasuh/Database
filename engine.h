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


typedef struct{
   Element  elem;
}Value;

typedef struct{
    Name name;
    Type type;
    Value **elements;
    int nrOfElements;
}Column;

typedef struct{
    Name name;
    Column **columns;
    int nrOfColumns;
}Table;

typedef struct{
    Name    name;
    Table **tables;
    int nrOfTables;
}Database;


void db_Create(Database **db, Name db_Database_Name);
int db_AddTables(Database *db, Name *db_TableNames,int nrOfTables);
int db_AddColumn(Database *db, Name table, Name column, Type columnType);
int db_AddColumns(Database *db, Name table, Name *columns, int nrOfColumns, Type *columnType);
int db_insert(Database *db, Name table, Name *columns, int nrOfColumns, Element *elements);
int db_insertElem(Database *db, Name table, Name column, Element element);
#endif // ENGINE_H_INCLUDED
