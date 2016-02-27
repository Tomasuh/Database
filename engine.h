#include <stdbool.h>
#include <uuid/uuid.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "error_codes.h"

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

typedef char* Element;
typedef char* Name;

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
    //int nrOfElements;
}Column;

typedef struct{
    Name name;
    Column **columns;
    int nrOfColumns;
    int nrOfRows;
    bool *delete_rows;
    bool *dirty_rows;
    char **row_ID;
    unsigned int free_elems;
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
int db_deleteWhere(Name table, Name *columnsToMatch, int nrOfColumns, Name *valuesToMatch, Name *columnToReturn);
int db_close(Database **db);

/*Internal functions used*/

int db_free_database(Database **db);
int db_free_table(Table *table);
int db_free_column(Column *column, int nrOfRows, bool *delete_rows);
int db_free_value(Value *value);

void* allocateBytes(int nrOfBytes);
void reAllocateBytes(void** memory,int nrOfBytes);
char* strdupErrorChecked(const char* str);

int db_deleteRows(Database *db, char **rowID, int nrOfRows, Name table);
int db_deleteRow(Table *table, char *rowID);
int tableIndex(Database *db, Name tableName);

#endif // ENGINE_H_INCLUDED
