////////////////////////////////////////////////////////
//  File name:      npn.h
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: container functions provided to flash instance
////////////////////////////////////////////////////////

#ifndef NPN_H
#define NPN_H

#include "npfunctions.h"
#include "npruntime.h"

uint16_t NPVersion();

void printPluginEntrypoints(NPPluginFuncs * pFuncs);

void initNPNetscapeFuncs(NPNetscapeFuncs *bFuncs);


NPError gNPN_GetValue(NPP instance, NPNVariable variable, void *ret_value);

const char* gNPN_UserAgent(NPP instance);

NPError gNPN_GetURL(NPP instance, const char* url, const char* window);

NPIdentifier gNPN_GetStringIdentifier(const NPUTF8* name);

bool gNPN_GetProperty(NPP npp, NPObject *obj, NPIdentifier propertyName, NPVariant *result);

bool gNPN_Invoke(NPP npp, NPObject* obj, NPIdentifier methodName,
                 const NPVariant *args, uint32_t argCount, NPVariant *result);

void gNPN_ReleaseVariantValue(NPVariant *variant);

void gNPN_ReleaseObject(NPObject *obj);

NPObject*  gNPN_CreateObject(NPP npp, NPClass *aClass);

NPObject*  gNPN_RetainObject(NPObject *obj);

NPError gNPN_GetURLNotify(NPP instance, const char* url,
                          const char* target, void* notifyData);


NPError gNPN_PostURL(NPP instance, const char *url, const char *target,
                     uint32_t len, const char *buf, NPBool file);

NPError gNPN_RequestRead(NPStream *stream, NPByteRange *rangelist);

NPError gNPN_NewStream(NPP instance, NPMIMEType type,
                       const char *target, NPStream **stream);

int32_t gNPN_Write(NPP instance, NPStream *stream, int32_t len, void *buf);

NPError gNPN_DestroyStream(NPP instance, NPStream *stream, NPError reason);

void gNPN_Status(NPP instance, const char*message);

void * gNPN_MemAlloc(uint32_t size);

void gNPN_MemFree(void *ptr);

uint32_t gNPN_MemFlush(uint32_t size);

void gNPN_ReloadPlugins(NPBool reloadPages);


NPError gNPN_PostURLNotify(NPP instance, const char *url, const char *target,
             uint32_t len, const char *buf, 	NPBool file, void *notifyData);


NPError gNPN_SetValue(NPP instance, NPPVariable variable, void *value);

void gNPN_InvalidateRect(NPP instance, NPRect *invalidRect);

void gNPN_InvalidateRegion(NPP instance, NPRegion invalidRegion);

void gNPN_ForceRedraw(NPP instance);

void * gNPN_GetJavaEnv();

void * gNPN_GetJavaPeer(NPP instance);

void gNPN_PushPopupsEnabledState(NPP instance, NPBool enable);

void gNPN_PopPopupsEnabledState(NPP instance);

typedef void PluginThreadAsyncCallFunction(void *);

void gNPN_PluginThreadAsyncCall(NPP instance,
       PluginThreadAsyncCallFunction func, void *userData);


void gNPN_GetStringIdentifiers(const NPUTF8 **names,
                  int32_t nameCount, NPIdentifier *identifiers);

NPIdentifier gNPN_GetIntIdentifier(int32_t intid);

bool gNPN_IdentifierIsString(NPIdentifier identifier);

NPUTF8 * gNPN_UTF8FromIdentifier(NPIdentifier identifier);

int32_t gNPN_IntFromIdentifier(NPIdentifier identifier);

bool gNPN_InvokeDefault(NPP instance,NPObject *o,const NPVariant *args,
                        uint32_t argCount, NPVariant *result);

bool gNPN_Evaluate(NPP instance, NPObject *o, NPString *s, NPVariant *variant);

bool gNPN_SetProperty(NPP instance, NPObject *o,
                      NPIdentifier propertyName, const NPVariant *variant);

bool gNPN_RemoveProperty(NPP instance, NPObject *o, NPIdentifier propertyName);

bool gNPN_HasProperty(NPP instance, NPObject *o, NPIdentifier propertyName);

bool gNPN_HasMethod(NPP instance, NPObject *o, NPIdentifier methodname);


void gNPN_SetException(NPObject *, const NPUTF8 *message);

bool gNPN_Enumerate(NPP instance, NPObject *o,
                    NPIdentifier **identifier, uint32_t *count);

bool gNPN_Construct(NPP instance, NPObject *o, const NPVariant *args,
                    uint32_t argcount, NPVariant *result);

NPError gNPN_GetValueForURL(NPP instance, NPNURLVariable,
                            const char *url, char **value, uint32_t *len);

NPError gNPN_SetValueForURL(NPP instance, NPNURLVariable,
                            const char *url, const char *value, uint32_t len);

NPError gNPN_GetAuthenticationInfo(NPP instance,
       const char *protocol, const char *host, int32_t port,
       const char *scheme, const char* realm, char **username,
       uint32_t *ulen, char **password, uint32_t *plen);


#endif /*_NPN_H_*/
