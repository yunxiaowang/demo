////////////////////////////////////////////////////////
//  File name:      FlashPlugin.cpp
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: Flash Plugin wrapped class
////////////////////////////////////////////////////////

#include <QString>
#include <QApplication>
#include <QLibrary>

#include "FlashPlugin.h"
#include "npn.h"
#include "faststrdup.h"
#include "FlashContentLoader.h"
#include "Identifier.h"

#include <sys/syscall.h>
#include <assert.h>
#include <stdio.h>

//#define DEBUG

const int  MAX_SIZE = 256;
const char * const FlashPluginName = "libflashplayer.so";

//static members
char * FlashPlugin::m_MIMEDescription;
char * FlashPlugin::m_PluginName;
char * FlashPlugin::m_PluginDescription;
int FlashPlugin::m_FlashInstanceNumber = 0;
QLibrary * FlashPlugin::m_LibraryHandle;
NPFunctionsStruct  FlashPlugin::m_NP_Func;
NPPluginFuncs FlashPlugin::m_NPP_PluginFuncs;
NPNetscapeFuncs FlashPlugin::m_NPN_funcs;


std::map<NPP,FlashPlugin *> FlashPlugin::NPPtoFlashPlugin;

typedef void gtkInitFunc(int * argc, char *** argv);


/*
 *FlashPlugin construct function
*/
FlashPlugin::FlashPlugin(const char * swffile ,QWidget * parent,
      int x, int y, int width, int height, QVector<QString> paramnames,QVector<QString> paramvalues)
{
#ifdef DEBUG
    printf("Begin to new FlashPlugin\n");
#endif

    m_FlashInstanceNumber++;
    m_SWFFile = faststrdup(swffile);
    m_pParentWidget = parent;

    m_FlashId = 0;

    ReadyState = 3;
    objectId = "(NoObjectId)";

    m_FlashInstance = new NPP_t();
    m_pNPObject = new NPObject();

//    m_pSaveddata = new NPSavedData();
//    memset(m_pSaveddata,0,sizeof(NPSavedData));

    SetParameters(paramnames,paramvalues);

#ifdef DEBUG
    printf("#######NPP_New START##############\n");
#endif

    NPPtoFlashPlugin.insert(std::make_pair<NPP,FlashPlugin *>(m_FlashInstance,this));
    m_NPP_PluginFuncs.newp((NPMIMEType)"application/x-shockwave-flash",
                       m_FlashInstance,NP_EMBED,m_ParamCount,m_ParamNames,m_ParamValues,/*m_pSaveddata*/NULL);// new a new flash instance

#ifdef DEBUG
    printf("#######NPP_New END##############\n");
#endif

    //get NPP plugin Scriptable NPObject
    GetNPObject();


    NPWindowConstruct(x,y,width,height,x,y,width,height);
    m_NPP_PluginFuncs.setwindow(m_FlashInstance,m_pNPWindow);
}


FlashPlugin * FlashPlugin::getFlashPluginFromNPP(NPP npp)
{
    if(NPPtoFlashPlugin.find(npp) != NPPtoFlashPlugin.end())
    {
        return NPPtoFlashPlugin[npp];
    }
    else
        return NULL;
}

/*
 *set flash parameters
*/
void FlashPlugin::SetParameters(const QVector<QString> paramnames, const QVector<QString> paramvalues)
{
    Q_ASSERT(paramnames.size() == paramvalues.size());

    int size = paramnames.size();
    int paramcount = 0;

    m_ParamNames = new char *[size];
    m_ParamValues = new char *[size];

    for(int i = 0; i < size; i++)
    {
        m_ParamNames[i] = faststrdup((char *)paramnames[i].toLatin1().data());
        m_ParamValues[i] = faststrdup((char *)paramvalues[i].toLatin1().data());
        paramcount++;
    }

    m_ParamCount = paramcount;
}


bool FlashPlugin::InitFlashPlugin()
{
    if(false == LoadFlashPlayerSo())
    {
        return false;
    }

    if(false == LoadNPFunctions())
    {
        return false;
    }

    if(false == GetPluginInfo())
    {
        return false;
    }

    //initial npn functions
    initNPNetscapeFuncs(&m_NPN_funcs);

    //initial NPP functions
    if(NPERR_NO_ERROR != m_NP_Func.m_NP_Initialize(&m_NPN_funcs, &m_NPP_PluginFuncs))
    {
        printf("m_NP_Initialize error\n");
        return false;
    }

    NPPtoFlashPlugin.clear();

    return true;
}

