#include "abstractopenweathermapapiclient.h"
#include <QString>
#include <QTextStream>

const QString AbstractOpenWeathermapApiClient::WEATHER_URL_STRING = "https://api.openweathermap.org/data/2.5/weather";
const QString AbstractOpenWeathermapApiClient::FORECAST_URL_STRING = "https://api.openweathermap.org/data/2.5/onecall";
const QString AbstractOpenWeathermapApiClient::ICON_URL_STRING = "http://openweathermap.org/img/wn/";

AbstractOpenWeathermapApiClient::AbstractOpenWeathermapApiClient(QObject *parent) : QObject(parent)
{

}

AbstractOpenWeathermapApiClient::~AbstractOpenWeathermapApiClient()
{

}

std::pair<AbstractOpenWeathermapApiClient::FieldError, QString>
AbstractOpenWeathermapApiClient::checkField(
	const QJsonValue& field,
	QJsonValue::Type targetType,
	const QString& path)
{
	QString errString;
	QTextStream stream(&errString);
	if (field.isUndefined()) {
		stream<<"Field "<<path<<" is not found in json.";
		return {FieldError::FieldNotFound, errString};
	}

	if (field.type() != targetType) {
		stream<<"Unexpected field type: "<<path;
		return {FieldError::UnexpectedFieldType, errString};
	}

	return {FieldError::NoError, "No error"};
}

QString AbstractOpenWeathermapApiClient::makeHugeIconUrlString(const QString& icon)
{
	return ICON_URL_STRING + icon + "@2x.png";
}

QString AbstractOpenWeathermapApiClient::makeNormalIconUrlString(const QString& icon)
{
	return ICON_URL_STRING + icon + ".png";
}
