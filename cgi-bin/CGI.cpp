/**********************************************************
*CGI.cpp                                                  *
* Implementation for a rudimentary CGI input parser       *
* Last updated by Mike Lin 8/7/1998                       *
**********************************************************/

#include "CGI.h"
#include <iostream>
using namespace std;
/* CCGIItem constructor */

CCGIItem::CCGIItem()
{
	m_lpszValue = NULL;
	m_lpszName = NULL;
}

/* CCGIItem deconstructor */

CCGIItem::~CCGIItem()
{
	// free buffers if they are allocated

	if(m_lpszValue)
		free(m_lpszValue);

	if(m_lpszName)
		free(m_lpszName);
}

/************************************************
* CCGIItem::GetLength                           *
* Retrieves the length of the value             *
* Return value: the length of the value, 0 if   *
* there was no parseable value                  *
************************************************/

int CCGIItem::GetLength()
{
	return strlen(m_lpszValue);
}

/************************************************
* CCGIItem::GetNameLength                       *
* Retrieves the length of the name              *
* Return value: the length of the name, 0 if    *
* there was no parseable name                   *
************************************************/

int CCGIItem::GetNameLength()
{
	return strlen(m_lpszName);
}

/************************************************
* CCGIItem::GetValue                            *
* Retrieves the value                           *
* Parameters: lpszBuffer - pointer to buffer to *
*                          receive value        *
*             cbBufferLength - length in bytes  *
*                              of lpszBuffer    *
* Return value: 0 if success, -1 if failure     *
************************************************/

int CCGIItem::GetValue(char* lpszBuffer, int cbBufferLength)
{
	if(m_lpszValue == NULL)		// die if the value of this item has not been set
		return -1;

	if(cbBufferLength < GetLength())	// die of lpszBuffer is too small
		return -1;

	strcpy(lpszBuffer, m_lpszValue);	// copy value into lpszBuffer

	return 0;	// success
}

/************************************************
* CCGIItem::GetName                             *
* Retrieves the name of this item               *
* Parameters: lpszBuffer - buffer to be filled  *
*                          with item name       *
*             cbBufferLength - length, in bytes,*
*                              of lpszBuffer    *
* Return value: 0 if success, -1 if error       *
************************************************/

int CCGIItem::GetName(char* lpszBuffer, int cbBufferLength)
{
	if(m_lpszName == NULL)	// die if the name of this item has not been set
		return -1;

	if(cbBufferLength < GetNameLength())	// die if lpszBuffer is too small
		return -1;

	strcpy(lpszBuffer, m_lpszName);	// copy name into lpszBuffer

	return 0;	// success
}

/************************************************
* CCGIItem::SetValue                            *
* Sets this item's value                        *
* Parameters: lpszBuffer - pointer to string    *
*                          containing value     *
* Return value: 0 if success, -1 if error       *
************************************************/

int CCGIItem::SetValue(char* lpszBuffer)
{
	int cbBufferLength;

	if(m_lpszValue)			// if a value for this item has already been set,
		free(m_lpszValue);	// deallocate it

	cbBufferLength = strlen(lpszBuffer);	// get length of value

	if(cbBufferLength == 0)		// die if we couldn't get the length for whatever reason
		return -1;

	m_lpszValue = (char*) malloc(cbBufferLength + 1);	// allocate a buffer

	if(m_lpszValue == NULL)	// die if allocation failed
		return -1;

	strcpy(m_lpszValue, lpszBuffer);	// copy value into buffer

	return 0;	// success
}

/************************************************
* CCGIItem::SetName                             *
* Sets the name of this item                    *
* Parameters: lpszBuffer - pointer to string    *
*                          containing name      *
* Return value: 0 if success, -1 if failure     *
************************************************/

int CCGIItem::SetName(char* lpszBuffer)
{
	int cbBufferLength;

	if(m_lpszName)			// if a name has already been set, deallocate it
		free(m_lpszName);

	cbBufferLength = strlen(lpszBuffer);	// get length of name

	if(cbBufferLength == 0)	// die if we couldn't get the length for whatever reason
		return -1;

	m_lpszName = (char*) malloc(cbBufferLength + 1); // allocate a buffer

	if(m_lpszValue == NULL)	// die if allocation failed
		return -1;

	strcpy(m_lpszName, lpszBuffer);	// copy name into buffer

	return 0;
}




/* CCGI Constructor */

CCGI::CCGI()
{
	m_lpszRawData = NULL;
}

/* CCGI Deconstructor */

CCGI::~CCGI()
{
	if(m_lpszRawData)	// if a buffer was allocated for data, deallocate it
		free(m_lpszRawData);
}