bool FlashPlugin::LoadFlashPlayerSo()
{
    char CurrentPath[MAX_SIZE];
    getcwd(CurrentPath,MAX_SIZE);

    strcat(CurrentPath,"/");
    strcat(CurrentPath,FlashPluginName);

//    printf("CurrentPath:%s\n",CurrentPath);

    QString filepath = QString(CurrentPath);
    m_LibraryHandle = new QLibrary(filepath);
    m_LibraryHandle->setLoadHints(QLibrary::ResolveAllSymbolsHint);
    if(!m_LibraryHandle->load())
    {
        printf("[ERROR] LOADING LIBFLASHPLAYER.SO ERROR!\n");
        return false;
    }

#ifdef DEBUG
    printf("succeed to load libflashplayer.so!");
#endif

    //initialize gtk
    gtkInitFunc* gtkInit;
    gtkInit = (gtkInitFunc*)m_LibraryHandle->resolve("gtk_init");
    gtkInit(0,0);

    return true;
}

bool FlashPlugin::LoadNPFunctions()
{
    m_NP_Func.m_NP_Initialize = (NP_InitializeFunc)m_LibraryHandle->resolve("NP_Initialize");
    m_NP_Func.m_NP_Shutdown = (NP_ShutdownFunc)m_LibraryHandle->resolve("NP_Shutdown");
    m_NP_Func.m_NP_GetMIMEDescription = (NP_GetMIMEDescriptionFunc)m_LibraryHandle->resolve("NP_GetMIMEDescription");
    m_NP_Func.m_NP_GetValue = (NP_GetValueFunc)m_LibraryHandle->resolve("NP_GetValue");

    if(!m_NP_Func.m_NP_Initialize || !m_NP_Func.m_NP_Shutdown
            || !m_NP_Func.m_NP_GetMIMEDescription || !m_NP_Func.m_NP_GetValue)
    {
        printf("[ERROR] FAILD TO LOAD NP FUNCTIONS!");
        return false;
    }
    return true;
}

bool FlashPlugin::GetPluginInfo()
{
    m_MIMEDescription = NULL;
    m_MIMEDescription = m_NP_Func.m_NP_GetMIMEDescription();

    m_PluginName = NULL;
    m_NP_Func.m_NP_GetValue(NULL,NPPVpluginNameString,&m_PluginName);

    m_PluginDescription = NULL;
    m_NP_Func.m_NP_GetValue(NULL,NPPVpluginDescriptionString,&m_PluginDescription);

#ifdef DEBUG
    printf("m_MIMEDescription: %s\n",m_MIMEDescription);
    printf("m_PluginName: %s\n",m_PluginName);
    printf("m_PluginDescription: %s\n",m_PluginDescription);
#endif

    return true;
}

bool FlashPlugin::UninitFlashPlugin()
{
    if(NPERR_NO_ERROR != m_NP_Func.m_NP_Shutdown())
    {
        return false;
    }

    printf("after shutdown\n");

    if(false == m_LibraryHandle->unload())
    {
        return false;
    }

    if(m_LibraryHandle != NULL)
    {
        m_LibraryHandle->deleteLater();
        m_LibraryHandle = NULL;
    }

    return true;
}

int FlashPlugin::GetFlashInstanceNumber()
{
    return m_FlashInstanceNumber;
}


/*
 *construct NP Window
*/
void FlashPlugin::NPWindowConstruct(int x, int y, int width, int height,
                                    int clipx, int clipy, int clipwidth, int clipheight)
{
    if(!m_pParentWidget)
    {
        printf("PARENT WIDGET COULD NOT BE NULL WHEN CONSTRUCT THE NP WINDOW");
        return;
    }

    m_pContainer = new QX11EmbedContainer(m_pParentWidget);
    QApplication::syncX();

    m_pContainer->setGeometry(x,y,width,height);

    m_pContainer->show();

    m_pNPWindow = new NPWindow();
    m_pNPWindow->window = (void *)(m_pContainer->winId());

    m_pNPWindow->x = x;
    m_pNPWindow->y = y;
    m_pNPWindow->width = width;
    m_pNPWindow->height = height;
    m_pNPWindow->clipRect.left = clipx;
    m_pNPWindow->clipRect.top = clipy;
    m_pNPWindow->clipRect.right = clipx + clipwidth;
    m_pNPWindow->clipRect.bottom = clipy + clipheight;

    const QX11Info *X11info = &(m_pContainer->x11Info());
    m_pWinCallbackStruct = new NPSetWindowCallbackStruct;
    m_pWinCallbackStruct->type = NP_SETWINDOW;
    m_pWinCallbackStruct->display = X11info->display();
    m_pWinCallbackStruct->visual = (Visual *)X11info->visual();
    m_pWinCallbackStruct->depth = X11info->depth();
    m_pWinCallbackStruct->colormap = X11info->colormap();

    m_pNPWindow->ws_info = m_pWinCallbackStruct;
    m_pNPWindow->type = NPWindowTypeWindow;
}


