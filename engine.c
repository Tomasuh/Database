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
    Name columns = {"FIIIISK","OOOOOXE","KOOOORV"};
    Element elems = {"FIIIISK","OOOOOXE","KOOOORV"};
    Type typess = {DB_STRING,DB_STRING,DB_STRING};
    Database *db;
    //db = malloc(sizeof(Database));
    db_Create(&db, "Databasen");
    printf("%s",db->name);

    int *ec = malloc(sizeof(int));

    Name tbNames[] = {"Apa","Fisk","Ko"};
    *ec = db_AddTables(db, tbNames, 3);
    printf("%s",db->tables[0]->name);

    db_AddColumn(db,"Apa","fjong", 1);

    db_AddColumns(db,"Apa",&columns,3,&typess);
    //int db_AddColumns(Database *db, Name table, Name *columns, int nrOfColumns, Type *columnType){
    //printf("%d", db->tables[0]->columns[2]->type);
    printf("%d", db->tables[0]->columns[1]->type);
    //db_insertElem(db, "Apa","fjong","EPA");
    printf("%s", db->tables[0]->columns[0]->elements[0]->elem);
    //(Database *db, Name table, Name column, Element element)
    db_insert(db, "Apa", &columns, 3, &elems);
    //db_insert()
    printf("%s", db->tables[0]->columns[0]->elements[0]->elem);

    //printf("%d", db->tables[0]->nrOfColumns);
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
            if ((strcmp(db_TableNames[e], db->tables[i]->name))==0) {
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
    int ret;
    for (int i=0; i < nrOfColumns; i++){
        ret = db_AddColumn(db, table, columns[i], columnType[i]);

        if(ret!=SUCCESS) {
            return ret;
        }
    }

    return SUCCESS;
}


int db_AddColumn(Database *db, Name table, Name column, Type columnType){
    for(int i=0; i < db->nrOfTables; i++){
        /*Match on correct table*/
        if((strcmp(db->tables[i]->name, table))==0){
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

            //Allocate space for the column and set type
            db->tables[i]->columns[nrOfColumns-1] = malloc(sizeof(Column));
            db->tables[i]->columns[nrOfColumns-1]->type = columnType;
            db->tables[i]->columns[nrOfColumns-1]->name = strdup(column);
            db->tables[i]->columns[nrOfColumns-1]->elements = NULL;
            db->tables[i]->columns[nrOfColumns-1]->nrOfElements = 0;
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

int db_insert(Database *db, Name table, Name *columns, int nrOfColumns, Element *elements){
    int ret;
    for(int i=0; i<nrOfColumns; i++){
        ret = db_insertElem(db, table, columns[i], elements[i]);
        if(ret!=SUCCESS){
            return ret;
        }
    }
    return SUCCESS;
}

int db_insertElem(Database *db, Name table, Name column, Element element){
    /*Find correct table*/
    for(int i=0; i < db->nrOfTables;i++) {
        if(strcmp(db->tables[i]->name, table)==0){

            /*Find correct column*/
            for(int e=0; e < db->tables[i]->nrOfColumns; e++){
                if(strcmp(db->tables[i]->columns[e]->name, column)==0){

                    int nrOfElements = db->tables[i]->columns[e]->nrOfElements;

                    /*Malloc or realloc?*/
                    if(nrOfElements==0){
                        db->tables[i]->columns[e]->elements = malloc(sizeof(Element *));
                    }
                    else{
                        db->tables[i]->columns[e]->elements = realloc(db->tables[i]->columns[e]->elements,nrOfElements*sizeof(Element *));
                    }
                    nrOfElements++;

                    //Allocate space for element and write value
                    db->tables[i]->columns[e]->elements[nrOfElements-1] = malloc(sizeof(Element));
                    db->tables[i]->columns[e]->elements[nrOfElements-1]->elem = strdup(element);
                    //Update number of elements
                    db->tables[i]->columns[e]->nrOfElements=nrOfElements;
                    return SUCCESS;

                }
            }
            return COLUMN_NOT_FOUND;

        }
    }
    return TABLE_NOT_FOUND;
}

void db_select(){
}

void db_delete(){
}

void db_close(Database *db){

}
