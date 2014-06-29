#include "mydatabase.h"
#include <string>

sqlite3 * MyDatabase::createDatabase(const char* database_name)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(database_name, &db);

    if( rc )
    {
        std::cout<<"Error creating database";
    }
    else{
        std::cout<<"succesfully created database\n";
    }

    return db;
}


void MyDatabase::createGlobalFieldTable(sqlite3 *db)
{
    char *sql;
    char *zErrMsg = 0;
    int rc;

    /* Create SQL statement */
   sql = "CREATE TABLE global_fields("  \
         "id INTEGER PRIMARY KEY   AUTOINCREMENT  NOT NULL," \
         "field_code TEXT(20) NOT NULL," \
         "field_desc TEXT(40) NULL,"\
         "field_type TEXT(10) NOT NULL,"\
         "field_size INT(999) NULL,"\
         "field_demo TEXT(80) NOT NULL );";

    /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc == SQLITE_OK )
    {
        std::cout<< "Succesfully created GLOBAL_TABLE table\n";

    }
   else
    {
      std::cout<< "\nError: global_field\n"<<zErrMsg;
    }
    sqlite3_free(zErrMsg);
}


void MyDatabase::createFormsTable(sqlite3 *db)
{
     char *sql;
     char *zErrMsg = 0;
     int rc;

    /* Create SQL statement */
   sql = "CREATE TABLE forms("  \
         "id INTEGER PRIMARY KEY  AUTOINCREMENT   NOT NULL," \
         "form_code INT(999999) NOT NULL," \
         "form_dir  TEXT(40) NOT NULL,"\
         "form_desc TEXT(40) NULL,"\
         "form_orig CHAR(1) NOT NULL,"\
         "form_url  TEXT(200) NOT NULL );";

    /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc == SQLITE_OK )
    {
        std::cout<< "Succesfully created FORMS table\n";

    }
   else
    {
      std::cout<< "\nERROR Table FORMS created successfully\n"<< zErrMsg;
      sqlite3_free(zErrMsg);
    }

}



void MyDatabase::createFormFieldsTable(sqlite3 *db)
{

     char *sql;
     char *zErrMsg = 0;
     int rc;

    /* Create SQL statement */
   sql = "CREATE TABLE form_fields("  \
         "id INTEGER PRIMARY KEY  AUTOINCREMENT   NOT NULL," \
         "form_code INT(999999) NOT NULL," \
         "field_name  TEXT NULL,"\
         "pdf_page_number TEXT NULL,"\
         "field_x_position TEXT  NULL,"\
         "field_y_position TEXT  NULL,"\
         "field_width INT ,"\
         "field_height INT,"\
         "field_font TEXT ,"\
         "field_font_size INT ,"\
         "number_of_characters INT,"\
         "align  CHAR ,"\
         "field_type CHAR );";

    /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

   if( rc == SQLITE_OK )
    {
        std::cout<< "Succesfully created FORMS_FIELDS table\n";
        sqlite3_free(zErrMsg);
    }
   else
    {
      std::cout<< "ERROR::Table FORMS_FIELDS created successfully\n";
    }
}

void MyDatabase::createTables(sqlite3 *db)
{

    createGlobalFieldTable(db);
    createFormsTable(db);
    createFormFieldsTable(db);

}


//-------------------------------------------------------------------------------
//Defigning library functions for GLOBAL TABLE
void TableValueStore::setValue( GlobalTable gt )
{
        //Getting the end point of the data..
        global_it = globalTabularData.end();
        globalTabularData.insert ( global_it , gt );
        length++;

}

GlobalTable TableValueStore::getValue_global(unsigned position)
 {
        std::vector<GlobalTable>::size_type sz = globalTabularData.size();
        if(position<sz)
        {
           return globalTabularData[position];
        }
        else
        {
            std::cout<<"Error:Position cannot be greater than size of array table\n ";
        }

 }

//Defigning library functions for Forms TABLE
void TableValueStore::setValue( Forms gt )
{
        //Getting the end point of the data..
        forms_it = formsTabularData.end();
        formsTabularData.insert ( forms_it , gt );
        length++;

}

