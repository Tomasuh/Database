#include <stdio.h>
#include <uuid/uuid.h>
#include <stdlib.h>
#include "engine.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    Name columns[3] = {"FIIIISK","OOOOOXE","KOOOORV"};
    Element elems[3] = {"FIIIISK","OOOOOXE","KOOOORV"};
    Type typess[3] = {DB_STRING,DB_STRING,DB_STRING};
    Database *db;
    //db = malloc(sizeof(Database));
    db_Create(&db, "Databasen");
    printf("%s",db->name);

    int *ec = malloc(sizeof(int));

    Name tbNames[] = {"Apa","Fisk","Ko"};
    *ec = db_AddTables(db, tbNames, 3);
    //printf("%s",db->tables[0]->name);

    //db_AddColumn(db,"Apa","fjong", 1);

    int ret = db_AddColumns(db,"Apa",columns,3,typess);

    printf("apa%d",ret);
    //int db_AddColumns(Database *db, Name table, Name *columns, int nrOfColumns, Type *columnType){
    /*printf("Column type:%d\n", db->tables[0]->columns[2]->type);
    printf("Column type:%d\n", db->tables[0]->columns[1]->type);
    printf("Column type:%d\n", db->tables[0]->columns[0]->type);
    printf("Column name:%s\n", db->tables[0]->columns[2]->name);
    printf("Column name:%s\n", db->tables[0]->columns[1]->name);
    printf("Column name:%s\n", db->tables[0]->columns[0]->name);*/
    //db_insertElem(db, "Apa","fjong","EPA");
    //printf("\n%s", db->tables[0]->columns[0]->elements[0]->elem);
    //(Database *db, Name table, Name column, Element element)
    ret = db_insert(db, "Apa", columns, 3, elems);
    printf("%d ret", ret);
    ret = db_insert(db, "Apa", columns, 3, elems);
    printf("%d ret", ret);
    //db_insert()
    //printf("nrElem%d",db->tables[0]->columns[0]->nrOfElements);
    //printf("\nelemVal:%s", db->tables[0]->columns[0]->elements[1]->elem);
    //printf("\nelemVal:%s", db->tables[0]->columns[2]->elements[1]->elem);

    printf("%s", db->tables[0]->row_ID[0]);
    db_close(&db);

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
        t->columns = (Column **) NULL;
        t->delete_rows = NULL;
        t->row_ID = NULL;

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
            db->tables[i]->nrOfRows = 0;
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

    /*
    Is the amount of values equal to the number of columns?
    I know this code isn't effective but may be improved when time comes.
    */
    for(int i=0; i < db->nrOfTables;i++) {
        if(strcmp(db->tables[i]->name, table)==0){
            if(db->tables[i]->nrOfColumns!=nrOfColumns){
                return WRONG_NR_OF_VALUES;
            }
        }
    }



    for(int i=0; i<nrOfColumns; i++){
        ret = db_insertElem(db, table, columns[i], elements[i]);
        if(ret!=SUCCESS){
            return ret;
        }


    }

    /*
    Update number of rows
    (Re)allocate and set row ID
    (Re)allocate rows to deleted array
    */
    for(int i=0; i < db->nrOfTables;i++) {
        if(strcmp(db->tables[i]->name, table)==0){
            db->tables[i]->nrOfRows++;

            /*First row to be added?*/
            if(db->tables[i]->row_ID==NULL){
                db->tables[i]->row_ID = malloc(sizeof(char *));
            }
            else{
                db->tables[i]->row_ID = realloc(db->tables[i]->row_ID, db->tables[i]->nrOfRows*sizeof(char *));
            }
            /*Allocate and set row ID*/
            db->tables[i]->row_ID[db->tables[i]->nrOfRows-1] = malloc(37*sizeof(char));

            uuid_t uuid;

            // generate row ID
            uuid_generate_random(uuid);
            //save it as a string
            uuid_unparse(uuid, db->tables[i]->row_ID[db->tables[i]->nrOfRows-1]);

            break;

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

                    int nrOfRows = db->tables[i]->nrOfRows+1;

                    /*Malloc or realloc?*/
                    if(nrOfRows==1){
                        db->tables[i]->columns[e]->elements = malloc(sizeof(Value *));
                    }
                    else{
                        db->tables[i]->columns[e]->elements = realloc(db->tables[i]->columns[e]->elements,nrOfRows*sizeof(Value *));
                    }


                    //Allocate space for element and write value
                    db->tables[i]->columns[e]->elements[nrOfRows-1] = malloc(sizeof(Value));
                    db->tables[i]->columns[e]->elements[nrOfRows-1]->elem = strdup(element);

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


/*Free all allocated memory*/
int db_close(Database **db){


    /*Free tables*/
    for(int i=0; i < (*db)->nrOfTables; i++){

        for(int e=0; e<(*db)->tables[i]->nrOfColumns; e++){

            /*
            Write dirty elements to file and
            Free elements
            Wrong order? should be rows then columns loop order?
            */
            for(int f=0; f<(*db)->tables[i]->nrOfRows; f++){
                /*Element value*/
                free((*db)->tables[i]->columns[e]->elements[f]->elem);
                /*Element struct*/
                free((*db)->tables[i]->columns[e]->elements[f]);
                /*Free row ID*/
                free((*db)->tables[i]->row_ID[f]);
            }

        /*Free columns*/

        free((*db)->tables[i]->columns[e]->name);
        free((*db)->tables[i]->columns[e]->elements);
        free((*db)->tables[i]->columns[e]);
        }

        free((*db)->tables[i]->columns);

        /*Free tables*/
        free((*db)->tables[i]->name);
        free((*db)->tables[i]->delete_rows);
        free((*db)->tables[i]->row_ID);

        free((*db)->tables[i]);
    }

    free((*db)->tables);

    /*Free database*/
    free((*db)->name);
    free(*db);

    return SUCCESS;
}
