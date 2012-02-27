#!/usr/bin/perl    

# File Name: newClient.cgi

#use strict;

#use warnings;

use DBI;
use CGI;

#send the obligatory Content-Type

#print "Content-Type: text/html\n\n"; 
  
$user = 'root';
$password = 'Devesh#1';

# Determining whether or not the <FORM> method is GET or POST
# if it is GET, that means all the data was appended to the URL
# if it is POST, all the data was put into STDIN, a data buffer

if ($ENV{"REQUEST_METHOD"} eq 'GET') { $buffer = $ENV{'QUERY_STRING'}; }
else { read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'}); }

# All the data has now been copied into the variable called $buffer
# The purpose of this is to associate each set of name of form field
# with the data entries in the form field
@pairs = split(/&/, $buffer);

foreach $pair (@pairs) {
      ($name, $value) = split(/=/, $pair);

      $value =~ tr/+/ /;
      $value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;

      $FORM{$name} = $value;
} 

#################
# An associative array (hash) called %FORM was created to
# store the value pairs. Now you can get the data from a
# form field simply by calling it by name. If you had a form
# that was like <INPUT Type="text" Name"YourName">
# this piece of data is stored in $FORM{'YourName'}.
# This is derived from the NAME and VALUE (pair) attributes in
# form input fields.
#################
# Print out all the form data on a results page
# See the form and associate its values to this page.


#print $FORM{'clientid'};
#print $FORM{'clientname'};
#print $FORM{'address'};
#print $FORM{'city'};
#print $FORM{'mobileno'};
  
$db_handle = DBI->connect ('DBI:mysql:database=accounting',$user,$password) 
	or die "Couldn't connect to database: $DBI::errstr\n";
	
	
#set the value of SQL query
#$sql_query = "select * from clientList";


#set the value of SQL query
$sql_query = "insert into clientList values ('$FORM{'clientid'}','$FORM{'clientname'}','$FORM{'address'}','$FORM{'city'}','$FORM{'mobileno'}')";

#prepare your statement for connecting to the databse
$statement = $db_handle->prepare ($sql_query)
	or die "Couldn't prepare query '$sql_query': $DBI::errstr\n";  
  
#execute youe sql statement
$statement->execute()
	or die "SQL Error: $DBI::errstr\n";  

#system ("/usr/bin/perl http://localhost/cgi-bin/newClientForm.cgi");

#print CGI->redirect ("http://localhost/cgi-bin/newClientForm.cgi");



print "Location: http://localhost/cgi-bin/addNewClientForm.cgi\n\n";





