#include <engine.h>
#include <error_codes.h>

int mains()
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

    int ec;

    Name tbNames[] = {"Apa","Fisk","Ko"};
    ec = db_AddTables(db, tbNames, 3);
    //printf("%s",db->tables[0]->name);

    //db_AddColumn(db,"Apa","fjong", 1);

    int ret = db_AddColumns(db,"Apa",columns,3,typess);
    
    for(int i=0; i < 2000000; i++){
        ret = db_insert(db, "Apa", columns, 3, elems);
    }

    db_free_database(&db);

    return 0;
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

    printf("%d ret", ret);
    //ret = db_insert(db, "Apa", columns, 3, elems);
    printf("%d ret", ret);
    //db_insert()
    //printf("nrElem%d",db->tables[0]->columns[0]->nrOfElements);
    //printf("\nelemVal:%s", db->tables[0]->columns[0]->elements[1]->elem);
    //printf("\nelemVal:%s", db->tables[0]->columns[2]->elements[1]->elem);

    printf("%s", db->tables[0]->row_ID[0]);
    //db_close(&db);

    return 0;
}

void db_Create(Database **db, Name db_Database_Name){

    *db = (Database *) allocateBytes(sizeof(Database));

    //(*db)->name = strdup((const char *) db_Database_Name);
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
    (Re)allocate rows to deleted array and initialise added element to false
    (Re)allocate rows to dirty array and initialise added element to false
    */
    for(int i=0; i < db->nrOfTables;i++) {
        if(strcmp(db->tables[i]->name, table)==0){
            db->tables[i]->nrOfRows++;

            /*First row ID to be added?*/
            if(db->tables[i]->row_ID==NULL){
                db->tables[i]->row_ID = allocateBytes(sizeof(char *));
            }
            else{
                reAllocateBytes((void **) &db->tables[i]->row_ID, db->tables[i]->nrOfRows*sizeof(char *));
            }
            /*Allocate and set row ID*/
            db->tables[i]->row_ID[db->tables[i]->nrOfRows-1] = allocateBytes(37*sizeof(char));

            uuid_t uuid;

            // generate row ID
            uuid_generate_random(uuid);
            //save it as a string
            uuid_unparse(uuid, db->tables[i]->row_ID[db->tables[i]->nrOfRows-1]);

            /*(Re)allocate dirty row array and initialize to false for the inserted row*/
            if(db->tables[i]->dirty_rows==NULL){
                db->tables[i]->dirty_rows = (bool *)allocateBytes(sizeof(bool));
            }
            else{
                reAllocateBytes((void **) &db->tables[i]->dirty_rows, db->tables[i]->nrOfRows*sizeof(bool));
            }
            db->tables[i]->dirty_rows[db->tables[i]->nrOfRows-1] = true;

            /*(Re)allocate delete row array and set to false for the inserted row*/
            if(db->tables[i]->delete_rows==NULL){
                db->tables[i]->delete_rows = (bool *)allocateBytes(sizeof(bool));
            }
            else{
                reAllocateBytes((void**) &db->tables[i]->delete_rows, db->tables[i]->nrOfRows*sizeof(bool));
            }
            db->tables[i]->delete_rows[db->tables[i]->nrOfRows-1] = false;

            break;

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
                        db->tables[i]->columns[e]->elements = allocateBytes(sizeof(Value *));
                    }
                    else{
                        reAllocateBytes((void **) &db->tables[i]->columns[e]->elements,nrOfRows*sizeof(Value *));
                    }


                    //Allocate space for element and write value
                    db->tables[i]->columns[e]->elements[nrOfRows-1] = allocateBytes(sizeof(Value));
                    db->tables[i]->columns[e]->elements[nrOfRows-1]->elem = strdupErrorChecked(element);

                    return SUCCESS;

                }
            }
            return COLUMN_NOT_FOUND;

        }
    }
    return TABLE_NOT_FOUND;
}

int db_select(){
    return 0;
}

int db_deleteRows(Database *db, char **rowID, int nrOfRows, Name table){
    int tableIndex = -1;
    int ret;
    for(int i=0; i < db->nrOfTables;i++) {
        if(strcmp(db->tables[i]->name, table)==0){
            tableIndex = i;
            break;
        }
    }

    if(tableIndex==-1){
        return TABLE_NOT_FOUND;
    }

    for(int i=0; i < nrOfRows; i++){
        ret = db_deleteRow(db, rowID[i], table, tableIndex);
        if(ret==COLUMN_NOT_FOUND){
            //do something :)
        }
    }
    return SUCCESS;
}

int db_deleteRow(Database *db, char *rowID, Name table, int tableIndex){
    for(int i=0; i<db->tables[i]->nrOfRows; i++){
        /*Match row ID*/
        if(strcmp(db->tables[tableIndex]->row_ID[i], rowID)==0){
            db->tables[tableIndex]->delete_rows[i] = true;
            db->tables[tableIndex]->dirty_rows[i] = true;
            return SUCCESS;
        }
    }
    return ROWID_NOT_FOUND;

}


/*select from t*/
int db_deleteWhere(Name table, Name *columnsToMatch, int nrOfColumns, Name *valuesToMatch, Name *columnToReturn){
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
        db_free_column(table->columns[i], table->nrOfRows);
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

int db_free_column(Column *column, int nrOfRows){
    free(column->name);
    for(int i=0; i < nrOfRows; i++){
        db_free_value(column->elements[i]);
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
