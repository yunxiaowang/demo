////////////////////////////////////////////////////////
//  File name:      npn.cpp
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: container functions provided to flash instance
////////////////////////////////////////////////////////

#include "FlashPlugin.h"
#include "npn.h"
#include "Identifier.h"
#include "FlashContentLoader.h"
#include "Container.h"
#include "WindowNPClass.h"
#include "LocationNPClass.h"

#include <pthread.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <assert.h>

using namespace std;

//#define DEBUG

const char * uagent = "Mozilla/5.0 (X11; U; Linux x86_64; en-US; rv:1.9.2.13) Gecko/20101206 Ubuntu/10.10 (maverick) Firefox/3.6.13";

WindowNPClass __window_class;

LocationNPClass __location_class;

NPObject __window = { &__window_class, 1 };

NPObject __location = { &__location_class, 1};

#ifdef DEBUG
std::map<NPObject *, std::string> NPObjectName;
#endif

extern Container * pflashContainer;

uint16_t NPVersion()
{
	return NPVERS_HAS_PLUGIN_THREAD_ASYNC_CALL;
}

/*
 *print npp interface message
*/
void printPluginEntrypoints(NPPluginFuncs * pFuncs)
{
#ifdef DEBUG
    printf("[*] NPP struct:\n");
    printf("\t- NPP_size:                 %d\n",pFuncs->size);
    printf("\t- NPP_version:              %d\n",pFuncs->version);
    printf("\t- NPP_NewProcPtr:           0x%x\n",pFuncs->newp);
    printf("\t- NPP_DestroyProcPtr:       0x%x\n",pFuncs->destroy);
    printf("\t- NPP_SetWindowProcPtr:     0x%x\n",pFuncs->setwindow);
    printf("\t- NPP_NewStreamProcPtr:     0x%x\n",pFuncs->newstream);
    printf("\t- NPP_DestroyStreamProcPtr: 0x%x\n",pFuncs->destroystream);
    printf("\t- NPP_StreamAsFileProcPtr:  0x%x\n",pFuncs->asfile);
    printf("\t- NPP_WriteReadyProcPtr:    0x%x\n",pFuncs->writeready);
    printf("\t- NPP_WriteProcPtr:         0x%x\n",pFuncs->write);
    printf("\t- NPP_PrintProcPtr:         0x%x\n",pFuncs->print);
    printf("\t- NPP_HandleEventProcPtr:   0x%x\n",pFuncs->event);
    printf("\t- NPP_URLNotifyProcPtr:     0x%x\n",pFuncs->urlnotify);
    printf("\t- javaClass:                0x%x\n",pFuncs->javaClass);
    printf("\t- NPP_GetValueProcPtr:      0x%x\n",pFuncs->getvalue);
    printf("\t- NPP_SetValueProcPtr:      0x%x\n",pFuncs->setvalue);
	
    printf("\t- NPP_GotFocusPtr:   	      0x%x\n",pFuncs->gotfocus);
    printf("\t- NPP_LostFocusPtr:         0x%x\n",pFuncs->lostfocus);
    printf("\t- NPP_URLRedirectNotifyPtr: 0x%x\n",pFuncs->urlredirectnotify);
    printf("\t- NPP_ClearSiteDataPtr:     0x%x\n",pFuncs->clearsitedata);
    printf("\t- NPP_GetSitesWithDataPtr:  0x%x\n",pFuncs->getsiteswithdata);
#endif
}

