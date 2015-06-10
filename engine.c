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

    Database *db;
    //db = malloc(sizeof(Database));
    db_Create(&db, "Databasen");
    printf("%s",db->name);

    int *ec = malloc(sizeof(int));

    Name tbNames[] = {"Apa","Fisk","Ko"};
    *ec = db_AddTables(db, tbNames, 3);
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
int db_AddTables(Database *db, Name *db_TableNames,int nrOfTables){
    Table *t;
    int oldExistingTb = db->nrOfTables;

    /*Make sure no existing tables with same name exists*/
    for(int i=0; i < oldExistingTb; i++){
        for(int e=0; e < nrOfTables; e++){
            if (strcmp(db_TableNames[e], db->tables[i]->name)) {
                return DUPLICATE_TABLE;
            }
        }
    }

    /*Calculate new total size needed for the tables*/
    int newSize = (oldExistingTb+nrOfTables)*sizeof(Table *);

    if(oldExistingTb>0){
        db->tables = realloc(db->tables, newSize);
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

    return SUCCESS;
}


int db_AddColumns(Database *db, Name table, Name *columns, int nrOfColumns, Type *columnType){
    for (int i=0; i < nrOfColumns; i++){

    }

    return SUCCESS;
}


int db_AddColumn(Database *db, Name table, Name column, Type columnType){
    /*First check if any duplicate columns, performance wise not superb solution*/
    for(int i=0; i < db->nrOfTables; i++){
        /*Match on correct table*/
        if(strcmp(db->tables[i]->name, table)){
            db->tables[i]->nrOfColumns += 1;

            int nrOfColumns = db->tables[i]->nrOfColumns;
            /*First time allocating memory for a column in the table*/
            if(nrOfColumns==1){
                db->tables[i]->columns = malloc(nrOfColumns*sizeof(Column *));
            }
            /*Else reallocate the list of pointers*/
            else{
                db->tables[i]->columns = realloc(db->tables[i]->columns,nrOfColumns*sizeof(Column *));
            }

            //Allocate space for the column
            db->tables[i]->columns[nrOfColumns-1] = malloc(sizeof(Column));

            return SUCCESS;
        }
    }

    return TABLE_NOT_FOUND;
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

void db_close(Database *db){

}
