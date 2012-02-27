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
$sql_query = "select * from clientList";

#prepare your statement for connecting to the databse
$statement = $db_handle->prepare ($sql_query)
	or die "Couldn't prepare query '$sql_query': $DBI::errstr\n";  
  
#execute youe sql statement
$statement->execute()
	or die "SQL Error: $DBI::errstr\n";  
  	 	
# open the HTML template
my $template = HTML::Template->new(filename => 'clientList.tmpl');
  
my @loop_data = ();
  	
while (@data = $statement->fetchrow_array())
{
	#print @data;		
	my %row_data; # get a fresh hash for row data
		  		
	$row_data {clientID} = $data[0];
	$row_data {clientName} = $data[1]; 	
	$row_data {address} = $data[2];
	$row_data {ccity} = $data[3]; 
	$row_data {mobileNumber} = $data[4];
	
	$row_data {URL}="http://localhost/cgi-bin/showClientBills.cgi?clientid=$data[0]";	
				
	push (@loop_data, \%row_data)	;
	#print $data[1];
}
  	 
$template->param (DB_LOOP => \@loop_data);
  	
# print the template
print $template->output;
$dbh->disconnect; 
