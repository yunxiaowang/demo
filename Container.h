////////////////////////////////////////////////////////
//  File name:      Container.h
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: Container to play flash instance
////////////////////////////////////////////////////////

#ifndef CONTAINER_H
#define CONTAINER_H

#include "FlashPlugin.h"

#include <QWidget>
#include <map>


class Container : public QWidget
{
    Q_OBJECT
    
public:
    explicit Container(QWidget *parent = 0);
    ~Container();

    //url contain the flash
    std::string ContainerURL;

    FlashPlugin * GetFlashPointer(const int FlashId);

public slots:
    void Show(const bool cmd);
    void SetSize(const int width, const int height);
    void Move(const int x, const int y);
    void DestroyContainer();
    void CreateFlashInstance(int * pResult);
    void DestroyFlashInstance(const int FlashId, bool * pResult);
    void Flash_Play(const int FlashId, int * pResult);
    void Flash_put_Movie(const int FlashId, const std::string& Value, int * pResult);
    void Property_Load(std::map<std::string,std::string> * pPropertyBagMap, const int FlashId);
    void Flash_get_ReadyState(int * pResult, std::map<std::string,int> & KeyValueMap, const int FlashId);
    void SetObjectRects(int * pos, int * clip, const int FlashId);
    void ClipContainerWindow(std::vector<QRect> * pRects);
    void SetContainerURL(const std::string & url);

    void NP_HasMethod(const int FlashId, const  char * methodName, bool * bres);
    void NP_Invoke(const int FlashId, char * methodName,
                       void * pArgsVec, const int argCount, void * pResult);
    void NP_HasProperty(const int FlashId, const char * propertyName, bool * bres);
    void NP_GetProperty(const int FlashId, const char * propertyName, NPVariant & Result);
    bool NP_SetProperty(const int FlashId, const char * propertyName, const NPVariant & Value);
    bool NP_RemoveProperty(const int FlashId, const char * propertyName);

private:
    //next flash index to allocate
    int NextFlashIndex;

    //Flash index -> FlashPlugin *
    std::map<int, FlashPlugin *> flashInstancesMap;

    //mutex to synchronize operatations to map
    pthread_mutex_t flashInstanceMutex;

    //regions to mask in container
    QRegion FlashMaskRegion;

    //flash instances' regions
    std::map<int,QRect> FlashInstancesRect;

    //initialize container
    void InitContainer();

    //get absolute url from relative url
    std::string GetAbsURL(std::string RelativeURL);
};

#endif // CONTAINER_H
