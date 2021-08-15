#include "openweathermapapiclient.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>

OpenWeatherMapApiClient::OpenWeatherMapApiClient(QObject *parent) :
	AbstractOpenWeathermapApiClient(parent),
	m_netManager(new QNetworkAccessManager())
{

}

bool OpenWeatherMapApiClient::checkApiResponse(const QJsonDocument& doc) const
{
	static constexpr int HTTP_OK_CODE = 200;

	if (doc.isNull() || doc.isEmpty()) return false;

	QJsonValue code = doc.object().value("cod");
	if (code.isUndefined() || !code.isDouble()) return false;
	if (code.toInt() != HTTP_OK_CODE) return false;

	return true;
}

OpenWeatherMapApiClient::~OpenWeatherMapApiClient()
{

}

OpenWeatherMapApiClient* OpenWeatherMapApiClient::instance()
{
	static OpenWeatherMapApiClient inst;
	return &inst;
}

void OpenWeatherMapApiClient::weatherByCityId(QString id, QString apiKey, QString lang)
{
	Q_ASSERT(m_netManager);

	QUrl url("https://api.openweathermap.org/data/2.5/weather");
	QUrlQuery query;

	query.addQueryItem("id", id);
	query.addQueryItem("appid", apiKey);
	query.addQueryItem("lang", lang);
	query.addQueryItem("units", "metric");
	url.setQuery(query);

	QNetworkReply *reply = m_netManager->get(QNetworkRequest(url));
	Q_ASSERT(reply != nullptr);

	connect(reply, &QNetworkReply::finished, this, [this, reply] () {weatherByCityIdFinished(reply);} );
}

void OpenWeatherMapApiClient::weatherByCityIdFinished(QNetworkReply* reply)
{
	if (reply->error() != QNetworkReply::NoError) {
		emit networkErrorOccured(reply->url(), reply->errorString());
		reply->deleteLater();
		return;
	}

	QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
	reply->deleteLater();

	if (!checkApiResponse(doc)) {
		emit apiResponseErrorDetected(reply->url(), doc);
		return;
	}

	emit weatherByCityIdFetched(doc);
}
