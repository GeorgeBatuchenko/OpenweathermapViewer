#include "currentweathermodel.h"

#include <QJsonDocument>
#include <QLoggingCategory>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace {
	Q_LOGGING_CATEGORY(loggingCategory, "CurrentWeatherModel")
}

CurrentWeatherModel::CurrentWeatherModel(
	QObject *parent,
	AbstractOpenWeathermapApiClient* apiClient,
	AbstractAppSettings* appSettings) :

	QObject(parent),
	m_apiClient(apiClient),
	m_appSettings(appSettings)
{
	setPropertiesByDefault();

	connect(this, &CurrentWeatherModel::queryWeather, apiClient,  &AbstractOpenWeathermapApiClient::weatherByCityId);
	connect(apiClient, &AbstractOpenWeathermapApiClient::weatherByCityIdFetched, this, &CurrentWeatherModel::weatherFetched);
	connect(apiClient, &AbstractOpenWeathermapApiClient::weatherByCityIdErrorOccured, this, &CurrentWeatherModel::weatherErrorOccured);

	connect(appSettings, &AbstractAppSettings::settingsChanged, this, &CurrentWeatherModel::update);
}

void CurrentWeatherModel::update()
{
	qDebug(loggingCategory())<<"Start to update weater";
	setPropertiesByDefault();

	emit queryWeather(
		m_appSettings->value("city_id").toString(),
		m_appSettings->value("api_key").toString(),
		m_appSettings->value("api_lang").toString()
	);
}

void CurrentWeatherModel::weatherFetched(QJsonDocument doc)
{
	qDebug(loggingCategory())<<"Start parsing weather json";
	auto root = doc.object();

	auto city = root.value("name");
	if (!checkField(city, QJsonValue::Type::String,"name")) return;

	auto icon = root.value("weather").toArray().at(0).toObject().value("icon");
	if (!checkField(icon, QJsonValue::Type::String,"weather:icon")) return;

	auto desc = root.value("weather").toArray().at(0).toObject().value("description").toString();
	if (!checkField(desc, QJsonValue::Type::String,"weather:description")) return;
	desc[0] = desc[0].toTitleCase();

	QJsonValue temp = root.value("main").toObject().value("temp");
	if (!checkField(temp, QJsonValue::Type::Double,"main:temp")) return;

	auto feels = root.value("main").toObject().value("feels_like");
	if (!checkField(feels, QJsonValue::Type::Double, "main:feels_like")) return;

	auto humidity = root.value("main").toObject().value("humidity");
	if (!checkField(humidity, QJsonValue::Type::Double, "main:humidity")) return;

	auto windSpeed = root.value("wind").toObject().value("speed");
	if (!checkField(windSpeed,  QJsonValue::Type::Double,"wind:speed")) return;

	auto windDeg = root.value("wind").toObject().value("speed");
	if (!checkField(windDeg,  QJsonValue::Type::Double,"wind:deg")) return;

	qDebug(loggingCategory())<<"Json has been parsed successfuly";

	m_city = city.toString();
	// TODO helper method in api client for image url
	m_iconFilePath = "http://openweathermap.org/img/wn/"+icon.toString() + "@2x.png";
	m_description = tr("Feels like ") + QString::number(feels.toDouble(),'f',0) +"°C. " + desc;
	m_humadity = humidity.toDouble();
	m_temperature = temp.toDouble();
	m_windblow = QString::number(windSpeed.toDouble(),'f',1) + tr("m/s, "); //TODO chose direction

	m_ready = true;
	m_hasError = false;

	emit readyChanged();
	emit hasError();

	emit fetchingDateTimeChanged();
	emit cityChanged();
	emit temperatureChanged();
	emit iconFilePathChanged();
	emit descriptionChanged();
	emit humidityChanged();
	emit windblowChanged();
}

void CurrentWeatherModel::weatherErrorOccured(const AbstractOpenWeathermapApiClient::ApiError err)
{
	Q_UNUSED(err);

	qCritical(loggingCategory())<<"Fetching weather error";

	m_ready = true;
	m_hasError = true;

	emit readyChanged();
	emit hasErrorChanged();
}

void CurrentWeatherModel::setPropertiesByDefault()
{
	m_fetchingDateTime = QDateTime();
	emit fetchingDateTimeChanged();
	m_city = "-";
	emit cityChanged();
	m_temperature = 0;
	emit temperatureChanged();
	m_iconFilePath = "";
	emit iconFilePathChanged();
	m_description = "-";
	emit descriptionChanged();
	m_humadity = 0;
	emit humidityChanged();
	m_windblow = "-";
	emit windblowChanged();

	m_ready = false;
	emit readyChanged();
	m_hasError = false;
	emit hasErrorChanged();
}

bool CurrentWeatherModel::checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path)
{
//	const auto setErrors = [this] () {
//		m_hasError = true;
//		emit hasErrorChanged();
//		m_ready = true;
//		emit readyChanged();
//	};

//	if (field.isUndefined()) {
//		qInfo(loggingCategory())<<"Field "<<path<<" is not found in json.";
//		setErrors();
//		return false;
//	}

//	if (field.type() != targetType) {
//		qInfo(loggingCategory())<<"Unexpected field type: "<<path;
//		setErrors();
//		return false;
//	}

//	return true;

	using Api = AbstractOpenWeathermapApiClient;

	const auto err = Api::checkField(field, targetType, path);
	if (err.first != Api::FieldError::NoError) {
		qCritical(loggingCategory())<<err.second;
		m_hasError = true;
		emit hasErrorChanged();
		m_ready = true;
		emit readyChanged();
		return false;
	}

	return true;
}
