#ifndef GRAPHQLPARSER_H
#define GRAPHQLPARSER_H

#include <QObject>

class GraphQLParser : public QObject
{
    Q_OBJECT
public:
    explicit GraphQLParser(QObject *parent = nullptr);

signals:

public slots:
};

#endif // GRAPHQLPARSER_H