//play flash
void FlashPlugin::Play()
{
    if(NULL != m_SWFFile)
    {
        SendNPStream(m_SWFFile);
    }
    else
    {
        printf("[ERROR] FlashPlugin m_SWFFile = NULL!\n");
    }
}

/*
 *push the stream into the plugin
*/
void FlashPlugin::SendNPStream(char * swffile)
{
#ifdef DEBUG
    printf("[STREAM] send flashstream %s\n",swffile);
#endif

    if(!memcmp(swffile,"http",4))
    {
        QString url =  QString(QLatin1String(swffile));
        FlashContentLoader * flashloader = new FlashContentLoader(url,m_FlashInstance,m_NPP_PluginFuncs);
        flashloader->Request();
    }
    else//swffile is local resource
    {
            uint16_t stype = NP_NORMAL;
            NPStream * stream = new NPStream();
            stream->url="meaningless";//meaningless
            stream->ndata 	= 0;
            stream->end 	= 0;//if donot know the stream's size,set end to be 0
            stream->lastmodified= 0;
            stream->notifyData 	= 0x00000000;
            stream->headers = new char[512];

            m_NPP_PluginFuncs.newstream(m_FlashInstance,(NPMIMEType)("application/octet-stream"),stream,0,&stype);

            FILE * pp;
            pp = fopen(swffile,"rb");
            if(!pp)
            {
                printf("[ERROR] file: %s open failed!\n",swffile);
                m_NPP_PluginFuncs.destroystream(m_FlashInstance,stream,NPRES_DONE);
                delete[] stream->headers;
                delete stream;
                exit(0);
            }

            char buffer[8192];
            int len;
            int bytesReady;
            while((len = fread(buffer,1,sizeof(buffer),pp)) != 0)
            {
                bytesReady = m_NPP_PluginFuncs.writeready(m_FlashInstance,stream);
                if(len <= bytesReady)
                {
//                    printf("len:%d\n",len);
//                    int tmp =
                            m_NPP_PluginFuncs.write(m_FlashInstance,stream,0,len,buffer);
//                    printf("writen:%d\n",tmp);
                }
                else
                {
                    printf("fread result is larger than plugin's writeready");
                    fclose(pp);
                    m_NPP_PluginFuncs.destroystream(m_FlashInstance,stream,NPRES_DONE);
                    delete[] stream->headers;
                    delete stream;
                    exit(0);
                }
            }

            fclose(pp);
            m_NPP_PluginFuncs.destroystream(m_FlashInstance,stream,NPRES_DONE);
            delete[] stream->headers;
            delete stream;
    }
}


void FlashPlugin::GetNPObject()
{
    Q_ASSERT(m_FlashInstance && m_pNPObject);

    m_NPP_PluginFuncs.getvalue(m_FlashInstance, NPPVpluginScriptableNPObject, &m_pNPObject);

    Q_ASSERT(m_pNPObject);
    Q_ASSERT(m_pNPObject->_class);
}

bool FlashPlugin::NP_hasMethod(const char * methodName)
{
    assert(m_FlashInstance && m_pNPObject);
    NPIdentifier name = gNPN_GetStringIdentifier(methodName);

#ifdef DEBUG
    printf("call FlashPlugin::NP_hasMethod methodName:%s \n",methodName);
#endif

    return gNPN_HasMethod(m_FlashInstance,m_pNPObject,name);
}

bool FlashPlugin::NP_hasMethod(NPIdentifier nameId)
{
    return gNPN_HasMethod(m_FlashInstance,m_pNPObject,nameId);
}

