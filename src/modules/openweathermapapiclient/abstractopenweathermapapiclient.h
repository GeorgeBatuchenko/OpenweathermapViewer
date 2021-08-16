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
	enum ApiErrorType {
		NoError = 0,
		NetworkError,
		EmptyResponseBody,
		BrokenResponseBody,
		BadJsonStructure,
		ResponseWithErrorCode
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

	explicit AbstractOpenWeathermapApiClient(QObject *parent = nullptr);
	virtual ~AbstractOpenWeathermapApiClient();

public slots:
	virtual void weatherByCityId(QString id, QString apiKey, QString lang) = 0;
	virtual void findCity(QString name, QString apiKey) = 0;
signals:
	void weatherByCityIdFetched(QJsonDocument);
	void findCityFinished(QJsonDocument);

	void apiErrorOccured(const ApiError);

};

#endif // ABSTRACTOPENWEATHERMAPAPICLIENT_H
