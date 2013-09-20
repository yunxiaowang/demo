////////////////////////////////////////////////////////
//  File name:      FlashContentLoader.h
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: get and post Flash Content from the Internet
////////////////////////////////////////////////////////

#ifndef FLASHCONTENTLOADER_H
#define FLASHCONTENTLOADER_H

/*
 *because we work on the unix platform,so we must define XP_UNIX
*/
#define XP_UNIX
/*
 *THE REASON AS XP_UNIX
*/
#define MOZ_X11

#include <QString>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "npapi.h"
#include "npfunctions.h"


class FlashContentLoader : public QObject
{
    Q_OBJECT
public:
    explicit FlashContentLoader(QString url, NPP instance, NPPluginFuncs pluginfuncs,
                                void * notify = 0, QObject * parent = 0);
    ~FlashContentLoader();

    static QNetworkAccessManager m_NetWorkManager;

    static QNetworkProxy m_NetWorkProxy; //proxy

    void Request();

    void Post(QByteArray postData);

public slots:
    void httpfinished();
    void httpreadyread();
    void httpmetadatachanged();
    void httperror(QNetworkReply::NetworkError code);
    void httpsslErrors( const QList<QSslError> & errors);

private:
    QNetworkReply * m_pNetWorkReply;
    NPP  m_NPP;
    NPStream m_NP_Stream;
    NPPluginFuncs m_NPP_Pluginfuncs;
    QString m_URL;
    QString m_URLBackup;
    void * m_Notify;
    char * m_MIMEType;
    uint16_t m_TranslateType;
    bool m_Reload;
    bool m_Renotify;
    bool m_HttpError;
    char * m_Headers;

    qint64 TotalDataBytes;//the total data bytes loaded
};

#endif // FLASHCONTENTLOADER_H
