#include <database.h>

int main()
{
    //Database myOwn;
    printf("Hello world!\n");

    //gcc example.c -o example -std=gnu99 -ldatabase

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