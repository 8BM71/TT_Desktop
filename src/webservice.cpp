#include "webservice.h"
#include <QTimer>
#include <QDebug>
#include <memory>

WebService::WebService(QObject *parent) : QObject(parent)
{

}

void WebService::getRequest(const QNetworkRequest &request)
{
    auto reply = m_manager.get(request);
    requestFunction(reply);
}

void WebService::postRequest(const QNetworkRequest &request, const QByteArray &data)
{
    auto reply = m_manager.post(request, data);
    requestFunction(reply);
}

void WebService::requestFunction(QNetworkReply *reply)
{
    if (reply == nullptr)
    {
        //TODO: handle error
    }

    std::shared_ptr<QTimer> timer = std::make_shared<QTimer>;

    timer->setInterval(m_waitTime);
    timer->setSingleShot(true);

    connect(timer.get(), &QTimer::timeout, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::readyRead, timer.get(), &QTimer::stop);

    timer->start();

    connect(reply, &QNetworkReply::finished, [this, reply, timer] (){
        timer->stop();

        if (reply == nullptr)
        {
            //TODO: handle error
        }

        if (reply->error() != QNetworkReply::NoError)
        {
            //TODO: error
        }
        else
        {

        }

        reply->deleteLater();


    });

}














