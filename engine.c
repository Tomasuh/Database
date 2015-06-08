#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include <stdlib.h>
#include <string.h>
int main()
{
    Database myOwn;
    printf("Hello world!\n");



    /*
    Create following database structure:
    Database name: My test database
    Tables {column:type}:
    Apa {kiosk:String}

    Fisk {mes:int}
    Korv {brod:float}


    */

    Database db;

    /*Database name*/
    Name name = "BAJS";
    db.name = name;

    /*List of tables
      allocate space for 3 tables.
    */
    Table *tb = malloc(3*sizeof(Table));
    /*Allocate space for the table instances*/
    /*for(int i=0; i<3; i++){
        tb[i] = malloc(sizeof(Table));
    }*/


    /*Table names*/
    Name tb1 = "Apa";
    Name tb2 = "Fisk";
    Name tb3 = "Korv";

    /*Set table names and allocate space for columns*/
    tb[0].name = strdup(tb1);
    tb[1].name = strdup(tb2);
    tb[2].name = strdup(tb3);

    /*Allocate space for columns*/
    tb[0].columns = malloc(1*sizeof(Column));
    tb[1].columns = malloc(1*sizeof(Column));
    tb[2].columns = malloc(1*sizeof(Column));


    /*Column names and types*/
    Name c1 = strdup("kiosk");
    Name c2 = strdup("mes");
    Name c3 = strdup("brod");

    /*Set column type and name*/


    /*Add tables to database*/
    db.tables = tb;
    db.nrOfTables =3;

    Column column;
    myOwn = db_Create(name);
    return 0;
}

Database db_Create(Name db_Database_Name){

    Database *db = malloc(sizeof(Database));

    db->name = strdup((const char *) db_Database_Name);

    /*Empty database*/
    db->tables = NULL;
    db->nrOfTables = 0;

    return *db;
}

/*


*/
void db_AddTables(Database *db, Name *db_TableNames,int nrOfTables){
    Table *t;
    int oldExistingTb = db->nrOfTables;
    int newSize = (oldExistingTb+nrOfTables)*sizeof(Table);
    db->tables = realloc(db, newSize);

    for (int i=0; i < nrOfTables; i++){
        t = malloc(sizeof(Table));
        t->name = strdup(db_TableNames[i]);
        t->nrOfColumns = 0;
        t->columns = (Column *) NULL;
        db->tables[oldExistingTb+i] = *t;
    }
    db->nrOfTables = oldExistingTb + nrOfTables;
}


void db_AddColumns(Database *db, Name table, Name *columns, int nrOfColumns, Type *columnType){
    for (int i=0; i < nrOfColumns; i++){

    }
}


void db_AddColumn(Database *db, Name table, Name column, Type columnType){

}


Database db_load(Name fileName){
    Database db;
    return db;
}

//Database db_initialize(Database db,int db_Name_Length)

void db_insert(Database db, Name db_columnName, Element element){
}

void db_select(){
}

void db_delete(){
}

void db_close(){
}
