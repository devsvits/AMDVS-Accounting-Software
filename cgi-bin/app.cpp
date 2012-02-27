#include <iostream>
#include <malloc.h>
#include "CGI.h"

using namespace std;

int main()
{
  CCGI cgi;
  CCGIItem item;
  char* lpszOut;
  char* lpszContentLength = getenv("CONTENT_LENGTH");
// int nContentLength = atoi (lpszContentLength+1);  
int i,nLength;

  cout << "Content-type: text/html" << endl << endl
   << "<html>" << endl
   << "<body>" << endl
   << "<p>You sent " << lpszContentLength

   << " bytes of data, which contained the following values: </p>" << endl
   << "<ul>" << endl;
  cgi.Load();

  for(i=0; i<cgi.GetItemCount(); i++)
  {

	cgi.GetItem(i,&item);

    nLength = item.GetNameLength();
    lpszOut = (char*) malloc(nLength);
    item.GetName(lpszOut,nLength);
    cout << "<li>" << lpszOut << " - ";

    free(lpszOut);

    nLength = item.GetLength();
    lpszOut = (char*) malloc(nLength);
    item.GetValue(lpszOut,nLength);
    cout << lpszOut << endl;

    free(lpszOut);
  }

  cout << "</ul>" << endl
   << "</body>" << endl
   << "</html>" << endl;
}