Forms TableValueStore::getValue_forms(unsigned position)
 {
        std::vector<Forms>::size_type sz = formsTabularData.size();
        if(position<sz)
        {
           return formsTabularData[position];
        }
        else
        {
            std::cout<<"Error:Position cannot be greater than size of array table\n ";
        }

 }


//Defigning library functions for Form_Fields TABLE
void TableValueStore::setValue( Form_Fields gt )
{
        //Getting the end point of the data..
        form_field_it = form_field_TabularData.end();
        form_field_TabularData.insert ( form_field_it , gt );
        length++;

}

Form_Fields TableValueStore::getValue_form_fields(unsigned position)
 {
        std::vector<Form_Fields>::size_type sz = form_field_TabularData.size();
        if(position<sz)
        {
           return form_field_TabularData[position];
        }
        else
        {
            std::cout<<"Error:Position cannot be greater than size of array table\n ";
        }

 }

 //GLobalTableOperation class
 int GlobalTableOperations::callback(void *data, int argc, char **argv, char **azColName)
    {
        //Converting the object back to this object form
        GlobalTableOperations* obj = (GlobalTableOperations*) data;
        //Callback function for storing each column of a row..
        int i;
        //Creating instance of globalTable class
        GlobalTable gt;
        for(i=0; i<argc; i++)
        {
            if(  std::string(azColName[i]) == "field_code")
            {
                gt.field_code = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "field_desc")
            {
                gt.field_desc = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "field_type")
            {
                gt.field_type = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "field_size")
            {
                gt.field_size = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "field_demo")
            {
                gt.field_demo = argv[i] ? argv[i] : "NULL" ;
            }
             else if(std::string(azColName[i]) == "id")
            {
                gt.id = argv[i] ? argv[i] : "NULL" ;
            }
            else{ /*Do nothing*/ }

        }
        //Now set Value to an array..
        obj->globalValueStore.setValue(gt);
        return 0;
    }


//Select method of global table.. return type of TableValueStore write whereString without using where
TableValueStore  GlobalTableOperations::Select(  std::string whereString = "NULL" )
{
        if (whereString != "NULL")
        {
            std::ostringstream oss;
            oss<< "SELECT * FROM " << GlobalTable::tableName << " WHERE " << whereString;
            this->sql_statement = oss.str();
        }
        else{ this->sql_statement = "SELECT * FROM " + GlobalTable::tableName ; }


        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor, this->sql_statement.c_str(), callback, (void*) this, &zErrMsg);
         if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued GLOBAL_TABLE statement\n";

        }
        else
        {
            std::cout<< "\nError: global_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);

        std::cout<< this->sql_statement;
        return globalValueStore;

}


//Insert method of global table..
//gt id name in string is compulsary..
int GlobalTableOperations::Insert( GlobalTable gt )
{
        std::ostringstream oss;
        oss << "INSERT INTO " \
         << GlobalTable::tableName \
         << " ( id " \
         << ( gt.field_code != "NULL" ? ", field_code " : "" ) \
         << ( gt.field_desc != "NULL" ? ", field_desc " : "" ) \
         << ( gt.field_type != "NULL" ? ", field_type " : "" ) \
         <<  ", field_size " \
         << ( gt.field_demo != "NULL" ? ", field_demo " : "" ) \
         <<  " ) VALUES (  "  \
         << gt.id   \
         << ( gt.field_code != "NULL" ? ", '" + gt.field_code + "' " : "" ) \
         << ( gt.field_desc != "NULL" ? ", '" + gt.field_desc + "' " : "" ) \
         << ( gt.field_type != "NULL" ? ", '" + gt.field_type + "' " : "" ) \
         <<  ", " + gt.field_size \
         << ( gt.field_demo != "NULL" ? " , '" + gt.field_demo + "' "  : "" ) \
         << ");" ;
        this->sql_statement = oss.str();

        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor,this->sql_statement.c_str(), this->local_callback, 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued GLOBAL_TABLE statement\n";

        }
        else
        {
            std::cout<< "\nError: inserting to global_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);
        return rc;
}


   //Select method of global table.. return type of TableValueStore write whereString without using where
int GlobalTableOperations::Delete(  std::string whereString  )
{
        this->sql_statement = "DELETE FROM " + GlobalTable::tableName +" WHERE  " + whereString ;

        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor, this->sql_statement.c_str(), local_callback , 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued GLOBAL_TABLE delete statement\n";

        }
        else
        {
            std::cout<< "\nError: global_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);
        return rc;
}


