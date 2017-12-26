#include "httpsinglton.h"
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>

HttpSinglton::HttpSinglton(QObject *parent)
    : QObject(parent)
    , m_waitTime(30 * 1000)
{
    m_host = "https://tttpu.tk";
}

HttpSinglton::~HttpSinglton()
{
}

HttpSinglton *HttpSinglton::instance()
{
    static HttpSinglton instance;
    return &instance;
}

void HttpSinglton::getRequest(const QNetworkRequest &request, PerformCallback callback)
{
    auto reply = m_manager.get(request);
    requestFunction(reply, callback);
}

void HttpSinglton::postRequest(const QString &query, PerformCallback callback, QJsonValue vars)
{
    QString requestUrl(m_host + "/graphql");

    QJsonObject requestObject
    {
        {"query", query},
        {"variables", vars}
    };

    QJsonDocument doc(requestObject);

    QNetworkRequest request;
    request.setUrl(requestUrl);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");


    auto reply = m_manager.post(request, doc.toJson());
    requestFunction(reply, callback);
}

void HttpSinglton::requestFunction(QNetworkReply *reply, PerformCallback callback)
{
    if (reply == nullptr)
    {
        ResponsePtr resp = std::make_shared<Response>();
        resp->isError = true;
        resp->errorString = "Reply is nullptr";
        callback(resp);
        return;
    }

    std::shared_ptr<QTimer> timer = std::make_shared<QTimer>();

    timer->setInterval(m_waitTime);
    timer->setSingleShot(true);

    connect(timer.get(), &QTimer::timeout, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::readyRead, timer.get(), &QTimer::stop);

    timer->start();

    connect(reply, &QNetworkReply::finished, [this, reply, timer, callback] (){
        timer->stop();

        ResponsePtr response = std::make_shared<Response>();

        if (reply == nullptr)
        {
            response->isError = true;
            response->errorString = "Reply is nullptr";
            callback(response);
            return;
        }

        if (reply->error() != QNetworkReply::NoError)
        {
            response->isError = true;
            response->errorString = reply->errorString();
        }
        else
        {
            response->statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            for (auto it : reply->rawHeaderPairs())
            {
                QString name(it.first);
                QString data(it.second);
                response->headerData.insert(name, data);
            }
            response->data = reply->readAll();
        }
        callback(response);
        reply->deleteLater();
    });
}
