////////////////////////////////////////////////////////
//  File name:      Container.cpp
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: Container to play flash instance
////////////////////////////////////////////////////////


#include <QApplication>
#include "Container.h"
#include "FlashPlugin.h"
#include "faststrdup.h"
#include "Identifier.h"
#include "npn.h"

#include <map>
#include <stdio.h>
#include <string>
#include <pthread.h>
#include <sys/syscall.h>
#include <assert.h>

//#define DEBUG

//the base index of flash instance id
const int FLASH_INDEX_BASE = 2000;

std::string Container::GetAbsURL(std::string RelativeURL)
{
    if(RelativeURL.substr(0,4) == "http")
    {
        return RelativeURL;
    }

    if(RelativeURL.length() > 2 && RelativeURL.substr(0,2) == "//")
    {
        return "http:" + RelativeURL;
    }

    if("" == ContainerURL)
    {
        return RelativeURL;
    }

    int keypos = ContainerURL.find_last_of('/');
    std::string domainName = ContainerURL.substr(0,keypos);

    if(RelativeURL.length() > 2 && RelativeURL.substr(0,2) == "./")
    {
        return domainName + RelativeURL.substr(1);
    }

    return domainName + "/" + RelativeURL;

//    QUrl AbsUrl = QUrl(QString(ContainerURL.c_str())).resolved(QUrl(QString(RelativeURL.c_str())));
//    printf("ABSURL: %s\n",AbsUrl.toString().toLatin1().data());
//    return std::string(AbsUrl.toString().toLatin1().data());
}

Container::Container(QWidget *parent) :
    QWidget(parent)
{
//    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    InitContainer();
}

Container::~Container()
{
    pthread_mutex_destroy(&flashInstanceMutex);
    FlashPlugin::UninitFlashPlugin();
}



void Container::InitContainer()
{
    FlashPlugin::InitFlashPlugin();

    NextFlashIndex = FLASH_INDEX_BASE;

    flashInstancesMap.clear();
    pthread_mutex_init(&flashInstanceMutex,NULL);

    FlashMaskRegion = QRegion(0,0,1,1);
    FlashInstancesRect.clear();
    setMask(FlashMaskRegion);
}

void Container::Show(const bool cmd)
{
    if(cmd)
    {
        this->show();
    }
    else
        this->hide();
}

void Container::SetSize(const int width, const int height)
{
    this->resize(width,height);
}

void Container::Move(const int x, const int y)
{
    this->move(x,y);
}

void Container::DestroyContainer()
{
    this->destroy();
    qApp->exit();
}

void Container::CreateFlashInstance(int * pFlashId)
{
    pthread_mutex_lock(&flashInstanceMutex);
    flashInstancesMap.insert(std::make_pair(++NextFlashIndex,(FlashPlugin *)0));
    pthread_mutex_unlock(&flashInstanceMutex);

    * pFlashId = NextFlashIndex;

#ifdef DEBUG
    printf("CreateFlashInstance:FlashId = %d\n",NextFlashIndex);
#endif
}

void Container::DestroyFlashInstance(const int FlashId, bool * pResult)
{
    QRect rect = FlashInstancesRect[FlashId];
    FlashMaskRegion -= rect;
    setMask(FlashMaskRegion);

    pthread_mutex_lock(&flashInstanceMutex);

    std::map<int,FlashPlugin *>::iterator it1 = flashInstancesMap.find(FlashId);
    if(it1 == flashInstancesMap.end())
    {
        printf("[ERROR] No flash instance in container FlashId: %d\n",FlashId);
        * pResult = false;
        pthread_mutex_unlock(&flashInstanceMutex);
        return;
    }

    FlashPlugin * finstance = it1->second;
    delete finstance;
    flashInstancesMap.erase(it1);

    * pResult = true;

    pthread_mutex_unlock(&flashInstanceMutex);
}

void Container::Flash_Play(const int FlashId, int * pResult)
{
    FlashPlugin * finstance = flashInstancesMap[FlashId];
    finstance->Play();
    * pResult = 0;
}