/*
 *initial npn function
*/
void initNPNetscapeFuncs(NPNetscapeFuncs * bFuncs)
{
	memset(bFuncs,0,sizeof(*bFuncs));
    bFuncs->version = NPVersion();
	bFuncs->size=sizeof(*bFuncs);
	
    bFuncs->geturl = gNPN_GetURL;
    bFuncs->posturl = gNPN_PostURL;
    bFuncs->requestread = gNPN_RequestRead;
    bFuncs->newstream = gNPN_NewStream;
    bFuncs->write = gNPN_Write;
    bFuncs->destroystream = gNPN_DestroyStream;
    bFuncs->status = gNPN_Status;
    bFuncs->uagent = gNPN_UserAgent;
    bFuncs->memalloc = gNPN_MemAlloc;
    bFuncs->memfree = gNPN_MemFree;
    bFuncs->memflush = gNPN_MemFlush;
    bFuncs->reloadplugins = gNPN_ReloadPlugins;
    bFuncs->geturlnotify = gNPN_GetURLNotify;
    bFuncs->posturlnotify = gNPN_PostURLNotify;
    bFuncs->getvalue = gNPN_GetValue;
    bFuncs->setvalue = gNPN_SetValue;
    bFuncs->invalidaterect = gNPN_InvalidateRect;
    bFuncs->invalidateregion = gNPN_InvalidateRegion;
    bFuncs->forceredraw = gNPN_ForceRedraw;
    bFuncs->getJavaEnv = gNPN_GetJavaEnv;
    bFuncs->getJavaPeer = gNPN_GetJavaPeer;
    bFuncs->pushpopupsenabledstate = gNPN_PushPopupsEnabledState;
    bFuncs->poppopupsenabledstate = gNPN_PopPopupsEnabledState;
    bFuncs->pluginthreadasynccall = gNPN_PluginThreadAsyncCall;

    bFuncs->releasevariantvalue = gNPN_ReleaseVariantValue;
    bFuncs->getstringidentifier = gNPN_GetStringIdentifier;
    bFuncs->getstringidentifiers = gNPN_GetStringIdentifiers;
    bFuncs->getintidentifier = gNPN_GetIntIdentifier;
    bFuncs->identifierisstring = gNPN_IdentifierIsString;
    bFuncs->utf8fromidentifier = gNPN_UTF8FromIdentifier;
    bFuncs->intfromidentifier = gNPN_IntFromIdentifier;
    bFuncs->createobject = gNPN_CreateObject;
    bFuncs->retainobject = gNPN_RetainObject;
    bFuncs->releaseobject = gNPN_ReleaseObject;
    bFuncs->invoke = gNPN_Invoke;
    bFuncs->invokeDefault = gNPN_InvokeDefault;
    bFuncs->evaluate = gNPN_Evaluate;
    bFuncs->getproperty = gNPN_GetProperty;
    bFuncs->setproperty = gNPN_SetProperty;
    bFuncs->removeproperty = gNPN_RemoveProperty;
    bFuncs->hasproperty = gNPN_HasProperty;
    bFuncs->hasmethod = gNPN_HasMethod;
    bFuncs->setexception = gNPN_SetException;
    bFuncs->enumerate = gNPN_Enumerate;
    bFuncs->construct = gNPN_Construct;
    bFuncs->getvalueforurl = gNPN_GetValueForURL;
    bFuncs->setvalueforurl = gNPN_SetValueForURL;
    bFuncs->getauthenticationinfo = gNPN_GetAuthenticationInfo;

#ifdef DEBUG
    NPObjectName.clear();
    NPObjectName.insert(std::make_pair<NPObject *,std::string>(&__window,"__window"));
    NPObjectName.insert(std::make_pair<NPObject *,std::string>(&__location,"__location"));
#endif
}

/*
 *allows the plugin to query the container for information
*/
NPError gNPN_GetValue(NPP instance, NPNVariable variable, void *ret_value)
{
   switch(variable)
   {
    case NPNVToolkit:
        *(reinterpret_cast<int*>(ret_value)) = 2;
        break;
    case NPNVSupportsXEmbedBool:
        *(reinterpret_cast<NPBool*>(ret_value)) = 1;
		break;
    case NPNVWindowNPObject:
        *(reinterpret_cast<void **>(ret_value)) = &__window;
        gNPN_RetainObject(&__window);
        break;
    case NPNVprivateModeBool:
        *(reinterpret_cast<NPBool*>(ret_value))  = 0;
		break;
	default:
        *(reinterpret_cast<int*>(ret_value)) = 0;
   }	

   return NPERR_NO_ERROR;
}

/*
 *Returns the container's user agent field.
*/
const char * gNPN_UserAgent(NPP instance)
{
    return uagent;
}

//Asks the container to create a stream for the specified URL.
NPError gNPN_GetURL(NPP instance, const char * url, const char * window)
{
#ifdef DEBUG
    printf("[NPN] NPN_GetURL NPP:0x%x, url:%s\n",instance,url);
#endif
    return NPERR_NO_ERROR;
}

/*
 *Returns an opaque identifier for the string that is passed in.
*/
NPIdentifier gNPN_GetStringIdentifier(const NPUTF8 * name)
{
    return static_cast<NPIdentifier>(Identifier::get(name));
}