/************************************************
* CCGI::Load                                    *
* Retrieves CGI input data from stdin           *
* Return value: 0 if success, -1 if error       *
************************************************/

int CCGI::Load()
{
	char* lpszContentLength;
	int nContentLength;

	lpszContentLength = getenv("CONTENT_LENGTH");	// retrieve CONTENT_LENGTH environment variable

	if(lpszContentLength == NULL)	// die if we couldn't get it
		return -1;					// this probably means the program is NOT being run under CGI

	nContentLength = atoi(lpszContentLength);	// atoi it

	if(nContentLength == 0)	// die if it's 0
		return -1;

	m_lpszRawData = (char*) malloc(nContentLength+1);	// allocate a buffer

	if(m_lpszRawData == NULL)	// die if allocation failed
		return -1;

	memset(m_lpszRawData, 0, nContentLength+1);	// zero out the buffer

	if(fread(m_lpszRawData, 1, nContentLength, stdin) == 0)	// get the data
		return -1;

	
//	cout 	<< "Content-type: text/plain" << endl << endl;
	cout << "loading time" << m_lpszRawData;	

	
		

	if(ferror(stdin))	// die if there was an fread error
		return -1;

	return 0;	// success
}

/************************************************
* CCGI::GetItemCount                            *
* Retrieve number of items                      *
* Return value: number of items, or -1 if error *
************************************************/

int CCGI::GetItemCount()
{
	if(m_lpszRawData == NULL)	// die if there is no data
		return -1;

	char* pbuf = m_lpszRawData;	// get a working pointer to data
	int nItemCount = 1;

	while(pbuf = strchr(pbuf,'&'))	// as long as we can find a '&'
	{
		pbuf++;						// put pbuf to start of next item
		nItemCount++;				// increment nItemCount
	}

	return nItemCount;	// success, return count
}

/************************************************
* CCGI::GetItem                                 *
* Retrieves an item                             *
* Parameters: nItemIndex - zero-based index of  *
*                          item to retrieve     *
*             pItem - pointer to CCGIItem object*
*                     that will be filled       *
* Return value: 0 if success, -1 if error       *
************************************************/

int CCGI::GetItem(int nItemIndex, CCGIItem* pItem)
{
	// working pointers
	char *lpszItem,*lpszItemEnd,*lpszItemBuf,*lpszNameBuf,*lpszValueBuf;
	int i,nItemLength,nNameLength;

	lpszItem = m_lpszRawData;	// get working pointer to data

	// move to the requested item

	for(i=0; i<nItemIndex; i++)
	{
		lpszItem = strchr(lpszItem,'&');	// find '&'

		if(lpszItem == NULL)				// if we couldn't find it, then the index is invalid
			return -1;

		lpszItem++;		// move pointer to start of next item
	}

	// find the end of the item

	lpszItemEnd = strchr(lpszItem,'&');

	if(lpszItemEnd == 0)					// if there's no end, this is the last item...
		nItemLength = strlen(lpszItem);		// set it to the length of the remaining string
	else									// otherwise, get length via pointer subtraction
		nItemLength = lpszItemEnd - lpszItem;

	lpszItemBuf = (char*) malloc(nItemLength+1);	// allocate a buffer for the item

	if(lpszItemBuf == NULL)	// die if allocation failed
		return -1;

	memset(lpszItemBuf, 0, nItemLength+1);		// zero out the buffer

	memcpy(lpszItemBuf, lpszItem, nItemLength);	// copy item into buffer

	lpszValueBuf = strchr(lpszItemBuf,'=');		// now, find the = separating name and value

	if(lpszValueBuf == NULL)	// if we couldn't find the =, this is not valid CGI input; die
	{
		free(lpszItemBuf);
		return -1;
	}

	nNameLength = lpszValueBuf - lpszItemBuf;	// get the length of the name

	lpszValueBuf++;	// increment the value pointer past the =

	lpszNameBuf = (char*) malloc(nNameLength + 1);	// allocate a buffer for the name

	if(lpszNameBuf == NULL)	// die if allocation failed
	{
		free(lpszItemBuf);
		return -1;
	}

	memset(lpszNameBuf, 0, nNameLength + 1);	// zero out the buffer

	memcpy(lpszNameBuf, lpszItemBuf, nNameLength);	// copy name into buffer

	// set the value and name of the CCGIItem passed to us

	if((pItem->SetValue(lpszValueBuf) == -1) || (pItem->SetName(lpszNameBuf) == -1))
	{
		// free buffers and die if this failed

		free(lpszItemBuf);
		free(lpszNameBuf);
		return -1;
	}

	// free buffers

	free(lpszNameBuf);
	free(lpszItemBuf);

	return 0; // success
}
