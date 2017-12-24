#ifndef INCLUDERANSLATIONSERVICE_H
#define INCLUDERANSLATIONSERVICE_H

#include <QObject>
#include "translationmodel.h"

class TranslationService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString locale READ locale WRITE setLocale NOTIFY localeChanged)
    Q_PROPERTY(QString trString READ trString NOTIFY localeChanged)
    Q_PROPERTY(TranslationModel* translationModel READ translationModel NOTIFY translationModelChanged)

public:
    explicit TranslationService(QObject *parent = nullptr);
    ~TranslationService();

    void startSession();
    void stopSession();

    bool loadTranslation();
    void setTranslation(QStringList &translations);

    QString locale() const;
    bool switchLocale();
    bool setLocale(const QString &locale);
    void setDefaultLocale(const QString &defaultLocale);

    TranslationModel *translationModel() const;

    QString trString() const;

signals:
    void localeChanged(const QString &locale);
    void translationModelChanged(TranslationModel *translationModel);

public slots:

private:
    TranslationModel* m_translationModel;
    QStringList m_translations;
    QString m_defaultLocale;
    QString m_locale;

};

#endif // INCLUDERANSLATIONSERVICE_H
