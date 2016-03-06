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
int db_AddColumn(Database *db, Name table, Name column, Type columnType){
    for(int i=0; i < db->nrOfTables; i++){
        /*Match on correct table*/
        if((strcmp(db->tables[i]->name, table))==0){
            db->tables[i]->nrOfColumns += 1;

            int nrOfColumns = db->tables[i]->nrOfColumns;
            /*First time allocating memory for a column in the table?*/
            if(nrOfColumns==1){
                db->tables[i]->columns = allocateBytes(nrOfColumns*sizeof(Column *));
            }
            /*Else reallocate the list of pointers*/
            else{
                reAllocateBytes((void **) &db->tables[i]->columns,nrOfColumns*sizeof(Column *));
            }

            //Allocate space for the column and set type
            db->tables[i]->columns[nrOfColumns-1] = allocateBytes(sizeof(Column));
            db->tables[i]->columns[nrOfColumns-1]->type = columnType;
            db->tables[i]->columns[nrOfColumns-1]->name = strdupErrorChecked(column);
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
int db_insert(Database *db, Name table, Name *columns, int nrOfColumns, Element *elements){
    int ret;
    int freeRowIndex;
    int tableInd = tableIndex(db, table);
    bool newRow = false;
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

    /*
    Update number of rows
    (Re)allocate and set row ID
    (Re)allocate rows to deleted array and initialise added element to false
    (Re)allocate rows to dirty array and initialise added element to false
    */
    for(int i=0; i < db->nrOfTables;i++) {
        if(strcmp(db->tables[i]->name, table)==0){

            int nrOfRows = db->tables[i]->nrOfRows;
            freeRowIndex = nrOfRows; //Initially place free row to be last element + 1

            //Is there any deleted row we can use available, if so find it's index.
            if(nrOfRows>=1 && db->tables[i]->free_elems>0){
                for(unsigned int x=0; x < db->tables[i]->free_elems; x++){
                    if(db->tables[i]->delete_rows[x]==true){
                        freeRowIndex=x;
                        db->tables[i]->free_elems--;
                        newRow = true;
                    }
                }
            }

            //If no deleted row already available.
            if(freeRowIndex==db->tables[i]->nrOfRows){
                    db->tables[i]->nrOfRows++;

                /*First row ID to be added?*/
                if(db->tables[i]->nrOfRows-1==0){
                    db->tables[i]->row_ID = allocateBytes(sizeof(char *));
                    db->tables[i]->dirty_rows = (bool *)allocateBytes(sizeof(bool));
                    db->tables[i]->delete_rows = (bool *)allocateBytes(sizeof(bool));
                }
                else{
                    reAllocateBytes((void **) &db->tables[i]->row_ID, db->tables[i]->nrOfRows*sizeof(char *));
                    reAllocateBytes((void **) &db->tables[i]->dirty_rows, db->tables[i]->nrOfRows*sizeof(bool));
                    reAllocateBytes((void**) &db->tables[i]->delete_rows, db->tables[i]->nrOfRows*sizeof(bool));
                }
                /*Allocate and set row ID*/
                db->tables[i]->row_ID[db->tables[i]->nrOfRows-1] = allocateBytes(37*sizeof(char));
            }

            uuid_t uuid;

            // generate row ID
            uuid_generate_random(uuid);
            //save it as a string
            uuid_unparse(uuid, db->tables[i]->row_ID[freeRowIndex]);

            db->tables[i]->dirty_rows[freeRowIndex] = true;
            db->tables[i]->delete_rows[freeRowIndex] = false;
            break;
        }
    }

    for(int i=0; i<nrOfColumns; i++){
        /*Find correct column*/
        int columnInd = columnIndex(table, columns[i]);
        if(columnInd==-1){
            return COLUMN_NOT_FOUND;
        }

        Column column = db->tables[tableInd]->columns[columnInd];
        ret = db_insertElem(column, elements[i], freeRowIndex, newRow);
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
        freeRowIndex = nrOfRows;
    }
    else if(newRow){
        reAllocateBytes((void **) &column->elements,(nrOfRows+1)*sizeof(Value *));
        freeRowIndex = nrOfRows;
    }


    //Allocate space for element and write value
    column->elements[freeRowIndex] = allocateBytes(sizeof(Value));
    column->columns[columnInd]->elements[freeRowIndex]->elem = strdupErrorChecked(element);

    return SUCCESS;
}

int db_select(){
    return 0;
}

/*select from t*/
int db_deleteWhere(Name table, Name *columnsToMatch, int nrOfColumns, Name *valuesToMatch, Name *columnToReturn){
    return -1;
}


int db_deleteRows(Database *db, char **rowID, int nrOfRows, Name table){
    int tableInd = tableIndex(db, table);
    if(tableInd==-1){
        printf("Table %s does not exist", table);
        return TABLE_NOT_FOUND;
    }


    for(int i=0; i < nrOfRows; i++){
        int ret = db_deleteRow(db->tables[tableInd], rowID[i]);
        if(ret==ROWID_NOT_FOUND){
            //do something :)
        }
    }
    return SUCCESS;
}

int db_deleteRow(Table *table, char *rowID){
    for(int i=0; i< table->nrOfRows; i++){

        /*Match row ID*/
        if(!strcmp(table->row_ID[i], rowID)){
            table->delete_rows[i] = true;
            table->row_ID[i] = "";
            for(int ii=0; ii<table->nrOfColumns; ii++){
                db_free_value(table->columns[ii]->elements[i]);
                table->columns[ii]->elements[i]=NULL;
            }

            table->free_elems++;
            return SUCCESS;
        }
    }
    return ROWID_NOT_FOUND;

}

int db_close(Database **db){
    int ret = db_free_database(db);
}

//Returns array index of table with name TableName
int tableIndex(Database *db, Name tableName){
    for(int i=0; i<db->nrOfTables; i++){
        if(!strcmp(db->tables[i]->name,tableName)){
            return i;
        }
    }
    return -1;
}

//Returns array index of column with name columnName
int columnIndex(Table *db, Name columnName){
    for(int i=0; i<table->nrOfColumns; i++){
        if(!strcmp(table->columns[i]->name,columnName)){
            return i;
        }
    }
    return -1;
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
