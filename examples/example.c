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
    printf("Datbase name: %s\n",db->name);

    int ec;

    Name tbNames[] = {"table1","table2","table3"};
    ec = db_AddTables(db, tbNames, 3);

    int ret = db_AddColumns(db,"table1",columns,3,typess);
    
    for(int i=0; i < 30; i++){
        ret = db_insert(db, "table1", columns, 3, elems);
    }

    printf("Tableindex of %s: %d\n%s: %d\n%s: %d\n",
        tbNames[0], tableIndex(db, tbNames[0]),
        tbNames[1], tableIndex(db, tbNames[1]),
        tbNames[2], tableIndex(db, tbNames[2])
        );

    db_deleteRows(db, db->tables[0]->row_ID, 3, tbNames[0]);

    printf("Free elems:%d\n", db->tables[0]->free_elems);
    printf("Free elems:%d %d\n", db->tables[0]->nrOfRows, true);

    /*printf("ret%d\n",ret);
    printf("Column type:%d\n", db->tables[0]->columns[2]->type);
    printf("Column type:%d\n", db->tables[0]->columns[1]->type);
    printf("Column type:%d\n", db->tables[0]->columns[0]->type);
    printf("Column name:%s\n", db->tables[0]->columns[2]->name);
    printf("Column name:%s\n", db->tables[0]->columns[1]->name);
    printf("Column name:%s\n", db->tables[0]->columns[0]->name);
    printf("%d ret\n", ret);
    printf("%d ret\n", ret);
    printf("nrElem%d\n",db->tables[0]->nrOfRows);
    printf("\nelemVal:%s\n", db->tables[0]->columns[0]->elements[1]->elem);
    printf("\nelemVal:%s\n", db->tables[0]->columns[2]->elements[1]->elem);

    printf("%s\n", db->tables[0]->row_ID[0]);*/
    //db_free_database(&db);
    db_close(&db);
    return 0;
}