int GlobalTableOperations::Update( GlobalTable gt, std::string whereString )
{

        int initial_counter = 0;
        std::string set_stat = "";
        std::string stat;
        std::string update_stat = "UPDATE "  + GlobalTable::tableName + " set ";
        if(gt.field_code != "NULL")
        {
            /* Create merged SQL statement */


            if (initial_counter == 0)
            {
                   set_stat = set_stat + std::string("field_code =  '") + gt.field_code + "'" ;
                   initial_counter = 1;
            }
            else
            {
                   set_stat = set_stat + ", field_code =  '" + gt.field_code + "'" ;
            }
        }
        if(gt.field_demo != "NULL")
        {

            if(initial_counter == 0)
            {
                /* Create merged SQL statement */
                set_stat = set_stat + " field_demo =  '" + gt.field_demo + "'";
                initial_counter = 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat = set_stat + ", field_demo =  '" + gt.field_demo + "'";
            }


        }
        if(gt.field_desc != "NULL")
        {
            if(initial_counter == 0)
            {
                /* Create merged SQL statement */
                set_stat = set_stat + " field_desc =  '" + gt.field_desc + "'";
                initial_counter = 1;
            }
            else
            {
                /* Create merged SQL statement */
                 set_stat = set_stat + ", field_desc =  '" + gt.field_desc + "'";
            }


        }
        if( gt.field_size != "NULL" )
        {
            //std::stringstream sstm;
            if(initial_counter == 0)
            {

                /* Create merged SQL statement */
                set_stat = set_stat + " field_size =   " +   gt.field_size ;
                initial_counter = 1;

            }
            else
            {
                               /* Create merged SQL statement */
                set_stat =  set_stat + ", field_size =   " +   gt.field_size ;
            }


        }

        if( gt.id != "NULL" )
        {

            if(initial_counter == 0)
            {
                set_stat = set_stat + " id =  " + gt.id;
                initial_counter == 1;

            }
            else
            {
                /* Create merged SQL statement */
                set_stat = set_stat + ", id =  " + gt.id;

            }



        }

        if( gt.field_type != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " field_type = '" + gt.field_type + "'";
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", field_type = '" + gt.field_type + "'";
            }


        }

        this->sql_statement = "";
        this->sql_statement = this->sql_statement + update_stat + set_stat + " WHERE " + whereString;
        //Printing statement..
        std::cout<< "hey \n "<<this->sql_statement.c_str();
        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor,this->sql_statement.c_str(), local_callback , 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully updated GLOBAL_TABLE UPDATE statement\n";

        }
        else
        {
            std::cout<< "\nError: global_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);

        return rc;

}


//-----------------------------------------------------------------AREA FOR FORM_FIELDS TABLE--------------------------------------------
 //Form_Fields TableOperation class
 int Form_FieldsTableOperations::callback(void *data, int argc, char **argv, char **azColName)
    {
        //Converting the object back to this object form
        Form_FieldsTableOperations* obj = (Form_FieldsTableOperations*) data;
        //Callback function for storing each column of a row..
        int i;
        //Creating instance of globalTable class
        Form_Fields gt;
        for(i=0; i<argc; i++)
        {
            if(  std::string(azColName[i]) == "id")
            {
                gt.id = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "form_code")
            {
                gt.form_code = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "field_name")
            {
                gt.field_name = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "pdf_page_number")
            {
                gt.pdf_page_number = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "field_X_position_in_page")
            {
                gt.field_X_position_in_page = argv[i] ? argv[i] : "NULL" ;
            }
             else if(std::string(azColName[i]) == "field_Y_position_in_page")
            {
                gt.field_Y_position_in_page = argv[i] ? argv[i] : "NULL" ;
            }
             else if(std::string(azColName[i]) == "field_width")
            {
                gt.field_width = argv[i] ? argv[i] : "NULL" ;
            }
             else if(std::string(azColName[i]) == "field_height")
            {
                gt.field_height = argv[i] ? argv[i] : "NULL" ;
            }
             else if(std::string(azColName[i]) == "field_font")
            {
                gt.field_font = argv[i] ? argv[i] : "NULL" ;
            }
             else if(std::string(azColName[i]) == "field_font_size")
            {
                gt.field_font_size = argv[i] ? argv[i] : "NULL" ;
            }
             else if(std::string(azColName[i]) == "number_of_characters")
            {
                gt.number_of_characters = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "align")
            {
                gt.align = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "field_type")
            {
                gt.field_type = argv[i] ? argv[i] : "NULL" ;
            }
            else{ /*Do nothing*/ }

        }
        //Now set Value to an array..
        obj->form_fieldsValueStore.setValue(gt);
        return 0;
    }


