#include "appsettings.h"
#include <QFile>

AppSettings::AppSettings() :
	m_settings(new QSettings("appsettings.ini", QSettings::IniFormat))
{
	if (!m_settings->contains("city") ||
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
	QVariantHash city;
	city.insert("id", 524901); // Set to Moscow
	city.insert("lon", 37.6156);
	city.insert("lat", 55.7522);

	m_settings->setValue("city", city);
	m_settings->setValue("api_key", "");
	m_settings->setValue("app_lang", "en_US");
	m_settings->setValue("api_lang", "en");
	m_settings->sync();
	emit settingsChanged();
}

AppSettings* AppSettings::instance(QQmlEngine* engine, QJSEngine* scriptEngine)
{
	Q_UNUSED(engine);
	Q_UNUSED(scriptEngine);

	static AppSettings* settings = new AppSettings();
	return settings;
}

void AppSettings::setValue(const QString& key, const QVariant& value)
{
	m_settings->setValue(key, value);
	m_settings->sync();
	emit settingsChanged();
}

QVariant AppSettings::value(const QString& key) const
{
	return m_settings->value(key);
}
