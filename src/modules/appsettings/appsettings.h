#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
#include <QVariant>
#include <QScopedPointer>
#include "abstractappsettings.h"

class QQmlEngine;
class QJSEngine;

class AppSettings : public AbstractAppSettings
{
	Q_OBJECT
public:
	AppSettings(AppSettings&) = delete;
	AppSettings(AppSettings&&) = delete;

	~AppSettings() override final;

	static AppSettings* instance(
		QQmlEngine *engine = nullptr,
		QJSEngine *scriptEngine = nullptr
	);

	QVariant value(const QString& key) const override final;

public slots:
	void setValue(const QString &key, const QVariant &value) override final;

private:
	explicit AppSettings();
	void setDefaults();

	QScopedPointer<QSettings> m_settings;
};

#endif // APPSETTINGS_H
