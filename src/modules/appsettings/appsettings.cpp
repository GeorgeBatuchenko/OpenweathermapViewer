#include "appsettings.h"
#include <QMutexLocker>
#include <QFile>

AppSettings::AppSettings() :
m_settings(new QSettings("appsettings.ini", QSettings::IniFormat))
{
	QMutexLocker lock(&m_mutex);
	if (!m_settings->contains("city_id") ||
		!m_settings->contains("api_key") ||
		!m_settings->contains("app_lang") ||
		!m_settings->contains("api_lang")
	) {
		setDefaults();
	}
}

AppSettings::~AppSettings()
{

}

void AppSettings::setDefaults()
{
	m_settings->setValue("city_id", "524901"); // set to moscow
	m_settings->setValue("api_key", "");
	m_settings->setValue("app_lang", "en_US");
	m_settings->setValue("api_lang", "en");
	m_settings->sync();
}

AppSettings* AppSettings::instance()
{
	static AppSettings settings;
	return &settings;
}

void AppSettings::setValue(const QString& key, const QVariant& value)
{
	QMutexLocker lock(&m_mutex);
	m_settings->setValue(key, value);
	m_settings->sync();
}

QVariant AppSettings::value(const QString& key) const
{
	return m_settings->value(key);
}
