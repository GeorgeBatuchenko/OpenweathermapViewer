#ifndef HOURLYFORECAST_H
#define HOURLYFORECAST_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QList>

#include "../../modules/openweathermapapiclient/openweathermapapiclient.h"
#include "../../modules/appsettings/appsettings.h"


class HourlyForecastModel : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(bool state READ state NOTIFY stateChanged)

public:
	enum DailyForecastRoles {
		TimeRole = Qt::UserRole + 1,
		IconUrlRole,
		TempetatureRole
	};

	enum State {
		Ready,
		Query,
		Error,
		NotFound
	};

	explicit HourlyForecastModel(
		QObject* parent=nullptr,
		AbstractOpenWeathermapApiClient* apiClient = OpenWeatherMapApiClient::instance(),
		AbstractAppSettings* appSettings = AppSettings::instance()
	);
	virtual ~HourlyForecastModel() override;

	int	rowCount(const QModelIndex &parent = QModelIndex()) const override final;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;

	QHash<int, QByteArray> roleNames() const override final;

	State state() const {return m_state;}
	Q_INVOKABLE bool isReady() {return m_state == Ready;}
	Q_INVOKABLE bool isQuery() {return m_state == Query;}
	Q_INVOKABLE bool hasError() {return m_state == Error;}
	Q_INVOKABLE bool isNotFound() {return m_state == NotFound;}

public slots:
	void update();

signals:
	void queryHourlyForecast(double lon, double lat, QString apiKey, QString lang);
	void stateChanged();

private slots:
	void hourlyForecastFinished(QJsonDocument doc);
	void hourlyForecastErrorOccured(AbstractOpenWeathermapApiClient::ApiError err);

private:
	struct Item {
		Item (
			const QString& _time,
			const QString& _iconUrlString,
			int _temperature
		) : time(_time), iconUrlString(_iconUrlString),
			temperature(_temperature){}
		QString time;
		QString iconUrlString;
		int temperature;
	};

	void clear();

	bool checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path );

	QList<Item> m_items;
	AbstractOpenWeathermapApiClient* m_apiClient;
	AbstractAppSettings* m_appSettings;

	State m_state;
};

#endif // HOURLYFORECAST_H
