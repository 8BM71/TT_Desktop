#ifndef HTTPSINGLTON_H
#define HTTPSINGLTON_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <functional>
#include <memory>
#include <QJsonValue>

struct Response
{
    bool isError = false;
    QString errorString = "";
    int statusCode = 0;
    QByteArray data;
    QMap<QString, QString> headerData;
};

typedef std::shared_ptr<Response> ResponsePtr;

typedef std::function<void (ResponsePtr)> PerformCallback;
typedef std::function<void (bool, QString)> SuccessCallback;

class HttpSinglton : public QObject
{
    Q_OBJECT
    explicit HttpSinglton(QObject *parent = nullptr);
    ~HttpSinglton();
    HttpSinglton(const HttpSinglton&) = delete;
    HttpSinglton& operator=( HttpSinglton& ) = delete;
public:
    static HttpSinglton* instance();

    void getRequest(const QNetworkRequest& request, PerformCallback callback);

    void postRequest(const QString &query, PerformCallback callback, QJsonValue vars = QJsonValue());

private:
     void requestFunction(QNetworkReply * reply, PerformCallback callback);

private:
    QNetworkAccessManager m_manager;
    QString m_host;
    int m_waitTime;
};

#endif // HTTPSINGLTON_H
