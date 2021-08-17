#include "citieslistmodel.h"

#include <QLoggingCategory>
#include <QJsonValue>
#include <QJsonObject>

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
	m_ready(false),
	m_hasError(false)
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

	if (role == NameRole)
		return m_items[index.row()].name;

	if (role == IdRole)
		return m_items[index.row()].id;

	return QVariant();
}

QHash<int, QByteArray> CitiesListModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[NameRole] = "cityName";
	roles[IdRole] = "cityId";
	return roles;
}

void CitiesListModel::update(QString cityName)
{
	clear();
	emit findCity(cityName, m_appSettings->value("api_key").toString());
}

void CitiesListModel::findCityFinished(QJsonDocument doc)
{
	auto name = doc.object().value("name");
	auto id = doc.object().value("id");

	appendItem( Item( name.toString(), QString::number(id.toInt()) ) );
}

void CitiesListModel::findCityErrorOccured(AbstractOpenWeathermapApiClient::ApiError err)
{
	if (err.type == AbstractOpenWeathermapApiClient::ResponseWithErrorCode ||
		err.type == AbstractOpenWeathermapApiClient::NetworkError) {
		if (err.errorCode == AbstractOpenWeathermapApiClient::HTTP_NOT_FOUND_CODE ||
		err.networkError == QNetworkReply::NetworkError::ContentNotFoundError) {
			clear();
			appendItem(Item(tr("Not found"), ""));

			m_ready = true;
			emit readyChanged();
			m_hasError = false;
			emit hasErrorChanged();
		}
	}

	m_ready = true;
	emit readyChanged();
	m_hasError = true;
	emit hasErrorChanged();
}

void CitiesListModel::clear()
{
	m_ready = false;
	emit readyChanged();
	m_hasError = false;
	emit hasErrorChanged();

	beginRemoveRows(QModelIndex(), 0, rowCount());
	m_items.clear();
	endRemoveRows();
}

void CitiesListModel::appendItem(const Item& item)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	m_items.append(item);
	endInsertRows();
}
