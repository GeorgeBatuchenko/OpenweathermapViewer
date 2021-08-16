#ifndef OPENWEATHERMAPAPICLIENT_H
#define OPENWEATHERMAPAPICLIENT_H

#include "abstractopenweathermapapiclient.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QScopedPointer>
#include <QUrl>

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

signals:

private slots:
	void weatherByCityIdFinished(QNetworkReply* reply);
	void findCityReplyFinished(QNetworkReply* reply);

private:
	explicit OpenWeatherMapApiClient(QObject *parent = nullptr);
	bool processApiResponse(QNetworkReply* reply, QJsonDocument& doc);

	QScopedPointer<QNetworkAccessManager> m_netManager;
};

#endif // OPENWEATHERMAPAPICLIENT_H
