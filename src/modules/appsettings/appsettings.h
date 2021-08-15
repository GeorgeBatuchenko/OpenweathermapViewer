#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
#include <QVariant>
#include <QMutex>
#include <QScopedPointer>
#include "abstractappsettings.h"

class AppSettings : public AbstractAppSettings
{
public:
	AppSettings(AppSettings&) = delete;
	AppSettings(AppSettings&&) = delete;

	~AppSettings() override final;

	static AppSettings* instance();

	QVariant value(const QString& key) const override final;

public slots:
	void setValue(const QString &key, const QVariant &value) override final;

private:
	explicit AppSettings();
	void setDefaults();

	QScopedPointer<QSettings> m_settings;
	QMutex m_mutex;
};

#endif // APPSETTINGS_H
