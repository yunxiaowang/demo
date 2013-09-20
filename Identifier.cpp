////////////////////////////////////////////////////////
//  File name:      Identifier.cpp
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: NPP identifier to deal with string and variable
////////////////////////////////////////////////////////

#include "Identifier.h"

#include <set>
#include <map>
#include <string>
#include <stdio.h>

using namespace std;

set<Identifier *> IdentifierSet;
map<int, Identifier *> IntIdentifierMap;
map<string, Identifier *> StringIdentifierMap;

//#define DEBUG

/*
 *query weather a identifier is validated
*/
bool Identifier::isValid(Identifier * idptr)
{
//    return IdentifierSet.count(idptr);
    return IdentifierSet.find(idptr) != IdentifierSet.end();
}


/*
 *gain a identifier for a integer
*/
Identifier * Identifier::get(int id)
{	
    map<int,Identifier*>::iterator it = IntIdentifierMap.find(id);
    if(it != IntIdentifierMap.end())//if exists
	{
		return it->second;
    }
    else//if not exists
	{
		Identifier * newiden = new Identifier(id);
#ifdef DEBUG
        printf("[Identifier] a new Identifier, %d\n",id);
#endif
		IntIdentifierMap.insert(map<int, Identifier*>::value_type(id,newiden));
		IdentifierSet.insert(newiden);
		return newiden;
	}
}


/*
 *gain identifier related to string @name
*/
Identifier * Identifier::get(const char * name)
{
    string sname(name);
    map<string,Identifier *>::iterator it = StringIdentifierMap.find(sname);
    if(it != StringIdentifierMap.end())
	{
        return it->second;
    }
    else
	{
		Identifier * newiden = new Identifier(name);
#ifdef DEBUG
        printf("[Identifier] a new Identifier, %s\n",name);
#endif
		StringIdentifierMap.insert(map<string,Identifier*>::value_type(sname,newiden));
		IdentifierSet.insert(newiden);

		return newiden;
	}
}