/*
 *Gets the value of a property on the specified NPObject
*/
bool gNPN_GetProperty(NPP npp, NPObject * obj, NPIdentifier propertyName, NPVariant *result)
{
#ifdef DEBUG
    FlashPlugin * pFlash = FlashPlugin::getFlashPluginFromNPP(npp);
    assert(pFlash);

    if(Identifier::get("location") != propertyName && Identifier::get("top") != propertyName)
    {
        if(NPObjectName.find(obj) != NPObjectName.end())
        {
            printf("[NPN] NPN_GetProperty NPP:%s, NPObject:%s, propertyName:%s\n",
                   pFlash->objectId.c_str(),NPObjectName[obj].c_str(),
                   static_cast<Identifier *>(propertyName)->getString());
        }
        else
        {
            printf("[NPN] NPN_GetProperty NPP:%s, NPObject:0x%x, propertyName:%s\n",
                   pFlash->objectId.c_str(),obj,
                   static_cast<Identifier *>(propertyName)->getString());
        }
    }
#endif

    if(obj == &__window)
    {
        if((reinterpret_cast<Identifier *>(propertyName))->isString())
        {
            const char * vstr = (reinterpret_cast<Identifier *>(propertyName))->getString();
            if(obj == &__window)
            {
                if(!strcmp(vstr,"location"))
                {
                    result->type = NPVariantType_Object;
                    result->value.objectValue = &__location;
                    gNPN_RetainObject(&__location);
                    return true;
                }
                else if(!strcmp(vstr,"top"))
                {
                    result->type = NPVariantType_Object;
                    result->value.objectValue = &__window;
                    gNPN_RetainObject(&__window);
                    return true;
                }
            }
        }
        return true;
    }
	if(obj->_class->hasProperty && obj->_class->getProperty)
	{
        if(obj->_class->hasProperty(obj,propertyName))
		{
			return obj->_class->getProperty(obj,propertyName,result);
		}

        printf("[ERROR] obj->_class->hasProperty failed!\n");
		return false;
	}

    printf("[ERROR] No obj->_class-hasProperty or no obj->_class_getProperty!\n");
	return false;
}


//releases the value in the given variant.
void gNPN_ReleaseVariantValue(NPVariant *variant) 
{
    assert(variant);

    switch(variant->type)
    {
    case NPVariantType_Void:
    case NPVariantType_Null:
    case NPVariantType_Bool:
    case NPVariantType_Int32:
    case NPVariantType_Double:
        // Nothing to do.
        break;
    case NPVariantType_String:
        free((void *)variant->value.stringValue.UTF8Characters);
        variant->value.stringValue.UTF8Characters = 0;
        variant->value.stringValue.UTF8Length = 0;
        break;
    case NPVariantType_Object:
        gNPN_ReleaseObject(variant->value.objectValue);
        variant->value.objectValue = 0;
        break;
    }
    variant->type = NPVariantType_Void;
}

//call by function gNPN_ReleaseObject to free the memory of npobject
void gNPN_DeallocateObject(NPObject * obj)
{
    if(obj)
	{
		if(obj->_class->deallocate)
        {
            obj->_class->deallocate(obj);
        }
		else
			free(obj);
	}
}

//decrements the reference count of the given NPObject. If the reference count reaches 0,
//the NPObject is deallocated by calling its deallocate function
void gNPN_ReleaseObject(NPObject * obj)
{
    if(!obj)
    {
        return;
    }

    obj->referenceCount--;

    if(!obj->referenceCount)
    {
        gNPN_DeallocateObject(obj);
    }
}