//Select method of global table.. return type of TableValueStore write whereString without using where
TableValueStore  Form_FieldsTableOperations::Select(  std::string whereString = "NULL" )
{
        if (whereString != "NULL")
        {
            std::ostringstream oss;
            oss<< "SELECT * FROM " << Form_Fields::tableName << " WHERE " << whereString;
            this->sql_statement = oss.str();
        }
        else{ this->sql_statement = "SELECT * FROM " + Form_Fields::tableName ; }


        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor, this->sql_statement.c_str(), callback, (void*) this, &zErrMsg);
         if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued SELECT FORM_FIELD_TABLE statement\n";

        }
        else
        {
            std::cout<< "\nError: FORM_FIELD_TABLE\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);

        std::cout<< this->sql_statement;
        return form_fieldsValueStore;

}


//Insert method of global table..
//gt id name in string is compulsary..
int Form_FieldsTableOperations::Insert( Form_Fields gt )
{
        std::ostringstream oss;
        oss << "INSERT INTO " \
         << Form_Fields::tableName \
         << " ( id " \
         << ( gt.form_code != "NULL" ? ", form_code " : "" ) \
         << ( gt.field_name != "NULL" ? ", field_name " : "" ) \
         << ( gt.pdf_page_number != "NULL" ? ", pdf_page_number " : "" ) \
         << ( gt.field_X_position_in_page != "NULL" ? ", field_X_position_in_page " : "" ) \
         << ( gt.field_Y_position_in_page != "NULL" ? ", field_Y_position_in_page " : "" ) \
         << ( gt.field_width != "NULL" ? ", field_width " : "" ) \
         << ( gt.field_height != "NULL" ? ", field_height " : "" ) \
         << ( gt.field_font != "NULL" ? ", field_font " : "" ) \
         << ( gt.field_font_size != "NULL" ? ", field_font_size " : "" ) \
         << ( gt.number_of_characters != "NULL" ? ", number_of_characters " : "" ) \
         << ( gt.align != "NULL" ? ", align " : "" ) \
         << ( gt.field_type != "NULL" ? ", field_type " : "" ) \
         <<  " ) VALUES (  "  \
         << gt.id   \
         << ( gt.form_code != "NULL" ? ", " + gt.form_code  : "" ) \
         << ( gt.field_name != "NULL" ? ", '" + gt.field_name + "' " : "" ) \
         << ( gt.pdf_page_number != "NULL" ? ", '" + gt.pdf_page_number + "' " : "" ) \
         << ( gt.field_X_position_in_page != "NULL" ? " , '" + gt.field_X_position_in_page + "' "  : "" ) \
         << ( gt.field_Y_position_in_page != "NULL" ? ", '" + gt.field_Y_position_in_page + "' " : "" ) \
         << ( gt.field_width != "NULL" ? ", " + gt.field_width : "" ) \
         << ( gt.field_height != "NULL" ? ", " + gt.field_height  : "" ) \
         << ( gt.field_font != "NULL" ? ", '" + gt.field_font + "' " : "" ) \
         << ( gt.field_font_size != "NULL" ? ", " + gt.field_font_size : "" ) \
         << ( gt.number_of_characters != "NULL" ? ", " + gt.number_of_characters  : "" ) \
         << ( gt.align != "NULL" ? ", '" + gt.align + "' " : "" ) \
         << ( gt.field_type != "NULL" ? ", '" + gt.field_type + "' " : "" ) \
         << ");" ;
        this->sql_statement = oss.str();

        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor,this->sql_statement.c_str(), this->local_callback, 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued Form_fields_TABLE statement\n";

        }
        else
        {
            std::cout<< "\nError: inserting to forms_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);
        return rc;
}


   //Select method of global table.. return type of TableValueStore write whereString without using where
