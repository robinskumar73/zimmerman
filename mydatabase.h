#ifndef MYDATABASE_H
#define MYDATABASE_H

#include "vector"
#include "iostream"
#include "sqlite3.h"
#include "string.h"
#include "stdio.h"
#include "sstream"




#define RIGHT 'right'
#define LEFT  'left'
#define CENTER 'center'

#define NUMBER 'Number'
#define RTL 'RTL'
#define LTR 'LTR'



class MyDatabase
{
    public:
        sqlite3* createDatabase(const char* database_name );
        void createTables(sqlite3 *db);
        void insertValues(const char *sql,sqlite3 *db);
        void closeDbConnection(sqlite3 *db)
        {
            sqlite3_close(db);
        }



    private:
        void createGlobalFieldTable(sqlite3 *db);
        void createFormsTable(sqlite3 *db);
        void createFormFieldsTable(sqlite3 *db);
        static int callback(void *NotUsed, int argc, char **argv, char **azColName){

            int i;
            for(i=0; i<argc; i++){
                std::cout<< azColName[i] << " = " << argv[i] ? argv[i] : "NULL" ;
            }
            std::cout<< "\n";
            return 0;
        }
};
//Defigning type for global type
class GlobalTable
{
    public:
        static std::string tableName ;
         std::string id = "NULL" ;
         std::string field_code = "NULL";
         std::string field_desc = "NULL";
         std::string field_type = "NULL";
         std::string field_size = "NULL" ;
         std::string field_demo = "NULL";

};

std::string GlobalTable::tableName = "global_fields";




//Type for Forms table..
class Forms
{
    public:
        static std::string tableName ;
        std::string id = "NULL";
        std::string form_code = "NULL";
        std::string form_dir = "NULL";
        std::string form_desc  = "NULL";
        std::string form_orig = "NULL";
        std::string form_URL = "NULL";

};
std::string Forms::tableName = "forms";




//Type for Forms table..
class Form_Fields
{
    public:
        static std::string tableName;
        std::string id = "";
        std::string form_code = "NULL";
        std::string field_name = "NULL";
        std::string pdf_page_number  = "NULL";
        std::string field_X_position_in_page = "NULL";
        std::string field_Y_position_in_page = "NULL";
        std::string field_width = "NULL";
        std::string field_height = "NULL";
        std::string field_font = "NULL";
        std::string field_font_size = "NULL";
        std::string number_of_characters = "NULL";
        std::string align = "NULL";
        std::string field_type = "NULL";

};

std::string  Form_Fields::tableName = "form_fields";

//Class for storing value of table..
class TableValueStore
{
public:
    int length = 0;

    //function for global table
    std::vector<GlobalTable> globalTabularData;
    //Iterator position..
    std::vector<GlobalTable>::iterator global_it;
    void setValue( GlobalTable gt );
    GlobalTable getValue_global(unsigned position);

    //properties and methods for table  Forms
    //function for Forms table
    std::vector<Forms> formsTabularData;
    //Iterator position..
    std::vector<Forms>::iterator forms_it;
    void setValue( Forms gt );
    Forms getValue_forms(unsigned position);

    //properties and methods for table  Forms
    //function for Form_Fields table
    std::vector<Form_Fields> form_field_TabularData;
    //Iterator position..
    std::vector<Form_Fields>::iterator form_field_it;
    void setValue( Form_Fields gt );
    Form_Fields getValue_form_fields(unsigned position);
};


//Table for performing the global table operations..
class GlobalTableOperations
{

public:
    sqlite3 *dbCursor;
    //creating objects of TableValueStore
    TableValueStore globalValueStore;
    static int callback(void *data, int argc, char **argv, char **azColName);
    //Select method of global table.. return type of TableValueStore write whereString without using where
    TableValueStore Select(  std::string whereString );
    int Insert( GlobalTable gt );
    //Select method of global table.. return type of TableValueStore write whereString without using where
    int Delete(  std::string whereString  );
    int Update( GlobalTable gt, std::string whereString );
    static int local_callback(void *NotUsed, int argc, char **argv, char **azColName){
        int i;
        for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
    printf("\n");
    return 0;
    }
     //Constructor...
    GlobalTableOperations( sqlite3 *db )
    {
        dbCursor = db;
    }

    private:
        std::string sql_statement = "SELECT * FROM global_fields ";

};



//-----------------------------------------------------AREA FOR FORM_FIELDS-----------------------------------------------------
class Form_FieldsTableOperations
{

public:
    sqlite3 *dbCursor;
    //creating objects of TableValueStore
    TableValueStore form_fieldsValueStore;
    static int callback(void *data, int argc, char **argv, char **azColName);
    //Select method of global table.. return type of TableValueStore write whereString without using where
    TableValueStore Select(  std::string whereString );
    int Insert( Form_Fields gt );
    //Select method of global table.. return type of TableValueStore write whereString without using where
    int Delete(  std::string whereString  );
    int Update( Form_Fields gt, std::string whereString );
    static int local_callback(void *NotUsed, int argc, char **argv, char **azColName){
        int i;
        for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
    printf("\n");
    return 0;
    }
     //Constructor...
    Form_FieldsTableOperations( sqlite3 *db )
    {
        dbCursor = db;
    }

    private:
        std::string sql_statement = "SELECT * FROM form_fields ";

};

//-------------------------------------AREA ENDS FOR FORM_FIELDS------------------------------------------------


//---------------------------------------AREA FORM FIELDS-------------------------------------------------------


class FormsTableOperations
{

public:
    sqlite3 *dbCursor;
    //creating objects of TableValueStore
    TableValueStore formsValueStore;
    static int callback(void *data, int argc, char **argv, char **azColName);
    //Select method of global table.. return type of TableValueStore write whereString without using where
    TableValueStore Select(  std::string whereString );
    int Insert( Forms gt );
    //Select method of global table.. return type of TableValueStore write whereString without using where
    int Delete(  std::string whereString  );
    int Update( Forms gt, std::string whereString );
    static int local_callback(void *NotUsed, int argc, char **argv, char **azColName){
        int i;
        for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }
    printf("\n");
    return 0;
    }
     //Constructor...
    FormsTableOperations( sqlite3 *db )
    {
        dbCursor = db;
    }

    private:
        std::string sql_statement = "SELECT * FROM forms ";

};


//-------------------------------------AREA FOR FORM ENDS HERE-------------------------------------------------


#endif // MYDATABASE_H