//invokes a method on the given NPObject
bool gNPN_Invoke(NPP npp, NPObject *obj, NPIdentifier methodname,
                 const NPVariant *args, uint32_t argcount, NPVariant *result)
{
    const char * methname = static_cast<Identifier *>(methodname)->getString();
    if(!strcmp(methname,"_DoFSCommand"))
    {
        BOOLEAN_TO_NPVARIANT(true,*result);
        return true;
    }


    if(!strcmp(methname,"__flash__request"))
    {
        std::string requestStr = "<invoke name=\"" + std::string(args[0].value.stringValue.UTF8Characters)
                + "\" returntype=\"javascript\"><arguments>";
        for(int i = 1; i < argcount; i++)
        {
            if(args[i].type == NPVariantType_String)
            {
                requestStr += "<string>" + std::string(args[i].value.stringValue.UTF8Characters) + "</string>";
            }
            else if(args[i].type == NPVariantType_Int32)
            {
                char numberValue[15];
                sprintf(numberValue,"%d",args[i].value.intValue);
                requestStr += "<number>" + std::string(numberValue) +"</number>";
            }
            else if(args[i].type == NPVariantType_Null)
            {
                requestStr += "<null/>";
            }
            else if(args[i].type == NPVariantType_Object)
            {
                requestStr += std::string(args[i].value.stringValue.UTF8Characters);
                printf("[WARNING] __flash__request type = object\n");
            }
        }
        requestStr += "</arguments></invoke>";

#ifdef DEBUG
        printf("request-xml: %s\n",requestStr.c_str());
#endif

        STRINGZ_TO_NPVARIANT(faststrdup(requestStr.c_str()),*result);

        return true;
    }


    if(obj)
    {
        if(obj->_class->invoke)
        {
            bool res = obj->_class->invoke(obj, methodname, args, argcount, result);
#ifdef DEBUG
            if(Identifier::get("toString") != methodname)
            {
                printf("call %s result type:%d\n",methname,result->type);
                if(result->type == NPVariantType_String)
                {
                    printf("%s result string:%s\n",methname,result->value.stringValue.UTF8Characters);
                }
            }
#endif
            return res;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//Evaluates a script in the scope of the specified NPObject.
//The script is evaluated in the context of the window
//that the calling plugin instance (the @npp argument) is loaded in.
bool gNPN_Evaluate(NPP npp, NPObject *npobj, NPString *script, NPVariant *result)
{
    FlashPlugin * pFlash = FlashPlugin::getFlashPluginFromNPP(npp);
    assert(pFlash);

    std::string objectid = pFlash->objectId;


#ifdef DEBUG
    printf("[NPN] NPN_Evaluate, NPP:%s:\n",objectid.c_str());
    printf("%s\n",script->UTF8Characters);
#endif

    VOID_TO_NPVARIANT(*result);
    return false;
}


//Allocates a new npobject
NPObject * gNPN_CreateObject(NPP npp, NPClass * aclass)
{
	if(aclass)
	{
        NPObject * obj;
		if(aclass->allocate != NULL)
		{
			obj = aclass->allocate(npp, aclass);
		}
		else 
		{
            obj = new NPObject;
		}
		if(!obj)
		{
            return NULL;
		}
		obj->_class = aclass;
        obj->referenceCount = 1;
		return obj;	
	}

    printf("[ERROR] aclass is null\n");
    return 0;
}


//increments the reference count of the given NPObject
NPObject*  gNPN_RetainObject(NPObject *obj) 
{
    assert(obj);

    if(!obj)
    {
        return NULL;
    }

    obj->referenceCount++;

    return obj;
}


//Requests creation of a new stream with the contents of the specified URL;
//gets notification of the result.
NPError gNPN_GetURLNotify(NPP instance, const char * url,
                          const char * target,void * notifyData)
{
    if(target == NULL)
    {
        if(!memcmp(url,"http",4 * sizeof(char)))
        {
            FlashContentLoader * contentLoader = new FlashContentLoader(QString(QLatin1String(url)),
                                 instance,FlashPlugin::m_NPP_PluginFuncs,notifyData);
            contentLoader->Request();
        }
        else if(!memcmp(url,"javascript",10 * sizeof(char)))
        {

        }
        else
        {

            printf("[ERROR] target==NULL, url:%s\n",url);
            FlashPlugin::m_NPP_PluginFuncs.urlnotify(instance,url,NPRES_DONE,notifyData);
        }
    }
    else
    {
#ifdef DEBUG
        printf("[NPN] target:%s, url:%s\n",target,url);
#endif

        printf("[ERROR] WebBrowser not implement!\n");

        FlashPlugin::m_NPP_PluginFuncs.urlnotify(instance,url,NPRES_DONE,notifyData);
    }

    return NPERR_NO_ERROR;
}

//Posts data to a URL. not implement
NPError gNPN_PostURL(NPP instance, const char *url, const char *target, uint32_t len, const char *buf, NPBool file)
{
    printf("[NPN] NPN_PostURL\n");
    return 0;
}

//Requests a range of bytes for a seekable stream.  not implement
NPError gNPN_RequestRead(NPStream *stream, NPByteRange *rangelist)
{
    printf("[NPN] NPN_RequestRead\n");
    return 0;
}

//Requests the creation of a new data stream produced by the plug-in
//and consumed by the browser. not implement
NPError gNPN_NewStream(NPP instance, NPMIMEType type, const char *target, NPStream **stream)
{
    printf("[NPN] NPN_NewStream NPP:0x%x, type:%s\n",instance,type);
    return 0;
}

//Pushes data into a stream produced by the plug-in and consumed by the browser. not implement
int32_t gNPN_Write(NPP instance, NPStream *stream, int32_t len, void *buf)
{
    printf("[NPN] NPN_Write NPP:0x%x\n",instance);
    return 0;
}


//Closes and deletes a stream. not implement
NPError gNPN_DestroyStream(NPP instance, NPStream *stream, NPError reason)
{
    printf("[NPN] NPN_DestroyStream NPP:0x%x",instance);
    return 0;
}


//Displays a message on the status line of the browser window. not implement
void gNPN_Status(NPP instance, const char*message)
{
    printf("[NPN] NPN_DestroyStream NPP:",instance);
}


//Allocates memory from the browser's memory space.
void * gNPN_MemAlloc(uint32_t size)
{
    void * result =  malloc(size);
	return result;
}

//Deallocates a block of allocated memory.
void gNPN_MemFree(void * ptr)
{
	free(ptr);
}

//Requests that the browser free a specified amount of memory. not implement
uint32_t gNPN_MemFlush(uint32_t size)
{
    printf("[NPN] NPN_MemFlush size:%d",size);
	return 0;
}

//Reloads all plug-ins in the Plugins directory. not implement
void gNPN_ReloadPlugins(NPBool reloadPages)
{
    printf("[NPN] NPN_ReloadPlugins\n");
}


//Posts data to a URL, and receives notification of the result.
NPError gNPN_PostURLNotify(NPP npp, const char * url, const char *target, uint32_t len,
                           const char * buf, NPBool file, void * notifyData)
{
    FlashContentLoader * pfposter = new FlashContentLoader(QString(url),npp,FlashPlugin::m_NPP_PluginFuncs,notifyData);
    pfposter->Post(QByteArray(buf,len));

    return 0;
}

//Sets windowless plug-in as transparent or opaque. not implement
NPError gNPN_SetValue(NPP instance, NPPVariable variable, void *value)
{
    printf("[NPN] NPN_SetValue\n");
	return 0;
}

//Invalidates specified drawing area prior to repainting or refreshing a windowless plug-in. not implement
void gNPN_InvalidateRect(NPP instance, NPRect *invalidRect)
{
    printf("[NPN] NPN_InvalidateRect\n");
}

//Invalidates specified drawing region prior to repainting or refreshing a windowless plug-in. not implement
void gNPN_InvalidateRegion(NPP instance, NPRegion invalidRegion)
{
    printf("[NPN] NPN_InvalidateRegion\n");
}

//Forces a paint message for a windowless plug-in. not implement
void gNPN_ForceRedraw(NPP instance)
{
    printf("[NPN] NPN_ForceRedraw\n");
}

// not implement
void * gNPN_GetJavaEnv()
{
    printf("[NPN] NPN_GetJavaEnv\n");
	return 0;
}

// not implement
void * gNPN_GetJavaPeer(NPP instance)
{
    printf("[NPN] NPN_GetJavaPeer\n");
	return 0;
}


//Pushes the popups-enabled state. not implement
void gNPN_PushPopupsEnabledState(NPP instance, NPBool enable)
{
    printf("[NPN] NPN_PushPopupsEnabledState\n");
}

//Pops the popups-enabled state. not implement
void gNPN_PopPopupsEnabledState(NPP instance)
{
    printf("[NPN] NPN_PopPopupsEnabledState\n");
}

//Thread-safe way to request that the browser calls a plugin function
//on the browser or plugin thread (the thread on which the plugin was initiated). not implement
void gNPN_PluginThreadAsyncCall(NPP instance, PluginThreadAsyncCallFunction func, void *userData)
{
    printf("[NPN] NPN_PluginThreadAsyncCall\n");
}

//Returns an array of opaque identifiers for the names that are passed in.
void gNPN_GetStringIdentifiers(const NPUTF8 ** names, int32_t nameCount, NPIdentifier * identifiers)
{
	if(names && identifiers)
	{
        for(int i = 0; i < nameCount;i++)
        {
            identifiers[i] = gNPN_GetStringIdentifier(names[i]);
        }
    }
    else
    {
        printf("[ERROR] names or identifiers are null\n");
    }
}

//Returns an opaque identifier for the integer that is passed in.
NPIdentifier gNPN_GetIntIdentifier(int32_t intid)
{
    NPIdentifier NPid = static_cast<NPIdentifier>(Identifier::get(intid));
    return NPid;
}

//Determines whether or not an identifier is a string.
bool gNPN_IdentifierIsString(NPIdentifier identifier)
{
    return static_cast<Identifier *>(identifier)->isString();
}

//Returns the UTF-8 string corresponding to the given string identifier.
NPUTF8 * gNPN_UTF8FromIdentifier(NPIdentifier identifier)
{
    const char * st = static_cast<Identifier *>(identifier)->getString();
	if(!st)
    {
        return 0;
    }

	return strdup(st);
}

//Returns the integer value corresponding to the given integer identifier.
int32_t gNPN_IntFromIdentifier(NPIdentifier identifier)
{
    int result = static_cast<Identifier *>(identifier)->getNumber();
    return result;
}

//Invokes the default method, if one exists, on the given NPObject.
bool gNPN_InvokeDefault(NPP instance, NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    if(obj == &__window)
    {
        return true;
    }
	if(obj )
	{
		if(obj->_class->invokeDefault)
		{
			return obj->_class->invokeDefault(obj, args, argCount, result);
        }
        else
		{
            return false;
		}
    }
    else
	{
        return false;
	}
}



//Sets the value of a property on the specified NPObject.
bool gNPN_SetProperty(NPP instance, NPObject *obj, NPIdentifier propertyName, const NPVariant *variant)
{
	if(obj && obj->_class && obj->_class->setProperty )
	{
		return obj->_class->setProperty(obj, propertyName, variant);
	}

    printf("[ERROR] obj or obj->_class or obj->_class->setproperty is null \n");
	return false;
}

//Removes a property from the specified NPObject. not implement
bool gNPN_RemoveProperty(NPP instance, NPObject *o, NPIdentifier propertyName)
{
    printf("[NPN] NPN_RemoveProperty\n");
	return 0;
}

//Determines whether or not the specified NPObject has a particular property.
bool gNPN_HasProperty(NPP instance, NPObject * obj, NPIdentifier propertyName)
{
    if(obj == &__window)
    {
        return true;
    }
	if(obj && obj->_class && obj->_class->hasProperty )
	{
		return obj->_class->hasProperty(obj, propertyName);
	}

    printf("[ERROR] obj or obj->_class or obj->_class->hasproperty is null \n");
	return false;
}

//Determines whether or not the specified NPObject has a particular method.
bool gNPN_HasMethod(NPP instance, NPObject *obj, NPIdentifier methodname)
{
	if(obj && obj->_class && obj->_class->hasMethod)
	{
        bool hasMed = obj->_class->hasMethod(obj,methodname);
        return hasMed;
    }
    else
	{
        printf("[ERROR] obj or obj->_class or obj->_class->hasMethod is null \n");
		return false;
	}
}

//A plugin can call this function to indicate that a call to one of the plugin's NPObjects
//generated an error. not implement
void gNPN_SetException(NPObject *obj, const NPUTF8 *message)
{
    printf("[NPN] NPN_SetException %s\n",message);
}


bool gNPN_Enumerate(NPP instance, NPObject *obj, NPIdentifier **identifier, uint32_t *count)
{
    if(obj == &__window)
    {
        return true;
    }
    if(obj->_class && obj->_class->enumerate)
    {
        return obj->_class->enumerate(obj,identifier,count);
    }
	else 
    {
        printf("[ERROR] obj->_class or obj->enumerate is null\n");
    }
	return false;
}

// not implement
bool gNPN_Construct(NPP instance, NPObject *obj, const NPVariant *args,
                    uint32_t argcount, NPVariant *result)
{
    return false;
}

//Provides information to a plugin which is associated with a given URL,
//for example the cookies or preferred proxy. not implement
NPError gNPN_GetValueForURL(NPP instance, NPNURLVariable npnurlvariable ,const char *url,char **value, uint32_t *len)
{
    printf("[NPN] NPN_GetValueForURL\n");
	return 0;
}

//Allows a plugin to change the stored information associated with a URL,
//in particular its cookies. not implement
NPError gNPN_SetValueForURL(NPP instance, NPNURLVariable npnurlvariable, const char *url,const char *value, uint32_t len)
{
    printf("[NPN] NPN_SetValueForURL\n");
	return 0;
}


//This function is called by plugins to get HTTP authentication information from the browser. not implement
NPError gNPN_GetAuthenticationInfo(NPP instance, const char *protocol, const char *host, int32_t port, const char *scheme,
                                   const char *realm, char**username, uint32_t *ulen, char **password, uint32_t *plen)
{
    printf("[NPN] NPN_GetAuthenticationInfo\n");
	return 0;
}
