#include <database.h>

int main()
{
    printf("Hello world!\n");

    //gcc example.c -o example -std=gnu99 -ldatabase

    Name columns[3] = {"column1","column2","column3"};
    Element elems[3] = {"elem1","elem2","elem3"};
    Type typess[3] = {DB_STRING,DB_STRING,DB_STRING};

    Database *db;

    db_Create(&db, "Databasen");
    printf("%s",db->name);

    int ec;

    Name tbNames[] = {"table1","table2","table3"};
    ec = db_AddTables(db, tbNames, 3);
    //printf("%s",db->tables[0]->name);

    int ret = db_AddColumns(db,"table1",columns,3,typess);
    
    for(int i=0; i < 2000000; i++){
        ret = db_insert(db, "table1", columns, 3, elems);
    }

    db_free_database(&db);

    return 0;
    printf("ret%d",ret);
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
    printf("%d ret", ret);
    //db_insert()
    //printf("nrElem%d",db->tables[0]->columns[0]->nrOfElements);
    //printf("\nelemVal:%s", db->tables[0]->columns[0]->elements[1]->elem);
    //printf("\nelemVal:%s", db->tables[0]->columns[2]->elements[1]->elem);

    printf("%s", db->tables[0]->row_ID[0]);
    //db_close(&db);

    return 0;
}