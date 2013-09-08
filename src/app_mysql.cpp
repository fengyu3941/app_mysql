#include <iostream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define OTL_ODBC // Compile OTL 4.0/ODBC
// The following #define is required with MyODBC 3.51.11 and higher
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE
// #define OTL_ODBC_UNIX // uncomment this line if UnixODBC is used
#include "otlv4.h" // include the OTL 4.0 header file

#include "app_mysql.h"

using namespace std;

static otl_connect db; // connect object
static int dbconnected = 0;

static int get_artist_id(char *artist);


int database_start(char *name, char *pwd) {

    if (dbconnected) return 1;

    otl_connect::otl_initialize(); // initialize ODBC environment

    char loginCmd[1024];
    sprintf(loginCmd, "UID=%s;PWD=%s;DSN=test2", name, pwd);

    try{

        db.rlogon(loginCmd); // connect to ODBC
        //  db.rlogon("scott/tiger@mysql35"); // connect to ODBC, alternative format
        // of connect string 
    }
    catch(otl_exception& p){ // intercept OTL exceptions
        cerr<<"login failed"<<endl;
        cerr<<p.msg<<endl; // print out error message
        cerr<<p.stm_text<<endl; // print out SQL that caused the error
        cerr<<p.sqlstate<<endl; // print out SQLSTATE message
        cerr<<p.var_info<<endl; // print out the variable that caused the error
    }

    dbconnected = 1;
    return 1;
} /* database start */

void database_end() {
    if (dbconnected) db.logoff(); // disconnect from ODBC
    dbconnected = 0;
} /* database_end */

