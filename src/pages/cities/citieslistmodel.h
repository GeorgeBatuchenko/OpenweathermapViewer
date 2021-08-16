#ifndef CITIESLISTMODEL_H
#define CITIESLISTMODEL_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QList>

#include "../../modules/openweathermapapiclient/openweathermapapiclient.h"
#include "../../modules/appsettings/appsettings.h"

class CitiesListModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum CitiesRoles {
		NameRole = Qt::UserRole + 1,
		IdRole
	};

	explicit CitiesListModel(
		QObject* parent=nullptr,
		AbstractOpenWeathermapApiClient* apiClient = OpenWeatherMapApiClient::instance(),
		AbstractAppSettings* appSettings = AppSettings::instance()
	);
	virtual ~CitiesListModel() override;

	int	rowCount(const QModelIndex &parent = QModelIndex()) const override final;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;

	QHash<int, QByteArray> roleNames() const override final;

public slots:
	void update(QString cityName);
	void findCityFinished(QJsonDocument doc);
	void findCityErrorOccured(AbstractOpenWeathermapApiClient::ApiError err);
signals:
	void findCity(QString name, QString apiKey);

private:
	struct Item {
		Item(const QString& _name, const QString& _id) :
			name(_name), id(_id) {}
		QString name;
		QString id;
	};

	void clear();
	void appendItem(const Item& item);

	QList<Item> m_items;
	AbstractOpenWeathermapApiClient* m_apiClient;
	AbstractAppSettings* m_appSettings;
};

#endif // CITIESLISTMODEL_H
