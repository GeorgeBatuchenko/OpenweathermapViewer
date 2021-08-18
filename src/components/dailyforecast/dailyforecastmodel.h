#ifndef DAILYFORECAST_H
#define DAILYFORECAST_H

#include <QAbstractListModel>
#include <QJsonDocument>
#include <QList>

#include "../../modules/openweathermapapiclient/openweathermapapiclient.h"
#include "../../modules/appsettings/appsettings.h"


class DailyForecastModel : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(bool state READ state NOTIFY stateChanged)

public:
	enum DailyForecastRoles {
		DateRole = Qt::UserRole + 1,
		IconUrlRole,
		TempetatureRole,
		DescriptionRole,
	};

	enum State {
		Ready,
		Query,
		Error,
		NotFound
	};

	explicit DailyForecastModel(
		QObject* parent=nullptr,
		AbstractOpenWeathermapApiClient* apiClient = OpenWeatherMapApiClient::instance(),
		AbstractAppSettings* appSettings = AppSettings::instance()
	);
	virtual ~DailyForecastModel() override;

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
	void queryDaylyForecast(double lon, double lat, QString apiKey, QString lang);
	void stateChanged();

private slots:
	void dailyForecastFinished(QJsonDocument doc);
	void dailyForecastOccured(AbstractOpenWeathermapApiClient::ApiError err);

private:
	struct Item {
		Item (
			const QString& _date,
			const QString& _iconUrlString,
			int _temperature,
			const QString& _description
		) : date(_date), iconUrlString(_iconUrlString),
			temperature(_temperature), description(_description){}
		QString date;
		QString iconUrlString;
		int temperature;
		QString description;
	};

	void clear();

	bool checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path );

	QList<Item> m_items;
	AbstractOpenWeathermapApiClient* m_apiClient;
	AbstractAppSettings* m_appSettings;

	State m_state;
};

#endif // DAILYFORECAST_H
