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
	virtual ~OpenWeatherMapApiClient();
	static OpenWeatherMapApiClient* instance();

public slots:
	void weatherByCityId(QString id, QString apiKey, QString lang) override;

signals:

private slots:
	void weatherByCityIdFinished(QNetworkReply* reply);

private:
	explicit OpenWeatherMapApiClient(QObject *parent = nullptr);
	bool checkApiResponse(const QJsonDocument& doc) const;

	QScopedPointer<QNetworkAccessManager> m_netManager;
};

#endif // OPENWEATHERMAPAPICLIENT_H
