#include "engine.h"
#include "error_codes.h"

void db_Create(Database **db, Name db_Database_Name){

    *db = (Database *) allocateBytes(sizeof(Database));
    (*db)->name = strdupErrorChecked((const char *) db_Database_Name);

    /*Empty database*/
    (*db)->tables = NULL;
    (*db)->nrOfTables = 0;
}

/*
Function: Adds a number of tables to the database.

Parameters:
    Database *db, database pointer
    Name *db_TableNames string array of table names.
    int nrOfTables, the number of tables to be added.

Returns:
    SUCCESS or DUPLICATE_TABLE
*/
int db_AddTables(Database *db, Name *db_TableNames, int nrOfTables){
    Table *t;
    int oldExistingTb = db->nrOfTables;

    /*Make sure no existing tables with same name exists*/

    for(int e=0; e < nrOfTables; e++){
        if(findTable(db, db_TableNames[e])!=NULL){
             return DUPLICATE_TABLE;
        }
    }
    

    /*Calculate new total size needed for the tables*/
    int newSize = (oldExistingTb+nrOfTables)*sizeof(Table *);

    if(oldExistingTb>0){
        //db->tables = realloc(db->tables, newSize);
        reAllocateBytes((void **) &db->tables, newSize);
    }
    else {
        db->tables = allocateBytes(newSize);
    }

    /*
    Allocate size for each table and add them to the database.
    */
    for (int i=0; i < nrOfTables; i++){
        t = allocateBytes(sizeof(Table));
        //t->name = (Name) strdup(db_TableNames[i]);
        t->name = (Name) strdupErrorChecked(db_TableNames[i]);
        t->nrOfColumns = 0;
        t->columns = (Column **) NULL;
        t->delete_rows = NULL;
        t->dirty_rows = NULL;
        t->row_ID = NULL;
        t->freeRows = NULL;

        db->tables[oldExistingTb+i] = t;
    }

    db->nrOfTables = oldExistingTb + nrOfTables;

    return SUCCESS;
}

/*
Function: Adds a number of columns to the database.

Parameters:
    Database *db, database pointer
    Name table, the name of the table to add columns to.
    Name *columns, the columns to add.
    int nrOfColumns, the number of columns to be added.
    Type *columnType, the type of each column.

Returns:
    SUCCESS or DUPLICATE_TABLE

Note:
    columns and columnType comes in the same order. Change?
*/
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


/*
Function: Adds a columnsto the database.

Parameters:
    Database *db, database pointer
    Name table, the name of the table to add column to.
    Name column, the column to add.
    Type columnType, the type of the column.

Returns:
    SUCCESS or DUPLICATE_TABLE
*/
int db_AddColumn(Database *db, Name tableName, Name column, Type columnType){
    /*Match on correct table*/
    Table* table = findTable(db, tableName);

    if(!table){
        return TABLE_NOT_FOUND;
    }

    table->nrOfColumns += 1;

    int nrOfColumns = table->nrOfColumns;
    /*First time allocating memory for a column in the table?*/
    if(nrOfColumns==1){
        table->columns = allocateBytes(nrOfColumns*sizeof(Column *));
    }
    /*Else reallocate the list of pointers*/
    else{
        reAllocateBytes((void **) &table->columns,nrOfColumns*sizeof(Column *));
    }

    //Allocate space for the column and set type
    table->columns[nrOfColumns-1] = allocateBytes(sizeof(Column));
    table->columns[nrOfColumns-1]->type = columnType;
    table->columns[nrOfColumns-1]->name = strdupErrorChecked(column);
    table->columns[nrOfColumns-1]->elements = NULL;
    table->nrOfRows = 0;
    return SUCCESS;
}


Database db_load(Name fileName){
    Database db;
    return db;
}

//Database db_initialize(Database db,int db_Name_Length)

