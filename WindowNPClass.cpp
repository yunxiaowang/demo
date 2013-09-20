////////////////////////////////////////////////////////
//  File name:      WindowNPClass.cpp
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: implementation of browser's window class
////////////////////////////////////////////////////////

#include "WindowNPClass.h"
#include "Identifier.h"
#include "npruntime.h"
#include "npn.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#define DEBUG

NPObject __top;
NPObject __top_location;

WindowNPClass::WindowNPClass()
{
    this->structVersion = NP_CLASS_STRUCT_VERSION;
    this->allocate = WNP_Allocate;
    this->deallocate = WNP_Deallocate;
    this->invalidate = WNP_Invalidate;
    this->hasMethod = WNP_HasMethod;
    this->invoke = WNP_Invoke;
    this->invokeDefault = WNP_InvokeDefault;
    this->hasProperty = WNP_HasProperty;
    this->getProperty = WNP_GetProperty;
    this->setProperty = WNP_SetProperty;
    this->removeProperty = WNP_RemoveProperty;
    this->enumerate = WNP_Enumeration;
    this->construct = WNP_Construct;
}


NPObject * WindowNPClass::WNP_Allocate(NPP npp, NPClass *aClass)
{
#ifdef DEBUG
    printf("[WNP] call WNP_Allocate\n");
#endif
    NPObject * obj = reinterpret_cast<NPObject *>(new WindowNPClass);
    return obj;
}

void WindowNPClass::WNP_Deallocate(NPObject *npobj)
{
#ifdef DEBUG
    printf("[WNP] call WNP_Deallocate\n");
#endif
    delete npobj;
}

void WindowNPClass::WNP_Invalidate(NPObject *npobj)
{
#ifdef DEBUG
    printf("[WNP] call WNP_Invalidate\n");
#endif
}

bool WindowNPClass::WNP_HasMethod(NPObject *npobj, NPIdentifier name)
{
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
#ifdef DEBUG
    printf("[WNP] call WNP_HasMethod,name:%s\n",strname);
#endif
    return false;
}

bool WindowNPClass::WNP_Invoke(NPObject *npobj, NPIdentifier name,
        const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
#ifdef DEBUG
    printf("[WNP] call WNP_Invoke,name:%s, argCount:%d\n",strname,argCount);
#endif

    return false;
}

bool WindowNPClass::WNP_InvokeDefault(NPObject *npobj,const NPVariant *args,
        uint32_t argCount, NPVariant *result)
{
#ifdef DEBUG
    printf("[WNP] call WNP_InvokeDefault\n");
#endif
    return false;
}

bool WindowNPClass::WNP_HasProperty(NPObject *npobj, NPIdentifier name)
{
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
#ifdef DEBUG
    printf("[WNP] call WNP_HasProperty,name:%s\n",strname);
#endif
    if(!strcmp(strname,"location"))
        return true;

    return false;
}

bool WindowNPClass::WNP_GetProperty(NPObject * npobj, NPIdentifier name, NPVariant *result)
{
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
    if(npobj == &__top)
    {
        if(strname == "location")
        {
            result->type = NPVariantType_Object;
            result->value.objectValue = &__top_location;
            gNPN_RetainObject(&__top_location);
//            printf("[NPN] NPN_GetProperty __top_location, obj=__top\n");
            return true;
        }
    }
#ifdef DEBUG
    printf("[WNP] call WNP_GetProperty,name:%s\n",strname);
#endif
    return false;
}

bool WindowNPClass::WNP_SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
{
#ifdef DEBUG
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
    printf("[WNP] call WNP_SetProperty,name:%s\n",strname);
#endif
    return false;
}

bool WindowNPClass::WNP_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
#ifdef DEBUG
    const char * strname = reinterpret_cast<Identifier *>(name)->getString();
    printf("[WNP] call WNP_RemoveProperty,name:%s\n",strname);
#endif
    return false;
}

bool WindowNPClass::WNP_Enumeration(NPObject *npobj, NPIdentifier **value, uint32_t *count)
{
#ifdef DEBUG
    printf("[WNP] call WNP_Enumeration\n");
#endif
    return false;
}

bool WindowNPClass::WNP_Construct(NPObject *npobj, const NPVariant *args,
        uint32_t argCount, NPVariant *result)
{
#ifdef DEBUG
    printf("[WNP] call WNP_Construct\n");
#endif
    return false;
}