void Container::Flash_put_Movie(const int FlashId, const std::string & Value, int * pResult)
{
    FlashPlugin * finstance = flashInstancesMap[FlashId];
    finstance->SetSWFFile(Value.c_str());
    * pResult = 0;
}


void Container::Property_Load(
        std::map<std::string,std::string> * pPropertyBagMap, const int FlashId)
{
    QVector<QString> paramnames;
    QVector<QString> paramvalues;
    char * src = NULL;

    std::map<std::string,std::string>::iterator itt;

    for(itt = pPropertyBagMap->begin();itt != pPropertyBagMap->end();itt++)
    {
#ifdef DEBUG
        printf("+++ %s:%s\n",itt->first.c_str(),itt->second.c_str());
#endif

        std::string tKey = itt->first;
        std::string tVal = itt->second;

        if(tKey == "Src" || tKey == "Movie" || tKey == "src" || tKey == "movie")
        {
            if(tVal.empty())
            {
                printf("[ERROR] src is empty.\n");
            }
            else
            {
                std::string AbsURL = GetAbsURL(tVal);
                tVal = AbsURL;

#ifdef DEBUG
                printf("AbsURL:%s\n",AbsURL.c_str());
#endif

                src = faststrdup(AbsURL.c_str());
            }
        }

        paramnames.push_back(QString(tKey.c_str()));
        paramvalues.push_back(QString(tVal.c_str()));

    }

    delete pPropertyBagMap;

    if(NULL == src)
    {
        printf("[ERROR] src == NULL!\n");
    }


    FlashPlugin * flash2 = new FlashPlugin(src,this,0,0,0,0,paramnames,paramvalues);
    flashInstancesMap[FlashId] = flash2;
    flash2->m_FlashId = FlashId;

    flash2->ReadyState = 4;

    flash2->Play();

    this->show();

    flash2->SetVisible(true);

    if(src != NULL)
    {
        delete[] src;
        src = NULL;
    }
}

void Container::Flash_get_ReadyState(int * pResult, std::map<std::string,int> & KeyValueMap, const int FlashId)
{
    FlashPlugin * pFlash = flashInstancesMap[FlashId];//to do: not find

    KeyValueMap.insert(std::make_pair("Val",pFlash->ReadyState));

    * pResult = 0;
}

void Container::SetObjectRects(int * pos, int * clip, const int FlashId)
{
     FlashPlugin * pFlash = flashInstancesMap[FlashId];//to do: not find

     if(NULL == pos || NULL == clip)
     {
         printf("[ERROR] pos == NULL || clip == NULL!\n");

         if(NULL != pos)
         {
             delete[] pos;
             pos = NULL;
         }

         if(NULL != clip)
         {
             delete[] clip;
             clip = NULL;
         }

         return;
     }

     // to avoid the bug in flash plugin and qt
     if(pos[0] == pos[2] && pos[1] == pos[3])
     {
         pos[2]++;
         pos[3]++;
     }
     if(clip[0] + clip[1] + clip[2] + clip[3] == 0)
     {
         clip[0] = pos[0];
         clip[1] = pos[1];
         clip[2] = pos[2];
         clip[3] = pos[3];
     }

     QRect QRectpos(pos[0],pos[1],pos[2]-pos[0],pos[3]-pos[1]);
     QRect QRectclip(clip[0],clip[1],clip[2]-clip[0],clip[3]-clip[1]);
     QRect QNPWindowpos(pFlash->m_pNPWindow->x,pFlash->m_pNPWindow->y,pFlash->m_pNPWindow->width,pFlash->m_pNPWindow->height);
     QRect QNPWindowclip(pFlash->m_pNPWindow->clipRect.left,pFlash->m_pNPWindow->clipRect.top,
                         pFlash->m_pNPWindow->clipRect.right,pFlash->m_pNPWindow->clipRect.bottom);
     if(QRectpos != QNPWindowpos || QRectclip != QNPWindowclip)
     {
         pFlash->m_pContainer->setGeometry(pos[0],pos[1],pos[2]-pos[0],pos[3]-pos[1]);

         pFlash->m_pNPWindow->x = pos[0];
         pFlash->m_pNPWindow->y = pos[1];
         pFlash->m_pNPWindow->width = pos[2]-pos[0];
         pFlash->m_pNPWindow->height = pos[3]-pos[1];

         pFlash->m_pNPWindow->clipRect.left = clip[0];
         pFlash->m_pNPWindow->clipRect.top = clip[1];
         pFlash->m_pNPWindow->clipRect.right = clip[2];
         pFlash->m_pNPWindow->clipRect.bottom = clip[3];


         NPError npres = pFlash->m_NPP_PluginFuncs.setwindow(pFlash->m_FlashInstance,pFlash->m_pNPWindow);

         FlashMaskRegion -= FlashInstancesRect[FlashId];
         FlashInstancesRect[FlashId] = QRectclip;
         FlashMaskRegion += FlashInstancesRect[FlashId];
         setMask(FlashMaskRegion);
     }

     delete[] pos;
     delete[] clip;
}

