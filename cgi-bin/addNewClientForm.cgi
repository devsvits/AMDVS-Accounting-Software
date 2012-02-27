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
$sql_query = "select clientCode from clientList order by clientCode DESC LIMIT 1";

#prepare your statement for connecting to the databse
$statement = $db_handle->prepare ($sql_query)
	or die "Couldn't prepare query '$sql_query': $DBI::errstr\n";  
  
#execute youe sql statement
$statement->execute()
	or die "SQL Error: $DBI::errstr\n";  
  	 	
# open the HTML template
my $template = HTML::Template->new(filename => 'addNewClientForm.tmpl');
  
my @loop_data = ();

my $x =   $statement->fetchrow_array();

$x = reverse $x;  #Reverse the String
chop $x; # Chop the last Character (here C)
$x = reverse $x; # Again Reverse
$x = $x  + 1; # Add 1

#print length($x); 

if ( length($x) eq 1) # if length ==2
{
	#print 'C000'.$x;
	$template->param (preString => C000);
}  	 
elsif ( length($x) eq 2)
{
	#print 'C00'.$x;
	$template->param (preString => C00);
} 
elsif ( length($x) eq 3)
{
	#print 'C0'.$x;
	$template->param (preString => C0);
} 
elsif ( length($x) eq 4)
{
	#print 'C'.$x;
	$template->param (preString => C);
} 
#print length($x);
#print $x;

$template->param (clientid => $x);
  	
# print the template
print $template->output;
#$db_handle->disconnect; 