int Form_FieldsTableOperations::Delete(  std::string whereString  )
{
        this->sql_statement = "DELETE FROM " + Form_Fields::tableName +" WHERE  " + whereString ;

        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor, this->sql_statement.c_str(), local_callback , 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued FORM_FIELDS_TABLE delete statement\n";

        }
        else
        {
            std::cout<< "\nError: form_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);
        return rc;
}


int Form_FieldsTableOperations::Update( Form_Fields gt, std::string whereString )
{
        int initial_counter = 0;
        std::string set_stat = "";
        std::string stat;
        std::string update_stat = "UPDATE "  + Form_Fields::tableName + " set ";
        if(gt.id != "NULL")
        {
            /* Create merged SQL statement */


            if (initial_counter == 0)
            {
                   set_stat = set_stat + std::string("id = ") + gt.id  ;
                   initial_counter = 1;
            }
            else
            {
                   set_stat = set_stat + ", id = " + gt.id  ;
            }
        }
        if(gt.form_code != "NULL")
        {

            if(initial_counter == 0)
            {
                /* Create merged SQL statement */
                set_stat = set_stat + " form_code = " + gt.form_code ;
                initial_counter = 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat = set_stat + ", form_code = " + gt.form_code;
            }


        }
        if(gt.field_name != "NULL")
        {
            if(initial_counter == 0)
            {
                /* Create merged SQL statement */
                set_stat = set_stat + " field_name =  '" + gt.field_name + "'";
                initial_counter = 1;
            }
            else
            {
                /* Create merged SQL statement */
                 set_stat = set_stat + ", field_name =  '" + gt.field_name + "'";
            }


        }
        if( gt.pdf_page_number != "NULL" )
        {
            //std::stringstream sstm;
            if(initial_counter == 0)
            {

                /* Create merged SQL statement */
                set_stat = set_stat + " pdf_page_number =   '" +   gt.pdf_page_number + "'" ;
                initial_counter = 1;

            }
            else
            {
                               /* Create merged SQL statement */
                set_stat =  set_stat + ", pdf_page_number =   '" +   gt.pdf_page_number + "'";
            }


        }

        if( gt.field_X_position_in_page != "NULL" )
        {

            if(initial_counter == 0)
            {
                set_stat = set_stat + " field_X_position_in_page =  '" + gt.field_X_position_in_page + "'";
                initial_counter == 1;

            }
            else
            {
                /* Create merged SQL statement */
                set_stat = set_stat + ", field_X_position_in_page =  '" + gt.field_X_position_in_page + "'";

            }



        }
          if( gt.field_Y_position_in_page != "NULL" )
        {

            if(initial_counter == 0)
            {
                set_stat = set_stat + " field_Y_position_in_page =  '" + gt.field_Y_position_in_page + "'";
                initial_counter == 1;

            }
            else
            {
                /* Create merged SQL statement */
                set_stat = set_stat + ", field_Y_position_in_page =  '" + gt.field_Y_position_in_page + "'";

            }



        }

        if( gt.field_width != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " field_width = " + gt.field_width ;
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", field_width = " + gt.field_width ;
            }


        }
         if( gt.field_height != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " field_height = " + gt.field_height;
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", field_height = " + gt.field_height ;
            }


        }

         if( gt.field_font != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " field_font = '" + gt.field_font + "'";
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", field_font = '" + gt.field_font + "'";
            }


        }

          if( gt.field_font_size != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " field_font_size = " + gt.field_font_size;
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", field_font_size = " + gt.field_font_size ;
            }


        }
        if( gt.number_of_characters != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " number_of_characters = " + gt.number_of_characters ;
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", number_of_characters = " + gt.number_of_characters ;
            }


        }
        if( gt.align != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " align = '" + gt.align + "'";
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", align = '" + gt.align + "'";
            }


        }
         if( gt.field_type != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " field_type = '" + gt.field_type + "'";
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", field_type = '" + gt.field_type + "'";
            }


        }

        this->sql_statement = "";
        this->sql_statement = this->sql_statement + update_stat + set_stat + " WHERE " + whereString;
        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor,this->sql_statement.c_str(), local_callback , 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued FORM_FIELD_TABLE UPDATE statement\n";

        }
        else
        {
            std::cout<< "\nError: FORM_FIELD_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);

        return rc;

}





