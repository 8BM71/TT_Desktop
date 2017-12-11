#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class WebService : public QObject
{
    Q_OBJECT
public:
    explicit WebService(QObject *parent = nullptr);

    void getAllWorkspaces();

    void getWorkspaceById();

    void getAllProjects();

    void getProjectById();

    void getAllTasks();

    void getTaskById();

    void getTimeEntries();

    void createWorkspace();

    void createProject();

    void createTask();

    void createTimeEntry();

signals:

public slots:


private:
    void getRequest(const QNetworkRequest& request);

    void postRequest(const QNetworkRequest& request, const QByteArray& data);

    void requestFunction(QNetworkReply * reply);

private:
    QNetworkAccessManager m_manager;
    QString m_host;
    int m_waitTime;
};

#endif // WEBSERVICE_H
