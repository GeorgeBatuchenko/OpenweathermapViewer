#ifndef OPENWEATHERMAPAPICLIENT_H
#define OPENWEATHERMAPAPICLIENT_H

#include "abstractopenweathermapapiclient.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QScopedPointer>
#include <QUrl>
#include <functional>

class OpenWeatherMapApiClient : public AbstractOpenWeathermapApiClient
{
	Q_OBJECT
public:
	OpenWeatherMapApiClient(const OpenWeatherMapApiClient&) = delete;
	OpenWeatherMapApiClient(OpenWeatherMapApiClient&&) = delete;

	~OpenWeatherMapApiClient() override final;
	static OpenWeatherMapApiClient* instance();

public slots:
	void weatherByCityId(QString id, QString apiKey, QString lang) override final;
	void findCity(QString name, QString apiKey) override final;
	void dailyForecast(double lon, double lat, QString apiKey, QString lang) override final;

signals:

private slots:
	void weatherByCityIdFinished(QNetworkReply* reply);
	void findCityReplyFinished(QNetworkReply* reply);
	void dailyForecastFinished(QNetworkReply* reply);

private:
	explicit OpenWeatherMapApiClient(QObject *parent = nullptr);

	bool processApiResponse(QNetworkReply* reply,
		QJsonDocument& doc,
		const std::function<void (const ApiError&)>& emitter
	);

	bool processOneCallApiResponse(QNetworkReply* reply,
		QJsonDocument& doc,
		const std::function<void (const ApiError&)>& emitter
	);

	QScopedPointer<QNetworkAccessManager> m_netManager;
};

#endif // OPENWEATHERMAPAPICLIENT_H
