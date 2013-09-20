////////////////////////////////////////////////////////
//  File name:      WindowNPClass.h
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: implementation of browser's window class
////////////////////////////////////////////////////////

#ifndef WINDOWNPOBJECT_H
#define WINDOWNPOBJECT_H

#include "npapi.h"
#include "npruntime.h"

class WindowNPClass : public NPClass
{
public:
    WindowNPClass();

private:
    static NPObject * WNP_Allocate(NPP npp, NPClass *aClass);
    static void WNP_Deallocate(NPObject *npobj);
    static void WNP_Invalidate(NPObject *npobj);
    static bool WNP_HasMethod(NPObject *npobj, NPIdentifier name);
    static bool WNP_Invoke(NPObject *npobj, NPIdentifier name,
        const NPVariant *args, uint32_t argCount, NPVariant *result);
    static bool WNP_InvokeDefault(NPObject *npobj,const NPVariant *args,
        uint32_t argCount, NPVariant *result);
    static bool WNP_HasProperty(NPObject *npobj, NPIdentifier name);
    static bool WNP_GetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result);
    static bool WNP_SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value);
    static bool WNP_RemoveProperty(NPObject *npobj, NPIdentifier name);
    static bool WNP_Enumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count);
    static bool WNP_Construct(NPObject *npobj, const NPVariant *args,
        uint32_t argCount, NPVariant *result);
};
#endif
