////////////////////////////////////////////////////////
//  File name:      FlashContentLoader.cpp
//  Author: wyx   ID：      Version:       Date: 2013.9.18
//  Description: get and post Flash Content from the Internet
////////////////////////////////////////////////////////

#include <QString>
#include <QApplication>
#include <qtextstream.h>
#include "FlashContentLoader.h"
#include "faststrdup.h"
#include "faststrdup.h"
#include <sys/syscall.h>
#include <QList>
#include <QNetworkProxy>
#include <time.h>

//#define DEBUG

using namespace std;

QNetworkProxy FlashContentLoader::m_NetWorkProxy(QNetworkProxy::HttpProxy,QString("10.66.76.245"),9876);
QNetworkAccessManager FlashContentLoader::m_NetWorkManager;
bool NetWorkManagerInit = false;


const char * DefaultMimeType = "text/plain";

FlashContentLoader::FlashContentLoader(QString url, NPP instance,
         NPPluginFuncs pluginfuncs, void * notify, QObject *parent) :
    QObject(parent)
{
#ifdef DEBUG
    printf("new loader url:%s\n",url.toLatin1().data());
#endif

    if(NetWorkManagerInit == false)
    {
        //set proxy using QNetworkProxy
//        FlashContentLoader::m_NetWorkManager.setProxy(FlashContentLoader::m_NetWorkProxy);
        NetWorkManagerInit = true;
    }

    m_NPP = instance;
    m_NPP_Pluginfuncs = pluginfuncs;
    m_URL = url;
    m_URLBackup = url;
    m_Notify = notify;
    m_Reload = false;
    m_Renotify = false;
    m_HttpError = false;
    m_MIMEType = const_cast<char *>(DefaultMimeType);

    TotalDataBytes = 0;
}

FlashContentLoader::~FlashContentLoader()
{
        printf("enter ~\n");
}

void FlashContentLoader::Request()
{
    QNetworkRequest Req;
    Req.setUrl(QUrl(m_URL,QUrl::StrictMode));

    Req.setAttribute(QNetworkRequest::CacheLoadControlAttribute,QVariant((int)QNetworkRequest::AlwaysNetwork));
    Req.setAttribute(QNetworkRequest::CacheSaveControlAttribute,QVariant(false));

    m_pNetWorkReply = m_NetWorkManager.get(Req);

    //Request Connect Signal
    connect(m_pNetWorkReply,SIGNAL(finished()),this,SLOT(httpfinished()));
    connect(m_pNetWorkReply,SIGNAL(readyRead()),this,SLOT(httpreadyread()));
    connect(m_pNetWorkReply,SIGNAL(metaDataChanged()),this,SLOT(httpmetadatachanged()));
    connect(m_pNetWorkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(httperror(QNetworkReply::NetworkError)));
#ifndef QT_NO_OPENSSL
    connect(m_pNetWorkReply,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(httpsslErrors(QList<QSslError>)));
#endif
}

QNetworkRequest::KnownHeaders getHeader(QByteArray HeaderKey)
{
    if(HeaderKey == QByteArray("Content-Type"))
    {
        return QNetworkRequest::ContentTypeHeader;
    }
    if(HeaderKey == QByteArray("Content-Length"))
    {
        return QNetworkRequest::ContentLengthHeader;
    }
    if(HeaderKey == QByteArray("Referer"))
    {
        return QNetworkRequest::LocationHeader;
    }

    printf("[WARNING] HeaderKey:%s!\n",HeaderKey.data());
    return QNetworkRequest::ContentTypeHeader;
}