int add_cd( char *artist, char *title, char *catalogue, int *cd_id )
{
    //no connection to database
    if (!dbconnected) return 0;

     // insert into cd 
    {
        try
        {
            int artist_id = -1;
            artist_id = get_artist_id(artist);

            otl_stream o(1, // buffer size should be == 1 always on INSERT
                "insert into cd values "
                " (:myid<int>, :mytitle<char[70]>,:myartist_id<int>,:mycatalogue<char[30]>) ",
                // INSERT statement. Multiple sets of values can be used
                // to work around the lack of the bulk interface
                db // connect object
                );
            o<<otl_null()<<title<<artist_id<<catalogue;
            cout<<"insert into cd successed"<<endl;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"insert into cd failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
        
    }

    //get the new_cd_id
    {
        try
        {
            int new_cd_id = -1;
           
            
            otl_stream i(50, // buffer size should be == 1 always on INSERT
                "SELECT LAST_INSERT_ID()",
                // INSERT statement. Multiple sets of values can be used
                // to work around the lack of the bulk interface
                db // connect object
                );
            while(!i.eof())
                i>>new_cd_id;
            if(-1 == new_cd_id)
            {
                 cerr<<"get the new_cd_id failed"<<endl;
                 return 0;
            }
            else 
            {
                *cd_id = new_cd_id;
                cout<<"get the new_cd_id successed"<<endl;
                return 1;
            }

        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"get the new_cd_id failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
    }
    

    return 1;
}
/* Find or create an artist_id for the given string */
static int get_artist_id(char *artist) {
    //no connection to database
    if (!dbconnected) return -1;

    //if already exsisted, return the artist_id
    {
        try
        {
            int artist_id = -1;
            otl_stream i(50, // buffer size should be == 1 always on INSERT
                "SELECT id FROM artist "
                "WHERE name = :myname<char[100]>",
                // INSERT statement. Multiple sets of values can be used
                // to work around the lack of the bulk interface
                db // connect object
                );
            i<<artist;
            while(!i.eof())
                i>>artist_id;
            if(-1 != artist_id)        
            {
                cout<<"get the artist_id successed"<<endl;
                return artist_id;
            }
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"get the artist_id failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return -1;
        }
        
        
    }

    //insert name into table artist
    {
        try
        {
            otl_stream o(1, // buffer size should be == 1 always on INSERT
                "insert into artist values "
                " (:myid<int>,:myname<char[100]>)",
                // INSERT statement. Multiple sets of values can be used
                // to work around the lack of the bulk interface
                db // connect object
                );
            o<<otl_null()<<artist;
            cout<<"insert into artist successed"<<endl;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"insert into artist failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return -1;
        }
    }

    //get the new_artist_id
    {
        try
        {
            int new_artist_id = -1;
            

            otl_stream i(50, // buffer size should be == 1 always on INSERT
                "SELECT LAST_INSERT_ID()",
                // INSERT statement. Multiple sets of values can be used
                // to work around the lack of the bulk interface
                db // connect object
                );
            while(!i.eof())
                i>>new_artist_id;
            if(-1 == new_artist_id)
            {
                cerr<<"get the new_artist_id failed"<<endl;
                return -1;
            }
            else 
            {             
                cout<<"get the new_artist_id successed"<<endl;
                return new_artist_id;
            }

        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"get the new_artist_id failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return -1;
        }
    }
}
int add_tracks( struct current_tracks_st *tracks )
{
    //no connection to database
    if (!dbconnected) return 0;

    int  rowIndex = 0;
    while(tracks->track[rowIndex][0])
    //insert name into table artist
    {
        try
        {
            otl_stream o(1, // buffer size should be == 1 always on INSERT
                "insert into track values "
                " (:mycd_id<int>, :mytrack_id<int>, :mytitle<char[100]>) ",
                // INSERT statement. Multiple sets of values can be used
                // to work around the lack of the bulk interface
                db // connect object
                );
            o<<tracks->cd_id<<rowIndex<<tracks->track[rowIndex];
          
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"insert into track failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
        rowIndex++;
    }

      cout<<"insert into track successed"<<endl;
      return 1;
}


int get_cd( int cd_id, struct current_cd_st *dest )
{
    //no connection to database
    if (!dbconnected) return 0;

    //init dest
    memset(dest, 0, sizeof(*dest));
    dest->artist_id = -1;

    //get_cd
    {
        try
        {
            otl_stream i(50, // buffer size may be > 1
                "SELECT artist.id, cd.id, artist.name, cd.title, cd.catalogue FROM artist, cd "
                "WHERE artist.id = cd.artist_id and cd.id = :myid<int>",
                // SELECT statement
                db // connect object
                ); 
            // create select stream
        
            i<<cd_id;

            while(!i.eof())
            { // while not end-of-data
                i>>dest->artist_id>>dest->cd_id>>dest->artist_name>>dest->title>>dest->catalogue;
            }

             cout<<"get_cd  successed"<<endl;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"get_cd failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
    }
    
      if (dest->artist_id != -1) return 1;
      return 0;
}

int get_cd_tracks( int cd_id, struct current_tracks_st *dest )
{
    //no connection to database
    if (!dbconnected) return 0;

    //init dest
    memset(dest, 0, sizeof(*dest));
    dest->cd_id = -1;

    //get_cd_tracks 
    {
        try
        {
            otl_stream i(50, // buffer size may be > 1
                "SELECT track_id, title FROM track "
                "WHERE track.cd_id = :mycd_id1<int> ORDER BY track_id",
                // SELECT statement
                db // connect object
                ); 
            // create select stream

            i<<cd_id;
            
            int rowIndex = 0 ;
            int temp = 0;
            while(!i.eof())
            { // while not end-of-data
                i>>temp>>dest->track[rowIndex];
                rowIndex++;
            }
            dest->cd_id = cd_id;
            cout<<"get_cd_tracks  successed"<<endl;
            return rowIndex;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"get_cd_tracks failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
    }
}

int find_cds(char *search_str, struct cd_search_st *dest)
{
    //no connection to database
    if (!dbconnected) return 0;
    //init
    memset(dest, -1, sizeof(*dest));

    //find_cds 
    {
        try
        {
            otl_stream i(50, // buffer size may be > 1
                "SELECT DISTINCT artist.id, cd.id FROM artist, cd "
                "WHERE artist.id = cd.artist_id and "
                "(artist.name LIKE :myname<char[250]> OR cd.title LIKE :mytitle<char[250]> OR cd.catalogue LIKE :mycatalogue<char[250]>)",
                // SELECT statement
                db // connect object
                ); 
            // create select stream

             char ss[250];
             sprintf(ss, "%%%s%%", search_str);
             i<<ss<<ss<<ss;


            int rowIndex = 0 ;
            int temp = 0;
            while(!i.eof())
            { // while not end-of-data
                i>>temp>>dest->cd_id[rowIndex];
                rowIndex++;
            }
            cout<<"find_cds  successed"<<endl;
            return rowIndex;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"find_cds failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
    }
}

int delete_cd( int cd_id )
{
    //no connection to database
    if (!dbconnected) return 0;

    int artist_id;
    artist_id = -1;

    //find artist_id
    {
        try
        {
            otl_stream i(50, // buffer size may be > 1
                "SELECT artist_id FROM cd "
                "WHERE artist_id = (SELECT artist_id FROM cd WHERE id = :myid<int>)",
                // SELECT statement
                db // connect object
                ); 
            // create select stream

          
            i<<cd_id;


           int rowIndex = 0;
            while(!i.eof())
            { 
                i>>artist_id;
                rowIndex++;
            }

            // Artist used by any other CDs, do not delete it 
            if(1!=rowIndex)
                artist_id = -1;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"find artist_id failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
    }

    
    //delete from track
    {
        try
        {
            otl_stream o(1, // buffer size should be == 1 always on UPDATE
                "DELETE FROM track "
                "WHERE cd_id = :mycd_id<int>", 
                // UPDATE statement
                db // connect object
                );
            o<<cd_id;
            cout<<"delete from track successed"<<endl;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"delete from track failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
    }
    
    //delete from cd
    {
        try
        {
            otl_stream o(1, // buffer size should be == 1 always on UPDATE
                "DELETE FROM cd "
                "WHERE id = :myid<int>", 
                // UPDATE statement
                db // connect object
                );
            o<<cd_id;
            cout<<"delete from cd successed"<<endl;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"delete from cd failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }
    }
    //delete from artist
    if (artist_id != -1) {
        try
        {
            otl_stream o(1, // buffer size should be == 1 always on UPDATE
                "DELETE FROM artist "
                "WHERE id = :myid<int>", 
                // UPDATE statement
                db // connect object
                );
            o<<artist_id;
            cout<<"delete from artist successed"<<endl;
        }
        catch(otl_exception& p)
        { // intercept OTL exceptions
            cerr<<"delete from artist failed"<<endl;
            cerr<<p.msg<<endl; // print out error message
            cerr<<p.stm_text<<endl; // print out SQL that caused the error
            cerr<<p.sqlstate<<endl; // print out SQLSTATE message
            cerr<<p.var_info<<endl; // print out the variable that caused the error
            return 0;
        }

    }
     return 1;
}