/*
Function: Creates a row with elements

Parameters:
    Database *db, database pointer
    Name table, the name of the table to add row to.
    Name *columns, the column to add elements to.
    int nrOfColumns, the number of columns.
    Element *elements, the elements.

Returns:
    SUCCESS or DUPLICATE_TABLE
*/
int db_insert(Database *db, Name tableName, Name *columns, int nrOfColumns, Element *elements){
    int ret;
    int freeRowIndex;
    bool newRow = true;

    Table* table = findTable(db, tableName);

    if(!table){
        return TABLE_NOT_FOUND;
    }

    if(table->nrOfColumns!=nrOfColumns){
        return WRONG_NR_OF_VALUES;
    }

    /*
    Update number of rows
    (Re)allocate and set row ID
    (Re)allocate rows to deleted array and initialise added element to false
    (Re)allocate rows to dirty array and initialise added element to false
    */

    int nrOfRows = table->nrOfRows;
    freeRowIndex = nrOfRows; //Initially place free row to be last element + 1

    //Is there any deleted row we can use available, if so find it's index.
    if(table->freeRows){
        newRow = false;
        freeRowIndex = table->freeRows->index;
        //Free the queued row and place next element first.
        table->freeRows = db_free_freeRow(table->freeRows);
    }


    //If no deleted row already available.
    if(!newRow){
            table->nrOfRows++;

        /*First row ID to be added?*/
        if(table->nrOfRows-1==0){
            printf("First to be added\n");
            table->row_ID = allocateBytes(sizeof(char *));
            table->dirty_rows = (bool *)allocateBytes(sizeof(bool));
            table->delete_rows = (bool *)allocateBytes(sizeof(bool));
        }
        else{
            reAllocateBytes((void **) &table->row_ID, table->nrOfRows*sizeof(char *));
            reAllocateBytes((void **) &table->dirty_rows, table->nrOfRows*sizeof(bool));
            reAllocateBytes((void**) &table->delete_rows, table->nrOfRows*sizeof(bool));
        }
        /*Allocate and set row ID*/
        table->row_ID[table->nrOfRows-1] = allocateBytes(37*sizeof(char));
    }

    uuid_t uuid;

    // generate row ID
    uuid_generate_random(uuid);
    //save it as a string
    uuid_unparse(uuid, table->row_ID[freeRowIndex]);

    table->dirty_rows[freeRowIndex] = true;
    table->delete_rows[freeRowIndex] = false;

    for(int i=0; i<nrOfColumns; i++){
        /*Find correct column*/
        Column *col = findColumn(table, columns[i]);

        if(!col){
            return COLUMN_NOT_FOUND;
        }

        ret = db_insertElem(col, elements[i], freeRowIndex, newRow);
        if(ret!=SUCCESS){
            return ret;
        }
    }


    return SUCCESS;
}

/*
Function: Insert element into specified table column.

Parameters:
    Database *db, database pointer
    Name table, the name of the table to add row to.
    Name columns, the column to add element to.
    Element elements, the element.

Returns:
    SUCCESS or DUPLICATE_TABLE
*/
int db_insertElem(Column *column, Element element, int freeRowIndex, bool newRow){

    //Is there a deleted row available?
    //Else allocate space for a new row
    /*Malloc or realloc?*/
    if(newRow && column->elements==NULL){
        column->elements = allocateBytes(sizeof(Value *));
    }
    else if(newRow){
        reAllocateBytes((void **) &column->elements,(freeRowIndex)*sizeof(Value *));
    }

    //Allocate space for element and write value
    column->elements[freeRowIndex] = allocateBytes(sizeof(Value));
    column->elements[freeRowIndex]->elem = strdupErrorChecked(element);

    return SUCCESS;
}

int db_select(){
    return 0;
}

/*select from t*/
int db_deleteWhere(Name table, Name *columnsToMatch, int nrOfColumns, Name *valuesToMatch, Name *columnToReturn){
    return -1;
}


int db_deleteRows(Database *db, char **rowID, int nrOfRows, Name tableName){
    Table* table = findTable(db, tableName);

    if(!table){
        return TABLE_NOT_FOUND;
    }


    for(int i=0; i < nrOfRows; i++){
        printf("%s\n", rowID[i]);
        int ret = db_deleteRow(table, rowID[i]);
        if(ret==ROWID_NOT_FOUND){
            printf("row id not found\n");
            //do something :)
        }
    }
    return SUCCESS;
}

