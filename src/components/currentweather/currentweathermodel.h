#ifndef CURRENTWEATHERMODEL_H
#define CURRENTWEATHERMODEL_H

#include <QObject>
#include <QDateTime>
#include "../openweathermapapiclient/openweathermapapiclient.h"
#include "../../modules/appsettings/appsettings.h"

class QJsonDocument;

class CurrentWeatherModel : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QDateTime fetchingDateTime READ fetchingDateTime NOTIFY fetchingDateTimeChanged)
	Q_PROPERTY(QString city READ city NOTIFY cityChanged)
	Q_PROPERTY(QString iconFilePath READ iconFilePath NOTIFY iconFilePathChanged)
	Q_PROPERTY(double temperature READ temperature NOTIFY temperatureChanged)
	Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
	Q_PROPERTY(QString windblow READ windblow NOTIFY windblowChanged)
	Q_PROPERTY(double humidity READ humidity NOTIFY humidityChanged)

	Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
	Q_PROPERTY(bool hasError READ hasError NOTIFY hasErrorChanged)

public:

	explicit CurrentWeatherModel(
		QObject *parent = nullptr,
		AbstractOpenWeathermapApiClient* apiClient = OpenWeatherMapApiClient::instance(),
		AbstractAppSettings* appSettings = AppSettings::instance());

	QDateTime fetchingDateTime() const {return m_fetchingDateTime;}
	QString city() const {return m_city;}
	QString iconFilePath() const {return m_iconFilePath;}
	double temperature() const {return m_temperature;}
	QString description() const {return m_description;}
	QString windblow() const {return m_windblow;}
	double humidity() const {return m_humadity;}

	bool ready() const {return m_ready;}
	bool hasError() const {return m_hasError;}

public slots:
	void update();

signals:
	void fetchingDateTimeChanged();
	void cityChanged();
	void iconFilePathChanged();
	void temperatureChanged();
	void descriptionChanged();
	void windblowChanged();
	void humidityChanged();

	void readyChanged();
	void hasErrorChanged();

	void queryWeather(QString id, QString apiKey, QString lang);

private slots:
	void weatherFetched(QJsonDocument doc);
	void weatherErrorOccured(const AbstractOpenWeathermapApiClient::ApiError err);

private:
	void setPropertiesByDefault();

	bool checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path );

	QDateTime m_fetchingDateTime;
	QString m_city;
	QString m_iconFilePath;
	double m_temperature;
	QString m_description;
	QString m_windblow;
	double m_humadity;
	bool m_ready;
	bool m_hasError;

	AbstractOpenWeathermapApiClient* m_apiClient;
	AbstractAppSettings* m_appSettings;
};

#endif // CURRENTWEATHERMODEL_H