//-----------------------------------------------------AREA ENDS FOR FORMS_FIELD---------------------------------------------------------------




//----------------------------------------------------AREA FOR FORMS START HERE----------------------------------------------------------------

//FormsTableOperations class
 int FormsTableOperations::callback(void *data, int argc, char **argv, char **azColName)
    {
        //Converting the object back to this object form
        FormsTableOperations* obj = (FormsTableOperations*) data;
        //Callback function for storing each column of a row..
        int i;
        //Creating instance of globalTable class
        Forms gt;
        for(i=0; i<argc; i++)
        {
            if(  std::string(azColName[i]) == "id")
            {
                gt.id = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "form_code")
            {
                gt.form_code = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "form_dir")
            {
                gt.form_dir = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "form_desc")
            {
                gt.form_desc = argv[i] ? argv[i] : "NULL" ;
            }
            else if(std::string(azColName[i]) == "form_orig")
            {
                gt.form_orig = argv[i] ? argv[i] : "NULL" ;
            }
             else if(std::string(azColName[i]) == "form_URL")
            {
                gt.form_URL = argv[i] ? argv[i] : "NULL" ;
            }
            else{ /*Do nothing*/ }

        }
        //Now set Value to an array..
        obj->formsValueStore.setValue(gt);
        return 0;
    }


//Select method of global table.. return type of TableValueStore write whereString without using where
TableValueStore  FormsTableOperations::Select(  std::string whereString = "NULL" )
{
        if (whereString != "NULL")
        {
            std::ostringstream oss;
            oss<< "SELECT * FROM " << Forms::tableName << " WHERE " << whereString;
            this->sql_statement = oss.str();
        }
        else{ this->sql_statement = "SELECT * FROM " + Forms::tableName ; }


        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor, this->sql_statement.c_str(), callback, (void*) this, &zErrMsg);
         if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued SELECT FORMS_TABLE statement\n";

        }
        else
        {
            std::cout<< "\nError: FORMS SELECT\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);

        std::cout<< this->sql_statement;
        return formsValueStore;

}


//Insert method of global table..
//gt id name in string is compulsary..
int FormsTableOperations::Insert( Forms gt )
{
        std::ostringstream oss;
        oss << "INSERT INTO " \
         << Forms::tableName \
         << " ( id " \
         << ( gt.form_code != "NULL" ? ", form_code " : "" ) \
         << ( gt.form_dir != "NULL" ? ", form_dir " : "" ) \
         << ( gt.form_desc != "NULL" ? ", form_desc " : "" ) \
         << ( gt.form_orig != "NULL" ? ", form_orig " : "" ) \
         << ( gt.form_URL != "NULL" ? ", form_URL " : "" ) \
         <<  " ) VALUES (  "  \
         << gt.id   \
         << ( gt.form_code != "NULL" ? ", " + gt.form_code + " " : "" ) \
         << ( gt.form_dir != "NULL" ? ", '" + gt.form_dir + "' " : "" ) \
         << ( gt.form_desc != "NULL" ? ", '" + gt.form_desc + "' " : "" ) \
         << ( gt.form_orig != "NULL" ? " , '" + gt.form_orig + "' "  : "" )
         << ( gt.form_URL != "NULL" ? " , '" + gt.form_URL + "' "  : "" ) \
         << ");" ;
        this->sql_statement = oss.str();

        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor,this->sql_statement.c_str(), this->local_callback, 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued INSERT FORMS_TABLE statement\n";

        }
        else
        {
            std::cout<< "\nError: inserting to FORMS_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);
        return rc;
}


   //Select method of global table.. return type of TableValueStore write whereString without using where
int FormsTableOperations::Delete(  std::string whereString  )
{
        this->sql_statement = "DELETE FROM " + Forms::tableName +" WHERE  " + whereString ;

        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor, this->sql_statement.c_str(), local_callback , 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued Forms delete statement\n";

        }
        else
        {
            std::cout<< "\nError: Forms\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);
        return rc;
}


