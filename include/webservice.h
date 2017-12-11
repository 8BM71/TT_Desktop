#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <functional>
#include <memory>

struct Response
{
    bool isError = false;
    QString errorString = "";
    int statusCode = 0;
    QByteArray data;
    QMap<QString, QString> headerData;
};

typedef std::shared_ptr<Response> ResponsePtr;

typedef std::function<void(ResponsePtr)> PerformCallback;

class WebService : public QObject
{
    Q_OBJECT
public:
    explicit WebService(QObject *parent = nullptr);

//    void getAllWorkspaces();

//    void getWorkspaceById();

//    void getAllProjects();

//    void getProjectById();

//    void getAllTasks();

//    void getTaskById();

//    void getTimeEntries();

//    void createWorkspace();

//    void createProject();

//    void createTask();

//    void createTimeEntry();

    //TODO: authorization

signals:

public slots:


private:
    void getRequest(const QNetworkRequest& request, PerformCallback callback);

    void postRequest(const QNetworkRequest& request, const QByteArray& data, PerformCallback callback);

    void requestFunction(QNetworkReply * reply, PerformCallback callback);

private:
    QNetworkAccessManager m_manager;
    QString m_host;
    int m_waitTime;
};

#endif // WEBSERVICE_H
