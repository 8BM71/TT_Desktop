#ifndef INCLUDERANSLATIONMODEL_H
#define INCLUDERANSLATIONMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QTranslator>

struct TranslationItem {
    int id;
    QTranslator* translator;
    QString locale;
};

class TranslationModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count /*WRITE setCount*/ NOTIFY countChanged)

public:
    explicit TranslationModel(QObject *parent = nullptr);
    ~TranslationModel();

    enum Roles{
        IdRole = Qt::UserRole + 1,
        LocaleRole
    };

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addItem(QString locale, QTranslator* translator);
    void addItem(TranslationItem* item);

    QTranslator* getTranslator(QString locale);
    TranslationItem* getItem(QString locale);
    TranslationItem* getItem(int index);
    int getIndex(QString locale);

    int count() const;
    void clearModel();

signals:
    void countChanged(int count);

private:
    QList<TranslationItem*> m_items;
};

#endif // INCLUDERANSLATIONMODEL_H
