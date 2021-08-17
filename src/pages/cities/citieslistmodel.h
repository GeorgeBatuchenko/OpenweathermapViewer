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

	Q_PROPERTY(bool state READ state NOTIFY stateChanged)

public:
	enum CitiesRoles {
		IndexRole = Qt::UserRole + 1,
		NameRole,
		IdRole,
		LongitudeRole,
		LatitudeRole
	};

	enum State {
		Ready,
		Query,
		Error,
		NotFound
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

	State state() const {return m_state;}
	Q_INVOKABLE bool isReady() {return m_state == Ready;}
	Q_INVOKABLE bool isQuery() {return m_state == Query;}
	Q_INVOKABLE bool hasError() {return m_state == Error;}
	Q_INVOKABLE bool isNotFound() {return m_state == NotFound;}

public slots:
	void update(QString cityName);
	void saveItem(int index);

signals:
	void findCity(QString name, QString apiKey);
	void stateChanged();

private slots:
	void findCityFinished(QJsonDocument doc);
	void findCityErrorOccured(AbstractOpenWeathermapApiClient::ApiError err);

private:
	struct Item {
		Item(int _index, const QString& _name, int _id, double _lon, double _lat) :
			index(_index), name(_name), id(_id), lon(_lon), lat(_lat) {}
		int index;
		QString name;
		int id;
		double lon;
		double lat;
	};

	void clear();
	void appendItem(const QString& _name, int _id, double _lon, double _lat);

	bool checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path );

	QList<Item> m_items;
	AbstractOpenWeathermapApiClient* m_apiClient;
	AbstractAppSettings* m_appSettings;

	State m_state;
};

#endif // CITIESLISTMODEL_H
