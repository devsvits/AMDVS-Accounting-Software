#!/usr/bin/perl    

use HTML::Template;
use DBI;
  
#send the obligatory Content-Type
print "Content-Type: text/html\n\n"; 


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

#print $FORM{'clientid'};



$user = 'root';
$password = 'Devesh#1';
  
$db_handle = DBI->connect ('DBI:mysql:database=accounting',$user,$password) 
	or die "Couldn't connect to database: $DBI::errstr\n";
  		
#set the value of SQL query
#$sql_query = "select * from clientBills where clientCode = '$FORM{'clientid'}'";

$sql_query = "SELECT t1.date, t1.billNumber,   t1.billAmount, t1.clientCode,  SUM(t2.amount) FROM clientBills t1 LEFT JOIN billRecievedFromClient t2 ON t1.billNumber = t2.forBillNumber  group by billNumber having t1.clientCode= '$FORM{'clientid'}'";


#prepare your statement for connecting to the databse
$statement = $db_handle->prepare ($sql_query)
	or die "Couldn't prepare query '$sql_query': $DBI::errstr\n";  
  
#execute youe sql statement
$statement->execute()
	or die "SQL Error: $DBI::errstr\n";  
  	 	
# open the HTML template
my $template = HTML::Template->new(filename => 'showClientBills.tmpl');
  
my @loop_data = ();
  	
while (@data = $statement->fetchrow_array())
{
	#print @data;		
	my %row_data; # get a fresh hash for row data
		  		
	$row_data {date} = $data[0];
	$row_data {billNumber} = $data[1]; 	
	$row_data {billAmount} = $data[2];

	if ($data[4] eq '')
	{
		$row_data {paidAmount} = '0.00';
	}	
	else 	
	{	
		$row_data {paidAmount} = $data[4]; 
	
	}
		
	if ($row_data {paidAmount} eq $row_data {billAmount})	
	{
		$row_data {billStatus} = "Fully Paid" ;
	}
	elsif ($row_data {paidAmount} eq '0.00')
	{
		$row_data {billStatus} = "Not Paid" ;
		$row_data {BOOL} = "1" ;		
		$row_data {URL2}="http://localhost/cgi-bin/clientPayment.cgi";
	}
	
	else
	{
		$row_data {billStatus} = "Partially Paid" ;
		$row_data {BOOL} = "1" ;			
		$row_data {URL2}="http://localhost/cgi-bin/clientPayment.cgi";
	}
	
	
	$row_data {URL}="http://localhost/cgi-bin/paymentHistoryClientBills.cgi?billnumber=$data[1]";	
		
	
		
	
	push (@loop_data, \%row_data)	;
	#print $data[1];
}
  	 
$template->param (DB_LOOP => \@loop_data);
  	
# print the template
print $template->output;
$dbh->disconnect; 