int FormsTableOperations::Update( Forms gt, std::string whereString )
{
        int initial_counter = 0;
        std::string set_stat = "";
        std::string stat;
        std::string update_stat = "UPDATE "  + Forms::tableName + " set ";
        if(gt.id != "NULL")
        {
            /* Create merged SQL statement */


            if (initial_counter == 0)
            {
                   set_stat = set_stat + std::string("id = ") + gt.id  ;
                   initial_counter = 1;
            }
            else
            {
                   set_stat = set_stat + ", id = " + gt.id  ;
            }
        }
        if(gt.form_code != "NULL")
        {

            if(initial_counter == 0)
            {
                /* Create merged SQL statement */
                set_stat = set_stat + " form_code =  " + gt.form_code;
                initial_counter = 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat = set_stat + ", form_code = " + gt.form_code ;
            }


        }
        if(gt.form_dir != "NULL")
        {
            if(initial_counter == 0)
            {
                /* Create merged SQL statement */
                set_stat = set_stat + " form_dir =  '" + gt.form_dir + "'";
                initial_counter = 1;
            }
            else
            {
                /* Create merged SQL statement */
                 set_stat = set_stat + ", form_dir =  '" + gt.form_dir + "'";
            }


        }
        if( gt.form_desc != "NULL" )
        {
            if(initial_counter == 0)
            {

                /* Create merged SQL statement */
                set_stat = set_stat + " form_desc =   '" +   gt.form_desc + "'";
                initial_counter = 1;

            }
            else
            {
                               /* Create merged SQL statement */
                set_stat =  set_stat + ", form_desc =   '" +   gt.form_desc + "'";
            }


        }

        if( gt.form_orig != "NULL" )
        {

            if(initial_counter == 0)
            {
                set_stat = set_stat + " form_orig =  '" + gt.form_orig + "'";
                initial_counter == 1;

            }
            else
            {
                /* Create merged SQL statement */
                set_stat = set_stat + ", form_orig =  '" + gt.form_orig + "'";

            }



        }

        if( gt.form_URL != "NULL" )
        {
            if(initial_counter == 0)
            {
                    /* Create merged SQL statement */
                    set_stat = set_stat + " form_URL = '" + gt.form_URL + "'";
                    initial_counter == 1;
            }
            else
            {
                /* Create merged SQL statement */
                set_stat =  set_stat +  ", form_URL = '" + gt.form_URL + "'";
            }

        }

        this->sql_statement = "";
        this->sql_statement = this->sql_statement + update_stat + set_stat + " WHERE " + whereString;
        int rc;
        char *zErrMsg = 0;
        /* Execute SQL statement */
        rc = sqlite3_exec(dbCursor,this->sql_statement.c_str(), local_callback , 0 , &zErrMsg);
          if( rc == SQLITE_OK )
        {
            std::cout<< "Succesfully exectued FORMS_TABLE UPDATE statement\n";

        }
        else
        {
            std::cout<< "\nError: FORMS_field\n"<<zErrMsg;
        }
        sqlite3_free(zErrMsg);

        return rc;

}





//---------------------------------------------------AREA FOR FORMS ENDS HERE-------------------------------------------------






int main(int argc, char* argv[])
{
    MyDatabase mydatabase;
    sqlite3 *db = mydatabase.createDatabase("forms.db");
    mydatabase.createTables(db);
    //---------------------------------------------AREA    FOR   GLOBAL TABLE ---------------------
    GlobalTable gt;
    TableValueStore tv;
    GlobalTableOperations gto(db);
    gt.field_code="545";
    gt.field_demo = "This is a demo string";
    gt.id = "NULL";
    gt.field_desc = "Description..";
    gt.field_size = "46";
    gt.field_type = "hfmk jdfncj";
    std::cout << gto.Insert( gt ) == SQLITE_OK;
    gto.Delete("id = 1");
    gt.id="250";
    gto.Update(gt,"id=8");
    tv = gto.Select("id=250");
    int l = tv.length;
    std::cout<<" \n"<< l;

//----------------------------------------------AREA ENDS FOR GLOBAL TABLE-------------------------

     mydatabase.closeDbConnection(db);
     return 0;
}

