#ifndef TIMEENTRYWEBSERVICE_H
#define TIMEENTRYWEBSERVICE_H

#include <QObject>
#include "timeentriesmodel.h"
#include "httpsinglton.h"

using namespace Entities;

class TimeEntryWebService : public QObject
{
    Q_OBJECT
public:
    explicit TimeEntryWebService(QObject *parent = nullptr);

    void getAllTimeEntries(const QString &ownerId, TimeEntriesModelPtr timeModel, SuccessCallback successCallback);

    void startTask(const QString &taskId, TimeEntryPtr timeEntry, SuccessCallback successCallback);

    void stopTimeEntry(TimeEntryPtr timeEntry, SuccessCallback successCallback);

    /**
     * @brief updateTimeEntry
     * @param id
     * @param params - QVariantMap:
     * "duration" - new duration (int)
     * "startDate" - new startDate in MSecsSinceEpoch (QString)
     * "endDate" - new endDate in MSecsSinceEpoch (QString)
     * @param successCallback
     */
    void updateTimeEntry(const QString &id, const QVariantMap &params, SuccessCallback successCallback);

    void removeTimeEntry(const QString &id, SuccessCallback successCallback);

};

#endif // TIMEENTRYWEBSERVICE_H