void FlashContentLoader::Post(QByteArray postData)
{
    int dataLen = postData.length();

    int idex = postData.indexOf("\r\n\r\n",0);

#ifdef DEBUG
    printf("idex:%d\n",idex);
#endif

    QList<QByteArray> qlHeader;
    if(-1 == idex)
    {
        printf("no header content\n");
    }
    else
        qlHeader = postData.left(idex).split('\r\n');

    QNetworkRequest * m_pNetWorkRequest = new QNetworkRequest(QUrl(m_URL));

    for(QList<QByteArray>::iterator it1 = qlHeader.begin();it1 != qlHeader.end(); it1++)
    {
        int ind = it1->indexOf(":",0);
        QByteArray HeaderKey = it1->left(ind);
        QByteArray HeaderValue = it1->right(it1->length() - ind - 1).trimmed();

#ifdef DEBUG
        printf("%s: %s\n",HeaderKey.data(),HeaderValue.data());
#endif

        m_pNetWorkRequest->setRawHeader(HeaderKey,HeaderValue);
    }

    int bodylen = 0;
    if(-1 != idex)
    {
        bodylen = dataLen - idex - 4;
    }
    else
        bodylen = dataLen - 2;

    QByteArray byteBodyArray = postData.right(bodylen);

#ifdef DEBUG
    printf("bodyData:%s\n",byteBodyArray.data());
#endif

    m_pNetWorkReply = m_NetWorkManager.post(*m_pNetWorkRequest,byteBodyArray);

    connect(m_pNetWorkReply,SIGNAL(finished()),this,SLOT(httpfinished()));
    connect(m_pNetWorkReply,SIGNAL(readyRead()),this,SLOT(httpreadyread()));
    connect(m_pNetWorkReply,SIGNAL(metaDataChanged()),this,SLOT(httpmetadatachanged()));
    connect(m_pNetWorkReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(httperror(QNetworkReply::NetworkError)));
#ifndef QT_NO_OPENSSL
    connect(m_pNetWorkReply,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(httpsslErrors(QList<QSslError>)));
#endif
}


void FlashContentLoader::httpfinished()
{
#ifdef DEBUG
    printf("httpfinished() url:%s\n",m_URL.toLatin1().data());
#endif

    //if error occur,just ignore
    if(m_HttpError)
    {
        return;
    }

    //if reload, call request and return
    if(m_Reload)
    {
        Request();
        m_Reload = false;
        m_Renotify = true;
        return;
    }

    m_NPP_Pluginfuncs.destroystream(m_NPP,&m_NP_Stream,NPRES_DONE);

    //if m_Notify is not null, notify the plugin
    if(m_Notify != 0)
    {
        //if reloaded, notify with the old url
        if(m_Renotify)
        {
#ifdef DEBUG
            printf("urlnotify renotify:%s notify:0x%x\n",m_URLBackup.toLatin1().data(),m_Notify);
#endif
            m_NPP_Pluginfuncs.urlnotify(m_NPP,m_URLBackup.toLatin1().data(),NPRES_DONE,m_Notify);
        }
        else//if is not reloaded
        {
#ifdef DEBUG
            printf("urlnotify:%s notify:0x%x\n",m_URL.toLatin1().data(),m_Notify);
#endif
            m_NPP_Pluginfuncs.urlnotify(m_NPP,m_URL.toLatin1().data(),NPRES_DONE,m_Notify);
        }

    }

    m_NP_Stream.ndata 	= 0;

    TotalDataBytes = 0;

    m_pNetWorkReply->close();
    m_pNetWorkReply->deleteLater();
    m_pNetWorkReply = NULL;
}

void FlashContentLoader::httpreadyread()
{
    if(m_Reload)
    {
        return;
    }

    qint64 bytesAvai = m_pNetWorkReply->bytesAvailable();
    TotalDataBytes += bytesAvai;

    QByteArray qa = m_pNetWorkReply->readAll();
    int bytesReady = m_NPP_Pluginfuncs.writeready(m_NPP,&m_NP_Stream);
    int WriteNum = m_NPP_Pluginfuncs.write(m_NPP,&m_NP_Stream,0,bytesAvai,qa.data());

#ifdef DEBUG
    printf("httpreadyread url:%s\n",m_URL.toLatin1().data());
    printf("bytes:%lld total:%lld\n",bytesAvai,TotalDataBytes);
    printf("writeready bytes:%d WriteNum:%d\n",bytesReady,WriteNum);

    QVariant statusCodeV = m_pNetWorkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCodeV.toInt() == 403)
    {
        printf("data: %s\n",qa.data());
    }
