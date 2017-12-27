#include "timeentrywebservice.h"
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

TimeEntryWebService::TimeEntryWebService(QObject *parent) : QObject(parent)
{

}

void TimeEntryWebService::getAllTimeEntries(const QString &ownerId, TimeEntriesModelPtr timeModel, SuccessCallback successCallback)
{
    QString query = QString("{"
                                "workspaces(ownerId: \"%1\") {"
                                    "projects { "
                                        "tasks {"
                                            "id timeEntries {"
                                                "id duration endDate startDate}"
                                            "}"
                                        "}"
                                    "}"
                            "}").arg(ownerId);

    HttpSinglton::instance()->postRequest(query, [this, timeModel, successCallback](ResponsePtr resp) {
        if (resp->isError)
        {
            successCallback(false, resp->errorString);
        }
        else
        {
            if (resp->statusCode == 200)
            {
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    QJsonArray workspaceArray = dataObject.value("workspaces").toArray(QJsonArray());
                    timeModel->clearModel();
                    for (auto workspaceValue : workspaceArray)
                    {
                        if (workspaceValue.isObject())
                        {
                            QJsonObject workspace = workspaceValue.toObject();

                            QJsonArray projectArray = workspace.value("projects").toArray(QJsonArray());

                            for (auto projectValue : projectArray)
                            {
                                if (projectValue.isObject())
                                {
                                    QJsonObject project = projectValue.toObject();
                                    QJsonArray taskArray = project.value("tasks").toArray(QJsonArray());
                                    for (auto taskValue : taskArray)
                                    {
                                        if (taskValue.isObject())
                                        {
                                            QJsonObject task = taskValue.toObject();
                                            QString taskId = task.value("id").toString("");

                                            QJsonArray timeArray = task.value("timeEntries").toArray(QJsonArray());

                                            for(auto timeValue : timeArray)
                                            {
                                                if (timeValue.isObject())
                                                {
                                                    QJsonObject timeEntry = timeValue.toObject();

                                                    QString timeEntryId = timeEntry.value("id").toString("");


                                                    QTime time(0, 0, 0, 0);
                                                    time = time.addMSecs(static_cast<int>(timeEntry.value("duration").toVariant().toLongLong()));

                                                    QString durationAsString = time.toString("hh:mm:ss");

                                                    QDateTime startDateTime;
                                                    qint64 startDateValue = timeEntry["startDate"].toString("").toLongLong();
                                                    if (startDateValue > 0)
                                                        startDateTime.setMSecsSinceEpoch(startDateValue);

                                                    QDateTime endDateTime;
                                                    qint64 endDateValue = timeEntry["endDate"].toString("").toLongLong();
                                                    if (endDateValue > 0)
                                                        endDateTime.setMSecsSinceEpoch(endDateValue);

                                                    auto timeEntryItem = std::make_shared<TimeEntry>();

                                                    timeEntryItem->id = timeEntryId;
                                                    timeEntryItem->startDate = startDateTime.date().toString("dd.MM.yy");
                                                    timeEntryItem->startTime = startDateTime.time().toString("hh:mm:ss");
                                                    timeEntryItem->duration = durationAsString;
                                                    timeEntryItem->endDate = endDateTime.date().toString("dd.MM.yy");
                                                    timeEntryItem->endTime = endDateTime.time().toString("hh:mm:ss");
                                                    timeEntryItem->taskId = taskId;
                                                    timeEntryItem->startMSecsSinceEpoch = startDateValue;

                                                    timeModel->addItem(timeEntryItem);

                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    successCallback(true, "Time entry model updated");
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    });
}

void TimeEntryWebService::startTask(const QString &taskId, TimeEntryPtr timeEntry, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($taskId: String!){"
                                "startTask(taskId: $taskId) {"
                                    "id startDate}"
                            "}");

    QJsonObject taskParams {
        {"taskId", taskId}
    };

    HttpSinglton::instance()->postRequest(query, [this, successCallback, timeEntry, taskId](ResponsePtr resp) {
        if (resp->isError)
        {
            successCallback(false, resp->errorString);
        }
        else
        {
            if (resp->statusCode == 200)
            {
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    QJsonObject resultObject = dataObject.value("startTask").toObject(QJsonObject());
                    if (!resultObject.isEmpty())
                    {
                        QString timeId = resultObject.value("id").toString("");
                        QString startDate = resultObject.value("startDate").toString();

                        QDateTime dateTime;

                        dateTime.setMSecsSinceEpoch(startDate.toLongLong());

                        QString dateAsString = dateTime.date().toString("dd.MM.yy");

                        QString timeAsString = dateTime.time().toString("hh:mm:ss");

                        timeEntry->id = timeId;
                        timeEntry->taskId = taskId;
                        timeEntry->startDate = dateAsString;
                        timeEntry->startTime = timeAsString;
                        timeEntry->startMSecsSinceEpoch = startDate.toLongLong();

                        successCallback(true, QString("Time entry created with id: %1").arg(timeId));
                    }
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, taskParams);
}

void TimeEntryWebService::stopTimeEntry(TimeEntryPtr timeEntry, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($id: String!) {"
                                "stopTimeEntry(id: $id) {"
                                    "id duration endDate}"
                            "}");

    QJsonObject teParams {
        {"id", timeEntry->id}
    };

    HttpSinglton::instance()->postRequest(query, [this, successCallback, timeEntry](ResponsePtr resp) {
        if (resp->isError)
        {
            successCallback(false, resp->errorString);
        }
        else
        {
            if (resp->statusCode == 200)
            {
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    QJsonObject resultObject = dataObject.value("stopTimeEntry").toObject(QJsonObject());
                    if (!resultObject.isEmpty())
                    {
                        QTime time(0, 0, 0, 0);
                        time = time.addMSecs(static_cast<int>(resultObject.value("duration").toVariant().toLongLong()));

                        QString durationAsString = time.toString("hh:mm:ss");

                        QString endDate = resultObject.value("endDate").toString();

                        QDateTime dateTime;

                        dateTime.setMSecsSinceEpoch(endDate.toLongLong());

                        QString dateAsString = dateTime.date().toString("dd.MM.yy");

                        QString timeAsString = dateTime.time().toString("hh:mm:ss");

                        timeEntry->endDate = dateAsString;
                        timeEntry->endTime = timeAsString;
                        timeEntry->duration = durationAsString;

                        successCallback(true, QString("Time entry '%1' stopped").arg(timeEntry->id));
                    }
                }
                else
                    successCallback(false, "Incorrect response from server");
            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, teParams);
}

void TimeEntryWebService::updateTimeEntry(const QString &id, const QVariantMap &params, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($id: String!, $te: TimeEntryInput!){"
                                "updateTimeEntry(id: $id, timeEntry: $te)"
                            "}");
    QJsonObject teParams{
        {"id", id},
        {"te",
            QJsonObject {
                {"duration", params["duration"].toString()},
                {"startDate", params["startDate"].toString()},
                {"endDate", params["endDate"].toString()}
            }
        }
    };
    HttpSinglton::instance()->postRequest(query, [this, successCallback](ResponsePtr resp) {
        if (resp->isError)
        {
            successCallback(false, resp->errorString);
        }
        else
        {
            if (resp->statusCode == 200)
            {
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    if(dataObject.value("updateTimeEntry").toBool(false))
                        successCallback(true, "Ok");
                    else
                    {
                        QJsonArray errorArray = QJsonDocument::fromJson(resp->data)
                                .object()
                                .value("errors")
                                .toArray(QJsonArray());

                        if(!errorArray.isEmpty())
                        {
                            QJsonObject errorObject = errorArray[0].toObject();
                            QString message = errorObject.value("message").toString("Some error");
                            successCallback(false, message);
                        }
                        else
                        {
                            successCallback(false, "Some error");
                        }
                    }
                }
                else
                    successCallback(false, "Incorrect response from server");

            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, teParams);
}

void TimeEntryWebService::removeTimeEntry(const QString &id, SuccessCallback successCallback)
{
    QString query = QString("mutation M ($id: String!) {"
                                "removeTimeEntry(id: $id)"
                            "}");

    QJsonObject teParams{
        {"id", id}
    };

    HttpSinglton::instance()->postRequest(query, [this, successCallback](ResponsePtr resp) {
        if (resp->isError)
        {
            successCallback(false, resp->errorString);
        }
        else
        {
            if (resp->statusCode == 200)
            {
                QJsonObject dataObject = QJsonDocument::fromJson(resp->data)
                        .object()
                        .value("data")
                        .toObject(QJsonObject());
                if (!dataObject.isEmpty())
                {
                    if(dataObject.value("removeTimeEntry").toBool(false))
                        successCallback(true, "Ok");
                    else
                    {
                        QJsonArray errorArray = QJsonDocument::fromJson(resp->data)
                                .object()
                                .value("errors")
                                .toArray(QJsonArray());

                        if(!errorArray.isEmpty())
                        {
                            QJsonObject errorObject = errorArray[0].toObject();
                            QString message = errorObject.value("message").toString("Some error");
                            successCallback(false, message);
                        }
                        else
                        {
                            successCallback(false, "Some error");
                        }
                    }
                }
                else
                    successCallback(false, "Incorrect response from server");

            }
            else
                successCallback(false, QString("Request status not OK, status code:%0").arg(resp->statusCode));
        }
    }, teParams);
}
