#include "translationmodel.h"

TranslationModel::TranslationModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

TranslationModel::~TranslationModel()
{
    clearModel();
}

int TranslationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return count();
}

QVariant TranslationModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_items.length())
        return QVariant();

    switch (role) {
    case IdRole:
        return m_items[index.row()]->id;
    case LocaleRole:
        return m_items[index.row()]->locale;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TranslationModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(IdRole, "localeId");
    roles.insert(LocaleRole, "locale");
    return roles;
}

void TranslationModel::addItem(QString locale, QTranslator *translator)
{
    TranslationItem* item = new TranslationItem;
    item->locale = locale;
    item->translator = translator;

    addItem(item);
}

void TranslationModel::addItem(TranslationItem *item)
{
    beginInsertRows(QModelIndex(), count(), count());
    m_items.append(item);
    endInsertRows();

    emit countChanged(count());
}

QTranslator *TranslationModel::getTranslator(QString locale)
{
    TranslationItem* item = getItem(locale);

    return item ? item->translator : nullptr;
}

TranslationItem *TranslationModel::getItem(QString locale)
{
    for(auto item : m_items)
    {
        if(item->locale == locale)
            return item;
    }

    return nullptr;
}

TranslationItem *TranslationModel::getItem(int index)
{
    if(index < 0 || index >= m_items.size())
        return nullptr;

    return m_items.at(index);
}

int TranslationModel::getIndex(QString locale)
{
    int i = 0;
    for(auto item : m_items)
    {
        if(item->locale == locale)
            return i;
        i++;
    }

    return -1;
}

int TranslationModel::count() const
{
    return m_items.size();
}

void TranslationModel::clearModel()
{
    beginRemoveRows(QModelIndex(), 0, count() - 1);
    for(auto item : m_items)
    {
        delete item->translator;
        delete item;
    }
    m_items.clear();
    endRemoveRows();

    emit countChanged(count());
}
