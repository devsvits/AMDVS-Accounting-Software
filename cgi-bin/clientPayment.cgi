#!/usr/bin/perl    

use HTML::Template;
use DBI;
  
#send the obligatory Content-Type
print "Content-Type: text/html\n\n"; 
  
$user = 'root';
$password = 'Devesh#1';
  
$db_handle = DBI->connect ('DBI:mysql:database=accounting',$user,$password) 
	or die "Couldn't connect to database: $DBI::errstr\n";
  		
#set the value of SQL query
$sql_query = "select count(clientCode) from clientList";

#prepare your statement for connecting to the databse
$statement = $db_handle->prepare ($sql_query)
	or die "Couldn't prepare query '$sql_query': $DBI::errstr\n";  
  
#execute youe sql statement
$statement->execute()
	or die "SQL Error: $DBI::errstr\n";  
  	 	
# open the HTML template
my $template = HTML::Template->new(filename => 'clientPayment.tmpl');
  
my @loop_data = ();

my $count = $statement->fetchrow_array();
#print $count; 
$count = $count+2;

  	
# print the template
print $template->output;
$dbh->disconnect; 
