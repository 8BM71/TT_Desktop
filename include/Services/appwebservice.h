#ifndef APPWEBSERVICE_H
#define APPWEBSERVICE_H

#include <QObject>
#include <QtNetworkAuth>
#include <QOAuth2AuthorizationCodeFlow>
#include "httpsinglton.h"
#include <QDebug>

#include "workspacesmodel.h"
#include "projectsmodel.h"
#include "tasksmodel.h"
#include "timeentriesmodel.h"


class GoogleQOAuthHttpServerReplyHandler : public QOAuthHttpServerReplyHandler
{
    Q_OBJECT
public:
    explicit GoogleQOAuthHttpServerReplyHandler(quint16 port, QObject *parent)
        : QOAuthHttpServerReplyHandler(QHostAddress::Any, port, parent)
        , m_idToken("")
    {}

    QString idToken() const
    {
        return m_idToken;
    }

public slots:
    void networkReplyFinished(QNetworkReply *reply) override
    {
        if (reply->error() != QNetworkReply::NoError) {
                qWarning("QOAuthOobReplyHandler::networkReplyFinished: %s",
                         qPrintable(reply->errorString()));
                return;
            }
            if (reply->header(QNetworkRequest::ContentTypeHeader).isNull()) {
                qWarning("QOAuthOobReplyHandler::networkReplyFinished: Empty Content-type header");
                return;
            }
            const QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).isNull() ?
                        QStringLiteral("text/html") :
                        reply->header(QNetworkRequest::ContentTypeHeader).toString();
            const QByteArray data = reply->readAll();
            if (data.isEmpty()) {
                qWarning("QOAuthOobReplyHandler::networkReplyFinished: No received data");
                return;
            }

            Q_EMIT replyDataReceived(data);

            QVariantMap ret;

            if (contentType.startsWith(QStringLiteral("text/html")) ||
                    contentType.startsWith(QStringLiteral("application/x-www-form-urlencoded"))) {
                ret = parseResponse(data);
            } else if (contentType.startsWith(QStringLiteral("application/json"))) {
                const QJsonDocument document = QJsonDocument::fromJson(data);
                if (!document.isObject()) {
                    qWarning("QOAuthOobReplyHandler::networkReplyFinished: Received data is not a JSON"
                             "object: %s", qPrintable(QString::fromUtf8(data)));
                    return;
                }
                const QJsonObject object = document.object();
                if (object.isEmpty()) {
                    qWarning("QOAuthOobReplyHandler::networkReplyFinished: Received empty JSON object: %s",
                             qPrintable(QString::fromUtf8(data)));
                }
                ret = object.toVariantMap();
            } else {
                qWarning("QOAuthOobReplyHandler::networkReplyFinished: Unknown Content-type: %s",
                         qPrintable(contentType));
                return;
            }
//            qDebug() << ret["id_token"].toString();
            m_idToken = ret["id_token"].toString();

            Q_EMIT tokensReceived(ret);
    }

private:
    QVariantMap parseResponse(const QByteArray &response)
    {
        QVariantMap ret;
        QUrlQuery query(QString::fromUtf8(response));
        auto queryItems = query.queryItems(QUrl::FullyDecoded);
        for (auto it = queryItems.begin(), end = queryItems.end(); it != end; ++it)
            ret.insert(it->first, it->second);
        return ret;
    }

    QString m_idToken;
};


class GoogleOQuth2CodeFlow : public QOAuth2AuthorizationCodeFlow
{
    Q_OBJECT
public:
    QString responseType() const override
    {
        return QStringLiteral("code");
    }
};


using namespace Entities;

class AppWebService : public QObject
{
    Q_OBJECT
public:
    explicit AppWebService(QObject *parent = nullptr);

    void createUser(const QString &username, const QString &email, UserPtr user, SuccessCallback successCallback);

    void updateAllEntities(const QString &ownerId,
                           WorkspacesModelPtr workspaceModel,
                           ProjectsModelPtr projectModel,
                           TasksModelPtr taskModel,
                           TimeEntriesModelPtr timeModel,
                           SuccessCallback successCallback);

    void auth();

    void authorizeWithGoogle();

signals:
    void authorizedSuccess(const QString &id, const QString &username, const QString &email);

    void authorizedFailed(const QString &errorString);


private:
    GoogleOQuth2CodeFlow m_google;
    QString m_idToken;
};

#endif // APPWEBSERVICE_H
