// 2011 Virgoptrex
// The code is adapted from James Marshall's getcgivars.c
// tested with the following webservers
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
// Mongoose and LIGHTTPD
// Feel Free to use leave credits intact

// Compile: 
// g++ -g testparse.cpp -o testparse.cgi
// To run: 
// place cgi file in the appropriate directory depending on server configuration
// Start webserver and then open internet browser and (assuming script running on localserver, port 8080 under 'cgi_bin' directory) enter something similar to
// http://localhost:8080/cgi_bin/testparse.cgi?name1=value1&name2=value2&name3=value3 


#include <iostream>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <cstdio>
#include <map>
#include <cstring>
#include <cctype>
#include <ctype.h>

#include <malloc.h>
#include "CGI.h"
using namespace std;

 
// just going to input the general details and not the port numbers
struct connection_details
{
    const char *server;
    const char *user;
    const char *password;
    const char *database;
};


vector<string> split(const string& s, const string& delim, const bool keep_empty = true) 
{
    vector<string> result;
    if (delim.empty()) {
    result.push_back(s);
    return result;
    }
    string::const_iterator substart = s.begin(), subend;
    while (true) {
    subend = search(substart, s.end(), delim.begin(), delim.end());
    string temp(substart, subend);
    if (keep_empty || !temp.empty()) {
        result.push_back(temp);
    }
    if (subend == s.end()) {
        break;
    }
    substart = subend + delim.size();
    }
    return result;
}


std::map<std::string, std::string>& map_pairs(char* character, std::map<std::string, std::string>& Elements)
{
string test;
string key;
string value;  
vector<string>::iterator it;
string character_string = character;
vector<string> words;


 //Elements.insert(std::pair<string,string>("0","0"));
   //cout << "########## SPLITTING STRING INTO SMALLER CHUNKS ################## " << endl;
   words = split(character_string, "&");
   //cout << "########## GENERATING KEY VALUE PAIRS ################## " << endl;
   for (it=words.begin(); it!=words.end(); ++it)
   {    
    test = *it;
    cout<< "string:" << test << endl;

    const string::size_type pos_eq = test.find ('=');    
    if (pos_eq  != string::npos)
    {
     //Assign Key and Value Respectively
     key = test.substr (0, pos_eq);
     value = test.substr (pos_eq + 1);
     //cout << "key = " << key << " and value = " << value << endl;
     Elements.insert(std::pair<string,string>(key,value));   
    }

   }


return Elements;
}

std::map<std::string, std::string>& getcgivars(std::map<std::string, std::string>& Elements) 
{
    register int i ;
    char *request_method = NULL;
    int content_length;
    char *cgiinput = NULL;

    // Depending on the request method, read all CGI input into cgiinput 
    // (really should produce HTML error messages, instead of exit()ing) 
    request_method= getenv("REQUEST_METHOD") ;
   if (!strcmp(request_method, "GET") || !strcmp(request_method, "HEAD") ) {
    // Some servers apparently don't provide QUERY_STRING if it's empty, 
    //   so avoid strdup()'ing a NULL pointer here.                      
    char *qs ;
    qs= getenv("QUERY_STRING") ;
    cgiinput= strdup(qs  ? qs  : "") ;
    }
    else if (!strcmp(request_method, "POST")) {
    // strcasecmp() is not supported in Windows-- use strcmpi() instead 
    if ( strcasecmp(getenv("CONTENT_TYPE"), "application/x-www-form-urlencoded")) {
        printf("getcgivars(): Unsupported Content-Type.\n") ;
        exit(1) ;
    }
    if ( !(content_length = atoi(getenv("CONTENT_LENGTH"))) ) {
        printf("getcgivars(): No Content-Length was sent with the POST request.\n") ;
        exit(1) ;
    }
    if ( !(cgiinput = new char[content_length+1] )) {
        printf("getcgivars(): Could not \"new\" for cgiinput.\n") ;
        exit(1) ;
    }
    if (!fread(cgiinput, content_length, 1, stdin)) {
        printf("Couldn't read CGI input from STDIN.\n") ;
        exit(1) ;
    }
    cgiinput[content_length]='\0' ;
    }
    else {
    printf("getcgivars(): unsupported REQUEST_METHOD\n") ;
    exit(1) ;
    }

     map_pairs(cgiinput,Elements);

    return Elements;
}
 
