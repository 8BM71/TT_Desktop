#ifndef ITEMS_H
#define ITEMS_H

#include <QString>
#include <memory>

namespace Entities {

struct TimeEntry {
    QString id;
    QString duration;
    QString startDate;
    QString startTime;
    QString endDate;
    QString endTime;
    QString taskId;
};

struct Task {
    QString id;
    QString name;
    QString description;
    QString projectId;
};

struct User {
    QString id;
    QString name;
    QString email;
};

struct Project {
    QString id;
    QString name;
    QString description;
    QString workspaceId;
};

struct Workspace {
    QString id;
    QString name;
    QString ownerId;
};

typedef std::shared_ptr<Workspace> WorkspacePtr;
typedef std::shared_ptr<Project> ProjectPtr;
typedef std::shared_ptr<User> UserPtr;
typedef std::shared_ptr<Task> TaskPtr;
typedef std::shared_ptr<TimeEntry> TimeEntryPtr;

}

#endif // ITEMS_H
