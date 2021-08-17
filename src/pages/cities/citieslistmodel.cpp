#include "citieslistmodel.h"

#include <QLoggingCategory>
#include <QJsonValue>
#include <QJsonObject>
#include <QTextStream>

namespace {
	Q_LOGGING_CATEGORY(loggingCategory, "CitiesListModel")
}

CitiesListModel::CitiesListModel(
	QObject* parent,
	AbstractOpenWeathermapApiClient* apiClient,
	AbstractAppSettings* appSettings
) :
	QAbstractListModel(parent),
	m_apiClient(apiClient),
	m_appSettings(appSettings),
	m_state(Ready)
{
	connect(this, &CitiesListModel::findCity, apiClient, &AbstractOpenWeathermapApiClient::findCity);
	connect(apiClient, &AbstractOpenWeathermapApiClient::findCityFinished,
			this, &CitiesListModel::findCityFinished);
	connect(apiClient, &AbstractOpenWeathermapApiClient::findCityErrorOccured,
			this, &CitiesListModel::findCityErrorOccured);
}

CitiesListModel::~CitiesListModel()
{

}

int CitiesListModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return m_items.size();
}

QVariant CitiesListModel::data(const QModelIndex& index, int role) const
{
	if (index.row() >= rowCount()) {
		Q_ASSERT(false);
		return QVariant();
	}

	if (role == IndexRole)
		return m_items[index.row()].index;

	if (role == NameRole)
		return m_items[index.row()].name;

	if (role == IdRole)
		return m_items[index.row()].id;

	if (role == LongitudeRole)
		return m_items[index.row()].lon;

	if (role == LatitudeRole)
		return m_items[index.row()].lat;

	return QVariant();
}

QHash<int, QByteArray> CitiesListModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[IndexRole] = "index";
	roles[NameRole] = "cityName";
	roles[IdRole] = "cityId";
	roles[LongitudeRole] = "cityLon";
	roles[LatitudeRole] = "cityLat";
	return roles;
}

void CitiesListModel::update(QString cityName)
{
	clear();
	m_state = Query;
	emit stateChanged();
	emit findCity(cityName, m_appSettings->value("api_key").toString());
}

void CitiesListModel::saveItem(int index)
{
	Q_ASSERT(index < m_items.size());

	qDebug(loggingCategory())<<"Save item " << index << " to appsettings";

	QVariantHash city;
	city.insert("id",m_items[index].id);
	city.insert("lon",m_items[index].lon);
	city.insert("lat",m_items[index].lat);

	m_appSettings->setValue("city", city);
}

void CitiesListModel::findCityFinished(QJsonDocument doc)
{
	auto name = doc.object().value("name");
	if (!checkField(name, QJsonValue::String, "name")) return;

	auto id = doc.object().value("id");
	if (!checkField(id, QJsonValue::Double, "id")) return;

	auto lon = doc.object().value("coord").toObject().value("lon");
	if (!checkField(lon, QJsonValue::Double, "coord:lon")) return;

	auto lat = doc.object().value("coord").toObject().value("lat");
	if (!checkField(lat, QJsonValue::Double, "coord:lat")) return;

	appendItem( name.toString(), id.toInt(), lon.toDouble(), lat.toDouble() );

	m_state = Ready;
	emit stateChanged();
}

void CitiesListModel::findCityErrorOccured(AbstractOpenWeathermapApiClient::ApiError err)
{
	using Api = AbstractOpenWeathermapApiClient;
	if (err.type == Api::ApiErrorType::ResponseWithErrorCode ||
		err.type == Api::ApiErrorType::NetworkError) {
		if (err.errorCode == Api::HTTP_NOT_FOUND_CODE ||
			err.networkError == QNetworkReply::NetworkError::ContentNotFoundError) {

			clear();
			appendItem(tr("Not found"), -1, 0, 0);

			m_state = NotFound;
			emit stateChanged();
			return;
		}
	}

	m_state = Error;
	emit stateChanged();
}

void CitiesListModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, rowCount());
	m_items.clear();
	endRemoveRows();
}

void CitiesListModel::appendItem(const QString& _name, int _id, double _lon, double _lat)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	m_items.append(Item(m_items.size(), _name, _id, _lon, _lat));
	endInsertRows();
}


bool CitiesListModel::checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path)
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