MYSQL* mysql_connection_setup(struct connection_details mysql_details)
{
     // first of all create a mysql instance and initialize the variables within
    MYSQL *connection = mysql_init(NULL);
 
    // connect to the database with the details attached.
    if (!mysql_real_connect(connection,mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)) {
      printf("Conection error : %s\n", mysql_error(connection));
      exit(1);
    }
    return connection;
}
 
MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query)
{
   // send the query to the database
   if (mysql_query(connection, sql_query))
   {
      printf("MySQL query error : %s\n", mysql_error(connection));
      exit(1);
   }
 
   return mysql_use_result(connection);
}
 
int main ()
{ 

	MYSQL *conn;		// the connection
	MYSQL_RES *res;	// the results
	MYSQL_ROW row;	// the results row (line by line)
 
	struct connection_details mysqlD;
  	mysqlD.server = "localhost";  // where the mysql database is
  	mysqlD.user = "root";		// the root user of mysql	
  	mysqlD.password ="Devesh#1"; 
  	mysqlD.database = "accounting";	// the databse to pick
  

  	CCGI cgi;
  	CCGIItem item;
  	string values[5];
	char* lpszOut;
  	char* lpszContentLength = getenv("CONTENT_LENGTH");
	// int nContentLength = atoi (lpszContentLength+1);  
  	int nLength;

	cout 	<< "Content-type: text/plain" << endl << endl;
	cout	<< "you sent " << lpszContentLength
	<< " bytes of data, which contained the following values: " << endl;
  
	cgi.Load();
	
	
	
  	for(int i=0; i<cgi.GetItemCount(); i++)
  	{
		cgi.GetItem(i,&item);

  		//  nLength = item.GetNameLength();
   		// lpszOut = (char*) malloc(nLength);
   		// item.GetName(lpszOut,nLength);
    		//cout << "<li>" << lpszOut << " - ";

    		//free(lpszOut);

		nLength = item.GetLength();
		lpszOut = (char*) malloc(nLength);
		item.GetValue(lpszOut,nLength);
		cout << lpszOut << endl;
		values[i]= lpszOut;
    		
		/*int position = values[i].find ("+");
		if (position)
		{
			values[i].replace (position, 1, " ");
		}*/
		
		free(lpszOut);
 	}	

	for (int i=0; i<5; i++)
	{
		cout << values [i];
	}	


	//connect to mysql database
	conn = mysql_connection_setup (mysqlD);
 	string sqlString;


  	sqlString = "insert into stockInHand values ('"+ values[0] + "','"+values[1]+"','"+values[2]+"', '"+values[3]+"','"+values[4]+"')";

	cout << sqlString << endl;	

	// converting string into char*.. because second parameter of mysql_perform_query is char*.
	char* sqlQuery = &sqlString[0];

	// assign the results return to the MYSQL_RES pointer
 	res = mysql_perform_query(conn, sqlQuery);

	cout << endl << endl;  
 
	// assign the results return to the MYSQL_RES pointer
  	res = mysql_perform_query(conn, "select * from stockInHand");

	cout << endl << endl;  
	cout << "MySQL Tables in mysql Database:" << endl; 
  	while ((row = mysql_fetch_row(res)) !=NULL)
      	{
		cout << row[0] << endl;
		cout << row[1] << endl;
		cout << row[2] << endl;
		cout << row[3] << endl;
		cout << row[4] << endl<<endl;
     	}	
 
  	/* clean up the database result set */
  	mysql_free_result(res);
  	/* clean up the database link */
  	mysql_close(conn);
 
  	return 0;
}
