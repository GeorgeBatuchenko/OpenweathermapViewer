#include "hourlyforecastmodel.h"

#include <QLoggingCategory>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

namespace {
	Q_LOGGING_CATEGORY(loggingCategory, "HourlyForecastModel")
}

HourlyForecastModel::HourlyForecastModel(
	QObject* parent,
	AbstractOpenWeathermapApiClient* apiClient,
	AbstractAppSettings* appSettings
) :
	QAbstractListModel(parent),
	m_apiClient(apiClient),
	m_appSettings(appSettings),
	m_state(Ready)
{
	connect(this, &HourlyForecastModel::queryHourlyForecast,
			apiClient, &AbstractOpenWeathermapApiClient::hourlyForecast);
	connect(apiClient, &AbstractOpenWeathermapApiClient::hourlyForecastFetched,
			this, &HourlyForecastModel::hourlyForecastFinished);
	connect(apiClient, &AbstractOpenWeathermapApiClient::hourlyForecastErrorOccured,
			this, &HourlyForecastModel::hourlyForecastErrorOccured);

	connect(appSettings, &AbstractAppSettings::settingsChanged, this, &HourlyForecastModel::update);
}

HourlyForecastModel::~HourlyForecastModel()
{

}

int HourlyForecastModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return m_items.size();
}

QVariant HourlyForecastModel::data(const QModelIndex& index, int role) const
{
	if (index.row() >= rowCount()) {
		Q_ASSERT(false);
		return QVariant();
	}

	if (role == TimeRole)
		return m_items[index.row()].time;

	if (role == IconUrlRole)
		return m_items[index.row()].iconUrlString;

	if (role == TempetatureRole)
		return m_items[index.row()].temperature;

	return QVariant();
}

QHash<int, QByteArray> HourlyForecastModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[TimeRole] = "time";
	roles[IconUrlRole] = "iconUrl";
	roles[TempetatureRole] = "temperature";
	return roles;
}

void HourlyForecastModel::update()
{
	qDebug(loggingCategory())<<"Start to update daily forecast";

	clear();
	m_state = Query;
	emit stateChanged();

	const auto city = m_appSettings->value("city").value<QVariantHash>();
	bool conv = true;
	const auto lon = city["lon"].toDouble(&conv);
	Q_ASSERT(conv);
	const auto lat = city["lat"].toDouble(&conv);
	Q_ASSERT(conv);

	emit queryHourlyForecast(
		lon,
		lat,
		m_appSettings->value("api_key").toString(),
		m_appSettings->value("api_lang").toString()
	);
}

void HourlyForecastModel::hourlyForecastFinished(QJsonDocument doc)
{
	using Api=AbstractOpenWeathermapApiClient;
	static constexpr int DELAY_2_HOURS_SEC = 2*60*60 - 30*60;
	static constexpr int DELAY_6_HOURS_SEC = 6*60*60 - 30*60;

	qDebug(loggingCategory())<<"Start parse hourly forecast json";

	const QJsonArray hourlyForecast = doc.object().value("hourly").toArray();
	if (!checkField(hourlyForecast, QJsonValue::Array, "hourly")) return;

	QList<Item> items;
	auto refDateTime = QDateTime::currentDateTime();
	auto deleay = DELAY_2_HOURS_SEC;
	for (const auto& f : hourlyForecast) {
		const auto dt = f.toObject().value("dt");
		if (!checkField(dt, QJsonValue::Double, "hourly:item:dt")) return;

		const auto parsedDateTime = QDateTime::fromSecsSinceEpoch(dt.toInt());
		if (!(parsedDateTime > refDateTime && refDateTime.secsTo(parsedDateTime) >= deleay ) ) {
			continue;
		}

		refDateTime = parsedDateTime;
		deleay = DELAY_6_HOURS_SEC;

		const auto icon = f.toObject().value("weather").toArray().at(0).toObject().value("icon");
		if (!checkField(icon, QJsonValue::String, "hourly:weather:icon")) return;

		const auto temp = f.toObject().value("temp");
		if (!checkField(temp, QJsonValue::Double, "hourly:temp")) return;

		items.append(Item(
			QDateTime::fromSecsSinceEpoch(dt.toInt()).toString("hh:mm"),
			Api::makeNormalIconUrlString(icon.toString()),
			temp.toDouble()
		));

		if (items.size() == 4)
			break;
	}

	qDebug(loggingCategory())<<"Hourly forecast has been parsed succesfuly";

	beginInsertRows(QModelIndex(), 0, items.size()-1);
	m_items.append(items);
	endInsertRows();

	m_state = Ready;
	emit stateChanged();
}

void HourlyForecastModel::hourlyForecastErrorOccured(AbstractOpenWeathermapApiClient::ApiError err)
{
	m_state = Error;
	emit stateChanged();
}

void HourlyForecastModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	m_items.clear();
	endRemoveRows();
}


bool HourlyForecastModel::checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path)
{
	using Api = AbstractOpenWeathermapApiClient;

	const auto err = Api::checkField(field, targetType, path);
	if (err.first != Api::FieldError::NoError) {
		qCritical(loggingCategory())<<err.second;
		m_state = Error;
		emit stateChanged();
	}

	return true;
}
