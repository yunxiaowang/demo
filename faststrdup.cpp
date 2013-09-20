////////////////////////////////////////////////////////
//  File name:      faststrdup.h
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: fast duplicate string
////////////////////////////////////////////////////////

#include "faststrdup.h"

#include <stdlib.h>
#include <string.h>

using namespace std;


/*
 *duplicate a string @src using new
*/
char * faststrdup(const char * src)
{
    if(NULL == src)
    {
        return NULL;
    }

    size_t len = strlen(src)+1;
    char * dup = new char[len];
	memcpy(dup, src, len);
	return dup;
}