int db_deleteRow(Table *table, char *rowID){
    
    int rowIndex = finRowInd(table, rowID);

    if(rowIndex==ROWID_NOT_FOUND){
        return ROWID_NOT_FOUND;
    }

    table->delete_rows[rowIndex] = true;
    table->row_ID[rowIndex] = '\0';

    for(int i=0; i<table->nrOfColumns; i++){
        db_free_value(table->columns[i]->elements[rowIndex]);
        table->columns[i]->elements[rowIndex]=NULL;
    }

    //Insert deleted row in queue for reuse when inserting.
    FreeRows *freeRow = allocateBytes(sizeof(FreeRows));
    freeRow->next = table->freeRows;
    freeRow->index = rowIndex;
    table->freeRows = freeRow;

    return SUCCESS;
}

int db_close(Database **db){
    int ret = db_free_database(db);
}

//Returns array index of table with name TableName

Table* findTable(Database *db, Name tableName){
    for(int i=0; i<db->nrOfTables; i++){
        if(!strcmp(db->tables[i]->name,tableName)){
            return db->tables[i];
        }
    }
    return NULL;
}

//Returns array index of column with name columnName
Column* findColumn(Table *table, Name columnName){
    for(int i=0; i<table->nrOfColumns; i++){
        if(!strcmp(table->columns[i]->name,columnName)){
            return table->columns[i];
        }
    }
    return NULL;
}

int finRowInd(Table *table, char *rowID){
    for(int i=0; i< table->nrOfRows; i++){
        printf("\n%s ZXXX\n", rowID);
        if(rowID==NULL){
            printf("WOWW");
        }
        strlen(rowID);
        /*Match row ID*/
        if(strlen(table->row_ID[i]) == strlen(rowID) && !strcmp(table->row_ID[i], rowID)){
            return i;
        }
    }

    return ROWID_NOT_FOUND;
}

int db_free_database(Database **db){
    free((*db)->name);

    for(int i=0; i<(*db)->nrOfTables; i++){
        db_free_table((*db)->tables[i]);
        free((*db)->tables[i]);
    }
    
    free((*db)->tables);
    free(*db);

    return 0;
}

int db_free_table(Table *table){
    free(table->name);

    for(int i=0; i<table->nrOfColumns; i++){
        db_free_column(table->columns[i], table->nrOfRows, table->dirty_rows);
        free(table->columns[i]);
    }

    free(table->columns);

    for(int i=0; i<table->nrOfRows; i++){
        free(table->row_ID[i]);
    }

    free(table->row_ID);
    free(table->delete_rows);
    free(table->dirty_rows);

    return 0;
}

int db_free_column(Column *column, int nrOfRows, bool *delete_rows){
    free(column->name);

    for(int i=0; i < nrOfRows; i++){
        if(delete_rows[i]==false){
            db_free_value(column->elements[i]);
        }
        free(column->elements[i]);
    }

    free(column->elements);
    return 0;
}

int db_free_value(Value *value){
    free(value->elem);
    return 0;
}

FreeRows* db_free_freeRow(FreeRows *freeRow){
    FreeRows *next = freeRow->next;
    free(freeRow);
    return next;
}

void* allocateBytes(int nrOfBytes){
    void* retPointer = malloc(nrOfBytes);

    if(!retPointer){
        fprintf(stderr, "Failed to allocate %d bytes, exiting....", nrOfBytes);
        exit(0);
    }
    return retPointer;
}

void reAllocateBytes(void** memory,int nrOfBytes){
    *memory = realloc(*memory, nrOfBytes);

    if(!memory){
        fprintf(stderr, "Failed to reallocate %d bytes, exiting....", nrOfBytes);
        exit(0);
    }
}

char* strdupErrorChecked(const char* str){
    char* duplicate = strdup(str);

    if(!duplicate){
        fprintf(stderr, "Failed to run strdup on string \"%s\", exiting....", str);
        exit(0);
    }
    return duplicate;
}
