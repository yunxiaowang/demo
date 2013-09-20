////////////////////////////////////////////////////////
//  File name:      FlashPlugin.h
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: Flash Plugin wrapped class
////////////////////////////////////////////////////////

#ifndef FLASHPLUGIN_H
#define FLASHPLUGIN_H

/*
 *because we work on the unix platform,so we must define XP_UNIX
*/
#define XP_UNIX
/*
 *THE REASON AS XP_UNIX
*/
#define MOZ_X11

#include <QString>
#include <QLibrary>
#include <QX11EmbedContainer>
#include <QWidget>
#include <QX11Info>
#include <QNetworkRequest>

#include "npapi.h"
#include "npfunctions.h"

#include <pthread.h>
#include <semaphore.h>

class NPFunctionsStruct
{
public:
    NP_InitializeFunc m_NP_Initialize;
    NP_ShutdownFunc m_NP_Shutdown;
    NP_GetMIMEDescriptionFunc m_NP_GetMIMEDescription;
    NP_GetValueFunc m_NP_GetValue;
};



class FlashPlugin
{
public:
        //make flash instance scriptable
        NPObject * m_pNPObject;

        // flash instance private data and container private data
        NPP m_FlashInstance;

        //NPP functions struct
        static NPPluginFuncs m_NPP_PluginFuncs;

        //map from NPP to FlashPlugin *
        static std::map<NPP, FlashPlugin *> NPPtoFlashPlugin;
        static FlashPlugin * getFlashPluginFromNPP(NPP npp);

        std::string objectId;

        int m_FlashId;

        long ReadyState;

        NPWindow * m_pNPWindow; //the actual size of flash
        NPSetWindowCallbackStruct * m_pWinCallbackStruct; //contains information about plugin's unix window environment
        QX11EmbedContainer * m_pContainer; // the show window size of flash

        FlashPlugin(const char * swffile = NULL, QWidget * parent = NULL, int x = 0, int y = 0,
                    int width = 0,int height = 0,QVector<QString> paramnames = QVector<QString>(),
                    QVector<QString> paramvalues = QVector<QString>());

        ~FlashPlugin();

        static bool InitFlashPlugin();
        static bool UninitFlashPlugin();

        static int GetFlashInstanceNumber();

        void Play(); //play flash
        void SetVisible(bool isVisible); //set flash to be visible if isVisible is true
        void SetSWFFile(const char * const swffile); //set swf file

        void NPWindowConstruct(int x, int y, int width, int height, int clipx, int clipy, int clipwidth, int clipheight);

//        void SetObjectID(const  char * const objectId);

        bool NP_hasMethod(const char * methodName);
        bool NP_hasMethod(NPIdentifier nameId);
        bool NP_invoke(NPIdentifier name, const NPVariant *args,
                           uint32_t argCount, NPVariant *result);
        bool NP_hasProperty(const char * propertyName);
        bool NP_hasProperty(NPIdentifier name);
        bool NP_getProperty(const char * propertyName, NPVariant * result);
        bool NP_getProperty(NPIdentifier name, NPVariant * result);
        bool NP_setProperty(NPIdentifier name, const NPVariant *value);
        bool NP_removeProperty(NPIdentifier name);

private:
        QWidget * m_pParentWidget;// the container window

//        NPSavedData * m_pSaveddata; // saved data

        char * m_SWFFile;// SWF file path or url
        char ** m_ParamNames;//parameter names
        char ** m_ParamValues;// parameter values
        int m_ParamCount;// parameter count

        static char * m_MIMEDescription;//mime description
        static char * m_PluginName;//plugin name
        static char * m_PluginDescription;//plugin description
        static QLibrary * m_LibraryHandle;//Flash player plugin loading handler
        static NPFunctionsStruct  m_NP_Func;//NP functions struct

        static int m_FlashInstanceNumber;//flash instance number

        static NPNetscapeFuncs m_NPN_funcs;//NPN functions struct

        static bool LoadFlashPlayerSo();
        static bool LoadNPFunctions();
        static bool GetPluginInfo();


        void SetParameters(const QVector<QString> paramnames,const QVector<QString> paramvalues);

        void SendNPStream(char * argv);

        void GetNPObject();
};


#endif // FLASHPLUGIN_H