void Container::ClipContainerWindow(std::vector<QRect> * pRects)
{
    QRegion FlashMaskRegion = QRegion(0,0,1,1);

    std::map<int,QRect>::iterator it1 = FlashInstancesRect.begin();
    for(;it1 != FlashInstancesRect.end();it1++)
    {
        FlashMaskRegion += it1->second;
    }

    for(int i = 0; i < pRects->size(); i++)
    {
        FlashMaskRegion -= (*pRects)[i];
    }

    setMask(FlashMaskRegion);
    delete pRects;
    pRects = NULL;
}


void Container::SetContainerURL(const std::string & url)
{
    ContainerURL = url;
}

FlashPlugin * Container::GetFlashPointer(const int FlashId)
{
    pthread_mutex_lock(&flashInstanceMutex);
    std::map<int, FlashPlugin *>::iterator it =  flashInstancesMap.find(FlashId);
    pthread_mutex_unlock(&flashInstanceMutex);

    if(it != flashInstancesMap.end())
    {
        return it->second;
    }
    else
        return NULL;
}


void Container::NP_HasMethod(const int FlashId, const  char * methodName, bool * bRes)
{
    FlashPlugin * pFlash = flashInstancesMap[FlashId];
    assert(pFlash);

#ifdef DEBUG
    printf("call Container::NP_hasMethod methodName:%s\n",methodName);
#endif

    * bRes = pFlash->NP_hasMethod(methodName);
}

void Container::NP_Invoke(const int FlashId, char * methodName,
                              void * pArgsVec, const int argCount, void * pResult)
{
    FlashPlugin * pFlash = flashInstancesMap[FlashId];
    assert(pFlash);

    bool res =  pFlash->NP_invoke(Identifier::get(methodName),
          reinterpret_cast<NPVariant *>(pArgsVec),argCount,reinterpret_cast<NPVariant *>(pResult));

#ifdef DEBUG
    printf("call Container::Script_Invoke methodName:%s result:%d\n",methodName,res);
#endif
}

void Container::NP_HasProperty(const int FlashId, const char * propertyName, bool * bRes)
{
    FlashPlugin * pFlash = flashInstancesMap[FlashId];
    assert(pFlash);

#ifdef DEBUG
    printf("call Container::NP_hasMethod propertyName:%s\n",propertyName);
#endif

    * bRes = pFlash->NP_hasProperty(propertyName);
}

void Container::NP_GetProperty(const int FlashId, const char * propertyName, NPVariant & Result)
{
    return;
}

bool Container::NP_SetProperty(const int FlashId, const char * propertyName, const NPVariant & Value)
{
    return false;
}

bool Container::NP_RemoveProperty(const int FlashId, const char * propertyName)
{
    return false;
}
