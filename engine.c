#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include <stdlib.h>
#include <string.h>
int main()
{
    //Database myOwn;
    printf("Hello world!\n");



    /*
    Create following database structure:
    Database name: My test database
    Tables {column:type}:
    Apa {kiosk:String}

    Fisk {mes:int}
    Korv {brod:float}


    */



    /*Database name*/
//    Name name = "BAJS";
  //  db.name = name;

    /*List of tables
      allocate space for 3 tables.
    */
    //Table *tb = malloc(3*sizeof(Table));
    /*Allocate space for the table instances*/
    /*for(int i=0; i<3; i++){
        tb[i] = malloc(sizeof(Table));
    }*/


    /*Table names*/
    /*Name tb1 = "Apa";
    Name tb2 = "Fisk";
    Name tb3 = "Korv";

    /*Set table names and allocate space for columns*/
    /*tb[0].name = strdup(tb1);
    tb[1].name = strdup(tb2);
    tb[2].name = strdup(tb3);

    /*Allocate space for columns*/
    /*tb[0].columns = malloc(1*sizeof(Column));
    tb[1].columns = malloc(1*sizeof(Column));
    tb[2].columns = malloc(1*sizeof(Column));


    /*Column names and types*/
    /*Name c1 = strdup("kiosk");
    Name c2 = strdup("mes");
    Name c3 = strdup("brod");

    /*Set column type and name*/


    /*Add tables to database*/
    /*db.tables = tb;
    db.nrOfTables =3;

    Column column;
    myOwn = db_Create(name);
    */

    Database *db;
    //db = malloc(sizeof(Database));
    db_Create(&db, "Databasen");
    printf("%s",db->name);

    int *ec = malloc(sizeof(int));

    Name tbNames[] = {"Apa","Fisk","Ko"};
    db_AddTables(db, tbNames, 3,ec);
    printf("%s",db->tables[0]->name);
    return 0;
}

void db_Create(Database **db, Name db_Database_Name){

    *db = malloc(sizeof(Database));

    (*db)->name = strdup((const char *) db_Database_Name);

    /*Empty database*/
    (*db)->tables = NULL;
    (*db)->nrOfTables = 0;
}

/*


*/
void db_AddTables(Database *db, Name *db_TableNames,int nrOfTables, int *errorcode){
    Table *t;
    int oldExistingTb = db->nrOfTables;

    /*Make sure no existing tables with same name exists*/
    for(int i=0; i < oldExistingTb; i++){
        for(int e=0; e < nrOfTables; e++){
            if (strcmp(db_TableNames[e], db->tables[i]->name)) {
                *errorcode = DUPLICATE_TABLE;
                return;
            }
        }
    }

    /*Calculate new total size needed for the tables*/
    int newSize = (oldExistingTb+nrOfTables)*sizeof(Table);

    if(oldExistingTb>0){
        db->tables = realloc(db, newSize);
    }
    else {
        db->tables = malloc(newSize);
    }

    /*
    Allocate size for each table and add them to the database.
    */
    for (int i=0; i < nrOfTables; i++){
        t = malloc(sizeof(Table));
        t->name = (Name) strdup(db_TableNames[i]);
        t->nrOfColumns = 0;
        t->columns = (Column *) NULL;
        db->tables[oldExistingTb+i] = t;
    }

    db->nrOfTables = oldExistingTb + nrOfTables;

    *errorcode = SUCCESS;
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

void db_close(Database *db, int *errorcode){
}
