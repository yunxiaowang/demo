////////////////////////////////////////////////////////
//  File name:      LocationNPClass.h
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: implementation of browser's location class
////////////////////////////////////////////////////////

#ifndef LOCATIONNPOBJECT_H
#define LOCATIONNPOBJECT_H

#include "npapi.h"
#include "npruntime.h"

class LocationNPClass : public NPClass
{
public:
    LocationNPClass();

private:
    static NPObject * LNP_Allocate(NPP npp, NPClass *aClass);
    static void LNP_Deallocate(NPObject *npobj);
    static void LNP_Invalidate(NPObject *npobj);
    static bool LNP_HasMethod(NPObject *npobj, NPIdentifier name);
    static bool LNP_Invoke(NPObject *npobj, NPIdentifier name,
        const NPVariant *args, uint32_t argCount, NPVariant *result);
    static bool LNP_InvokeDefault(NPObject *npobj,const NPVariant *args,
        uint32_t argCount, NPVariant *result);
    static bool LNP_HasProperty(NPObject *npobj, NPIdentifier name);
    static bool LNP_GetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result);
    static bool LNP_SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
    static bool LNP_RemoveProperty(NPObject *npobj, NPIdentifier name);
    static bool LNP_Enumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count);
    static bool LNP_Construct(NPObject *npobj, const NPVariant *args,
        uint32_t argCount, NPVariant *result);
private:
    static char * get_Location();
};

#endif // LOCATIONNPOBJECT_H
