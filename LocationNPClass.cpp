////////////////////////////////////////////////////////
//  File name:      LocationNPClass.cpp
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: implementation of browser's location class
////////////////////////////////////////////////////////

#include "Container.h"
#include "LocationNPClass.h"
#include "Identifier.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#define DEBUG

extern Container * pflashContainer;

LocationNPClass::LocationNPClass()
{
    this->structVersion = NP_CLASS_STRUCT_VERSION;
    this->allocate = LNP_Allocate;
    this->deallocate = LNP_Deallocate;
    this->invalidate = LNP_Invalidate;
    this->hasMethod = LNP_HasMethod;
    this->invoke = LNP_Invoke;
    this->invokeDefault = LNP_InvokeDefault;
    this->hasProperty = LNP_HasProperty;
    this->getProperty = LNP_GetProperty;
    this->setProperty = LNP_SetProperty;
    this->removeProperty = LNP_RemoveProperty;
    this->enumerate = LNP_Enumeration;
    this->construct = LNP_Construct;
}


NPObject * LocationNPClass::LNP_Allocate(NPP npp, NPClass * aClass)
{
#ifdef DEBUG
    printf("[LNP] call LNP_Allocate\n");
#endif
    NPObject * obj = reinterpret_cast<NPObject *>(new LocationNPClass);
    return obj;
}

void LocationNPClass::LNP_Deallocate(NPObject *npobj)
{
#ifdef DEBUG
    printf("[LNP] call LNP_Deallocate\n");
#endif
    delete npobj;
}

void LocationNPClass::LNP_Invalidate(NPObject *npobj)
{
#ifdef DEBUG
    printf("[LNP] call LNP_Invalidate\n");
#endif
}

bool LocationNPClass::LNP_HasMethod(NPObject *npobj, NPIdentifier name)
{
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();

    if(!strcmp(strname,"toString"))
    {
        return true;
    }
    return false;
}

bool LocationNPClass::LNP_Invoke(NPObject *npobj, NPIdentifier name,
        const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();

    if(!strcmp(strname,"toString"))
    {
        result->type = NPVariantType_String;
        result->value.stringValue.UTF8Characters = get_Location();
        result->value.stringValue.UTF8Length = strlen(result->value.stringValue.UTF8Characters);
        return true;
    }
    return false;
}

bool LocationNPClass::LNP_InvokeDefault(NPObject *npobj,const NPVariant *args,
        uint32_t argCount, NPVariant *result)
{
#ifdef DEBUG
    printf("[LNP] call LNP_InvokeDefault\n");
#endif
    return false;
}

bool LocationNPClass::LNP_HasProperty(NPObject *npobj, NPIdentifier name)
{
#ifdef DEBUG
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
    printf("[LNP] call LNP_HasProperty,name:%s\n",strname);
#endif
    return false;
}

bool LocationNPClass::LNP_GetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
{
#ifdef DEBUG
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
    printf("[LNP] call LNP_GetProperty,name:%s\n",strname);
#endif
    return false;
}

bool LocationNPClass::LNP_SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
#ifdef DEBUG
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
    printf("[LNP] call LNP_SetProperty,name:%s\n",strname);
#endif
    return false;
}

bool LocationNPClass::LNP_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
#ifdef DEBUG
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
    printf("[LNP] call LNP_RemoveProperty,name:%s\n",strname);
#endif
    return false;
}

bool LocationNPClass::LNP_Enumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
#ifdef DEBUG
    printf("[LNP] call LNP_Enumeration\n");
#endif
    return false;
}

bool LocationNPClass::LNP_Construct(NPObject *npobj, const NPVariant *args,
        uint32_t argCount, NPVariant *result)
{
#ifdef DEBUG
    printf("[LNP] call LNP_Construct\n");
#endif
    return false;
}

char * LocationNPClass::get_Location()
{
    return faststrdup(pflashContainer->ContainerURL.c_str());
}
