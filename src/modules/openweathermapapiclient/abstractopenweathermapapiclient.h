#ifndef ABSTRACTOPENWEATHERMAPAPICLIENT_H
#define ABSTRACTOPENWEATHERMAPAPICLIENT_H

#include <QObject>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QUrl>

class AbstractOpenWeathermapApiClient : public QObject
{
	Q_OBJECT
public:
	enum class ApiErrorType {
		NoError = 0,
		NetworkError,
		EmptyResponseBody,
		BrokenResponseBody,
		BadJsonStructure,
		ResponseWithErrorCode
	};

	enum class FieldError {
		NoError = 0,
		FieldNotFound,
		UnexpectedFieldType
	};

	struct ApiError {
		ApiError(
			ApiErrorType _type, int _errorCode,
			QNetworkReply::NetworkError _networkError,
			const QUrl& _url, const QString& _errorString
		) :
			type(_type),
			errorCode(_errorCode),
			networkError(_networkError),
			url(_url),
			errorString(_errorString) {}

		ApiErrorType type;
		int errorCode;
		QNetworkReply::NetworkError networkError;
		QUrl url;
		QString errorString;
	};

	static constexpr int HTTP_OK_CODE = 200;
	static constexpr int HTTP_NOT_FOUND_CODE = 400;

	static const QString WEATHER_URL_STRING;
	static const QString FORECAST_URL_STRING;
	static const QString ICON_URL_STRING;

	explicit AbstractOpenWeathermapApiClient(QObject *parent = nullptr);
	virtual ~AbstractOpenWeathermapApiClient();

	static std::pair<FieldError, QString> checkField(const QJsonValue& field, QJsonValue::Type targetType, const QString& path);
	static QString makeHugeIconUrlString(const QString& icon);
	static QString makeNormalIconUrlString(const QString& icon);

public slots:
	virtual void weatherByCityId(QString id, QString apiKey, QString lang) = 0;
	virtual void findCity(QString name, QString apiKey) = 0;
	virtual void dailyForecast(double lon, double lat, QString apiKey, QString lang) = 0;
	virtual void hourlyForecast(double lon, double lat, QString apiKey, QString lang) = 0;
signals:
	void weatherByCityIdFetched(QJsonDocument);
	void weatherByCityIdErrorOccured(const ApiError);

	void findCityFinished(QJsonDocument);
	void findCityErrorOccured(const ApiError);

	void dailyForecastFetched(QJsonDocument);
	void dailyForecastErrorOccured(const ApiError);

	void hourlyForecastFetched(QJsonDocument);
	void hourlyForecastErrorOccured(const ApiError);

};

#endif // ABSTRACTOPENWEATHERMAPAPICLIENT_H
