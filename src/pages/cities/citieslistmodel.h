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

	Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
	Q_PROPERTY(bool hasError READ hasError NOTIFY hasErrorChanged)

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

	bool ready() const {return m_ready;}
	bool hasError() const {return m_hasError;}

public slots:
	void update(QString cityName);

signals:
	void findCity(QString name, QString apiKey);
	void readyChanged();
	void hasErrorChanged();

private slots:
	void findCityFinished(QJsonDocument doc);
	void findCityErrorOccured(AbstractOpenWeathermapApiClient::ApiError err);

private:
	struct Item {
		Item(const QString& _name, const QString& _id) :
			name(_name), id(_id) {}
		QString name;
		QString id;
	};

	void clear();
	void appendItem(const Item& item);

	bool checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path );

	QList<Item> m_items;
	AbstractOpenWeathermapApiClient* m_apiClient;
	AbstractAppSettings* m_appSettings;

	bool m_ready;
	bool m_hasError;
};

#endif // CITIESLISTMODEL_H
