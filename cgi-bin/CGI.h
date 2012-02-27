/**********************************************************
*CGI.h                                                    *
* Class declarations for a rudimentary CGI input parser   *
* Last Updated 8/7/1998 by Mike Lin                       *
**********************************************************/

#ifndef CGI_H
#define	CGI_H

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************
* CCGIItem                                                *
* Represents an individual item passed to the program, and*
* allows you to retrieve its name and value separately.   *
**********************************************************/

class CCGIItem
{
public:
	CCGIItem();				// constructor
	~CCGIItem();			// deconstructor

	// All the following return -1 in the event of an error

	int GetLength();		// retrieve length, in bytes, of the value
	int GetNameLength();	// retrieve length, in bytes, of the name

	int GetValue(char* lpszBuffer, int cbBufferLength);	// retrieve value
	int GetName(char* lpszBuffer, int cbBufferLength);	// retrieve name

	int SetValue(char* lpszBuffer);		// set value
	int SetName(char* lpszBuffer);		// set name

protected:

	char* m_lpszValue;
	char* m_lpszName;
};

/**********************************************************
* CCGI                                                    *
* A thin wrapper that parses standard POST-method CGI     *
* input.                                                  *
**********************************************************/

class CCGI
{
public:
	CCGI();		// constructor
	~CCGI();	// deconstructor

	// All of the following return -1 in the event of an error

	int Load();		// Read data and parse it

	int GetItemCount();	// Retrieve # of items

	int GetItem(int nItemIndex, CCGIItem* pItem);	// Retrieve an item

protected:
	char* m_lpszRawData;
};

#endif
