#include "translationservice.h"
#include <QLoggingCategory>

#include <QTranslator>
#include <QtGui>

Q_LOGGING_CATEGORY(translationService, "Services.Translation")

TranslationService::TranslationService(QObject *parent)
    : QObject(parent)
    , m_translationModel(new TranslationModel)
    , m_translations(QStringList() << "ru_RU" << "en_GB")
    , m_defaultLocale("ru_RU")
    , m_locale()
{

}

TranslationService::~TranslationService()
{
    if(m_translationModel)
        delete m_translationModel;
}

bool TranslationService::loadTranslation()
{
    if(m_translations.isEmpty())
    {
        qCWarning(translationService) << "Translations not loaded: translation list is empty";
        return false;
    }

    for(QString locale : m_translations)
    {
        if(m_translationModel->getItem(locale))
            continue;

        QTranslator* translator = new QTranslator;
        if(translator->load(locale,":/locales"))
        {
            TranslationItem* trItem = new TranslationItem();
            trItem->locale = locale;
            trItem->translator = translator;
            m_translationModel->addItem(trItem);
            qCInfo(translationService) << "Translations loaded:" << locale;
        }
        else
        {
            qCCritical(translationService) << "Translation not loaded: " << locale;
            delete translator;
        }
    }

    return m_translationModel->count();
}

void TranslationService::setTranslation(QStringList &translations)
{
    if(m_translations == translations)
        return;

    m_translations = translations;
    qCInfo(translationService) << "New translations list:" << m_translations;

    loadTranslation();
}

QString TranslationService::locale() const
{
    return m_locale;
}

bool TranslationService::switchLocale()
{
    if(m_translationModel->count() <= 1)
    {
        qCWarning(translationService) << "There are is not other locale. Translations count =" << m_translationModel->count();
        return false;
    }

    int ind = m_translationModel->getIndex(m_locale);

    if(ind < m_translationModel->count() - 1)
        return setLocale(m_translationModel->getItem(ind+1)->locale);
    else
        return setLocale(m_translationModel->getItem(0)->locale);
}

bool TranslationService::setLocale(const QString &locale)
{
    if(m_locale == locale)
        return true;

    TranslationItem* trItem = m_translationModel->getItem(locale);
    if(!trItem)
    {
        qCCritical(translationService) << QString("Locale %1 is not set: it's translation is not loaded").arg(locale);
        return false;
    }

    if(!m_locale.isEmpty())
        qApp->removeTranslator(m_translationModel->getItem(m_locale)->translator);

    bool success = qApp->installTranslator(trItem->translator);
    if(success)
    {
        m_locale = locale;
        emit localeChanged(locale);
        qCInfo(translationService) << QString("Set locale: %1").arg(locale);
    }
    else
    {
        qCCritical(translationService) << QString("Locale %1 is not set: translator installation error").arg(locale);
    }

    return success;
}

void TranslationService::setDefaultLocale(const QString &defaultLocale)
{
    if(m_defaultLocale == defaultLocale)
        return;

    m_defaultLocale = defaultLocale;
}

TranslationModel *TranslationService::translationModel() const
{
    return m_translationModel;
}

QString TranslationService::trString() const
{
    return "";
}


void TranslationService::startSession()
{
}

void TranslationService::stopSession()
{
    if(m_locale != m_defaultLocale)
        setLocale(m_defaultLocale);
}

