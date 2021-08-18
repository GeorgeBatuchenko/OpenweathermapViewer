#include "dailyforecastmodel.h"

#include <QLoggingCategory>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

namespace {
	Q_LOGGING_CATEGORY(loggingCategory, "DailyForecastModel")
}

DailyForecastModel::DailyForecastModel(
	QObject* parent,
	AbstractOpenWeathermapApiClient* apiClient,
	AbstractAppSettings* appSettings
) :
	QAbstractListModel(parent),
	m_apiClient(apiClient),
	m_appSettings(appSettings),
	m_state(Ready)
{
	connect(this, &DailyForecastModel::queryDaylyForecast,
			apiClient, &AbstractOpenWeathermapApiClient::dailyForecast);
	connect(apiClient, &AbstractOpenWeathermapApiClient::dailyForecastFetched,
			this, &DailyForecastModel::dailyForecastFinished);
	connect(apiClient, &AbstractOpenWeathermapApiClient::dailyForecastErrorOccured,
			this, &DailyForecastModel::dailyForecastOccured);

	connect(appSettings, &AbstractAppSettings::settingsChanged, this, &DailyForecastModel::update);
}

DailyForecastModel::~DailyForecastModel()
{

}

int DailyForecastModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return m_items.size();
}

QVariant DailyForecastModel::data(const QModelIndex& index, int role) const
{
	if (index.row() >= rowCount()) {
		Q_ASSERT(false);
		return QVariant();
	}

	if (role == DateRole)
		return m_items[index.row()].date;

	if (role == IconUrlRole)
		return m_items[index.row()].iconUrlString;

	if (role == TempetatureRole)
		return m_items[index.row()].temperature;

	if (role == DescriptionRole)
		return m_items[index.row()].description;

	return QVariant();
}

QHash<int, QByteArray> DailyForecastModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[DateRole] = "date";
	roles[IconUrlRole] = "iconUrl";
	roles[TempetatureRole] = "temperature";
	roles[DescriptionRole] = "description";
	return roles;
}

void DailyForecastModel::update()
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

	emit queryDaylyForecast(
		lon,
		lat,
		m_appSettings->value("api_key").toString(),
		m_appSettings->value("api_lang").toString()
	);
}

void DailyForecastModel::dailyForecastFinished(QJsonDocument doc)
{
	using Api=AbstractOpenWeathermapApiClient;

	qDebug(loggingCategory())<<"Start parse daily forecast json";

	const QJsonArray dailyForecast = doc.object().value("daily").toArray();
	if (!checkField(dailyForecast, QJsonValue::Array, "daily")) return;

	QList<Item> items;
	bool skip = true;
	for (const auto& f : dailyForecast) {
		if (skip) { // skip firsst item because it is today
			skip = false;
			continue;
		}

		const auto dt = f.toObject().value("dt");
		if (!checkField(dt, QJsonValue::Double, "daily:item:dt")) return;

		const auto icon = f.toObject().value("weather").toArray().at(0).toObject().value("icon");
		if (!checkField(icon, QJsonValue::String, "daily:weather:icon")) return;

		const auto desc = f.toObject().value("weather").toArray().at(0).toObject().value("description");
		if (!checkField(desc, QJsonValue::String, "daily:weather:description")) return;

		const auto temp = f.toObject().value("temp").toObject().value("day");
		if (!checkField(temp, QJsonValue::Double, "daily:temp:day")) return;

		QString capitalDesc = desc.toString();
		capitalDesc[0] = capitalDesc[0].toTitleCase();

		items.append(Item(
			QDateTime::fromSecsSinceEpoch(dt.toInt()).toString("dd.MM.yy"),
			Api::makeNormalIconUrlString(icon.toString()),
			temp.toDouble(),
			capitalDesc
		));
	}

	qDebug(loggingCategory())<<"Daily forecast has been parsed succesfuly";

	beginInsertRows(QModelIndex(), 0, items.size()-1);
	m_items.append(items);
	endInsertRows();

	m_state = Ready;
	emit stateChanged();
}

void DailyForecastModel::dailyForecastOccured(AbstractOpenWeathermapApiClient::ApiError err)
{
	m_state = Error;
	emit stateChanged();
}

void DailyForecastModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	m_items.clear();
	endRemoveRows();
}


bool DailyForecastModel::checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path)
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
