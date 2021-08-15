#include "openweathermapapiclient.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>
#include <QLoggingCategory>

namespace {
	Q_LOGGING_CATEGORY(loggingCategory, "OpenWeatherMapApiClient")
}

OpenWeatherMapApiClient::OpenWeatherMapApiClient(QObject *parent) :
	AbstractOpenWeathermapApiClient(parent),
	m_netManager(new QNetworkAccessManager())
{

}

bool OpenWeatherMapApiClient::processApiResponse(QNetworkReply* reply, QJsonDocument& doc)
{
	static constexpr int HTTP_OK_CODE = 200;

	if (reply->bytesAvailable() == 0) {
		QString errString = QString
			("Empty response body has been recieved. (") + reply->url().toString() + ")";
		qCritical(loggingCategory())<<errString;
		emit weatherByCityIdErrorOccured( ApiError(
			EmptyResponseBody,
			-1,
			QNetworkReply::NoError,
			reply->url(),
			errString
		));
		return false;
	}

	doc = QJsonDocument::fromJson(reply->readAll());
	if (doc.isNull() || doc.isEmpty()) {
		QString errString = QString
			("Broken response body has been recieved. (") + reply->url().toString() + ")";
		qCritical(loggingCategory())<<errString;
		emit weatherByCityIdErrorOccured( ApiError(
			BrokenResponseBody,
			-1,
			QNetworkReply::NoError,
			reply->url(),
			errString
		));
		return false;
	}

	QJsonValue code = doc.object().value("cod");
	if (code.isUndefined() || !code.isDouble()) {
		QString errString = QString
			("Bad json structure. (") + reply->url().toString()+ ": " +
			doc.toJson(QJsonDocument::Compact);
		qCritical(loggingCategory())<<errString;
		emit weatherByCityIdErrorOccured({
			BadJsonStructure,
			-1,
			QNetworkReply::NoError,
			reply->url(),
			errString
		});
		return false;
	}

	if (code.toInt() != HTTP_OK_CODE) {
		QString errString = QString
			("Response with error code received. (") + reply->url().toString() + ": " +
			doc.toJson(QJsonDocument::Compact);
		qCritical(loggingCategory())<<errString;
		emit weatherByCityIdErrorOccured({
			ResponseWithErrorCode,
			code.toInt(),
			QNetworkReply::NoError,
			reply->url(),
			errString
		});
		return false;
	}

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
		QString errString = "Network error occured on request: "+
							reply->url().toString()+" "+reply->errorString();

		qCritical(loggingCategory())<<errString;
		emit weatherByCityIdErrorOccured(ApiError(
			NetworkError,
			-1,
			reply->error(),
			reply->url(),
			errString
		));
		reply->deleteLater();
		return;
	}

	QJsonDocument doc;
	if (!processApiResponse(reply, doc)) {
		reply->deleteLater();
		return;
	}

	reply->deleteLater();

	emit weatherByCityIdFetched(doc);
}