#endif
}

uint getTimetFromGMT(std::string GMTTime)
{
    int p1 = GMTTime.find(' ');
    std::string subStr = GMTTime.substr(p1 + 1);
    struct tm tm1;
    strptime(subStr.c_str(), "%d %b %Y %H:%M:%S GMT", &tm1);
//    printf("===== subStr:%s year:%d month:%d day:%d hour:%d minute:%d seconds:%d\n",
//           subStr.c_str(),tm1.tm_year,tm1.tm_mon,tm1.tm_mday,tm1.tm_hour,tm1.tm_min,tm1.tm_sec);
    time_t t1 = mktime(&tm1);
    return static_cast<uint>(t1);
}

void FlashContentLoader::httpmetadatachanged()
{
#ifdef DEBUG
    printf("httpmetadatachanged url:%s\n",m_URL.toLatin1().data());
    QVariant statusCodeV = m_pNetWorkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    printf("statusCode when metadatachanged:%d\n",statusCodeV.toInt());
    if(statusCodeV.toInt() == 403)
    {
        printf("%s\n",m_pNetWorkReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString().toLatin1().data());
    }
#endif

    QString lheaders = "HTTP 200 OK\n";

    uint32_t lastModified = 0;
    uint32_t uEnd = 0;

    QList<QNetworkReply::RawHeaderPair> Headers = m_pNetWorkReply->rawHeaderPairs();
    for(int i = 0; i < Headers.size(); i++)
    {
        lheaders.append(Headers.at(i).first.data());
        lheaders.append(": ");
        lheaders.append(Headers.at(i).second.data());
        lheaders.append("\n");

#ifdef DEBUG
        printf("head:%s value:%s\n",Headers.at(i).first.data(),Headers.at(i).second.data());
#endif
        //gain content-type
        if(Headers.at(i).first == QByteArray("Content-Type"))
        {
            //if ; exit
            if(Headers.at(i).second.contains(';'))
            {
                QList<QByteArray> balist = Headers.at(i).second.split(';');
                m_MIMEType = faststrdup(balist.first().data());
            }
            else
            {
                m_MIMEType = faststrdup(Headers.at(i).second.data());
            }

        }
        //judge if it is necessary to redirect,if it is necessary to redirect
        else if(Headers.at(i).first == QByteArray("Location"))
        {
            m_URLBackup = m_URL;//store the old url

            m_URL = m_pNetWorkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

            m_Reload = true;
        }
        else if(Headers.at(i).first == QByteArray("Last-Modified"))
        {
            std::string strTime(Headers.at(i).second.data());
            lastModified = getTimetFromGMT(strTime);
        }
        else if(Headers.at(i).first == QByteArray("Content-Length"))
        {
            uEnd = Headers.at(i).second.toUInt();
        }

    }

    if(m_Reload)
    {
        return;
    }

    m_Headers = faststrdup(lheaders.toLatin1().data());

    m_NP_Stream.url=faststrdup(m_URL.toLatin1().data());//meaningless
    m_NP_Stream.ndata 	= 0;
    m_NP_Stream.end 	= uEnd;//not known the length
    m_NP_Stream.lastmodified= lastModified;//need to fix
    m_NP_Stream.notifyData  = m_Notify;
    m_NP_Stream.headers = m_Headers;
    m_TranslateType = NP_NORMAL;

    m_NPP_Pluginfuncs.newstream(m_NPP,(NPMIMEType)(m_MIMEType),&m_NP_Stream,0,&m_TranslateType);
}

void FlashContentLoader::httperror(QNetworkReply::NetworkError code)
{
    m_HttpError = true;
    printf("[ERROR] NetworkError!!! code:%d url:%s\n",code,m_URL.toLatin1().data());
    printf("[ERROR] %s\n",m_pNetWorkReply->errorString().data());
}

void FlashContentLoader::httpsslErrors(const QList<QSslError> &errors)
{
    printf("[ERROR] httpsslerrors\n");
}