bool FlashPlugin::NP_invoke(NPIdentifier name, const NPVariant *args,
                   uint32_t argCount, NPVariant *result)
{
    if(!NP_hasMethod(name))
    {
        printf("NO METHOD NAMED %s\n",reinterpret_cast<Identifier *>(name)->getString());
        return false;
    }

    bool res = gNPN_Invoke(m_FlashInstance,m_pNPObject,name,args,argCount,result);

    return res;
}


bool FlashPlugin::NP_hasProperty(NPIdentifier name)
{
    return gNPN_HasProperty(m_FlashInstance,m_pNPObject,name);
}

bool FlashPlugin::NP_hasProperty(const char * propertyName)
{
    assert(m_FlashInstance && m_pNPObject);
    NPIdentifier name = gNPN_GetStringIdentifier(propertyName);

#ifdef DEBUG
    printf("call FlashPlugin::NP_hasProperty PropertyName:%s \n",propertyName);
#endif

    return gNPN_HasProperty(m_FlashInstance,m_pNPObject,name);
}

bool FlashPlugin::NP_getProperty(NPIdentifier name, NPVariant *result)
{
    assert(m_FlashInstance && m_pNPObject);
    return gNPN_GetProperty(m_FlashInstance,m_pNPObject,name,result);
}

bool FlashPlugin::NP_getProperty(const char * propertyName, NPVariant *result)
{
    assert(m_FlashInstance && m_pNPObject);
    NPIdentifier name = gNPN_GetStringIdentifier(propertyName);

#ifdef DEBUG
    printf("call FlashPlugin::NP_getProperty PropertyName:%s \n",propertyName);
#endif

    return gNPN_GetProperty(m_FlashInstance,m_pNPObject,name,result);
}

//not implement, to do
bool FlashPlugin::NP_setProperty(NPIdentifier name, const NPVariant *value)
{
    return true;
}

//not implement, to do
bool FlashPlugin::NP_removeProperty(NPIdentifier name)
{
    return true;
}


void FlashPlugin::SetVisible(bool isVisible)
{
    if(isVisible)
    {
        m_pContainer->show();
    }
    else
        m_pContainer->hide();
}

/*
 *FlashPlugin destruction
*/
FlashPlugin::~FlashPlugin()
{
    m_NPP_PluginFuncs.destroy(m_FlashInstance,/*&m_pSaveddata*/ NULL);
    m_FlashInstanceNumber--;

    if(m_FlashInstance != NULL)
    {
        delete m_FlashInstance;
        m_FlashInstance = NULL;
    }

    if(m_pContainer != NULL)
    {
        m_pContainer->deleteLater();
        m_pContainer = NULL;
    }

    if(m_pNPWindow != NULL)
    {
        delete m_pNPWindow;
        m_pNPWindow = NULL;
    }

    if(m_pWinCallbackStruct != NULL)
    {
        delete m_pWinCallbackStruct;
        m_pWinCallbackStruct = NULL;
    }

    if(m_pNPObject != NULL)
    {
        delete m_pNPObject;
        m_pNPObject = NULL;
    }


//    if(m_pSaveddata != NULL)
//    {
//        delete m_pSaveddata;
//        m_pSaveddata = NULL;
//    }

    if(m_SWFFile != NULL)
    {
        delete[] m_SWFFile;
        m_SWFFile = NULL;
    }

    if(m_ParamNames != NULL)
    {
        for(int i = 0; i < m_ParamCount; i++)
        {
            if(m_ParamNames[i] != NULL)
            {
                delete[] m_ParamNames[i];
                m_ParamNames[i] = NULL;
            }
        }
        delete[] m_ParamNames;
        m_ParamNames = NULL;
    }

    if(m_ParamValues != NULL)
    {
        for(int i = 0; i < m_ParamCount; i++)
        {
            if(m_ParamValues[i] != NULL)
            {
                delete[] m_ParamValues[i];
                m_ParamValues[i] = NULL;
            }
        }
        delete[] m_ParamValues;
        m_ParamValues = NULL;
    }
}


void FlashPlugin::SetSWFFile(const char * const swffile)
{
    if(m_SWFFile != NULL)
    {
        delete[] m_SWFFile;
    }

    m_SWFFile = faststrdup(swffile);
}

//void FlashPlugin::SetObjectID(const  char * const objectId)
//{
//    bool bhasProperty = gNPN_HasProperty(m_FlashInstance,m_pNPObject,Identifier::get("id"));
//}
