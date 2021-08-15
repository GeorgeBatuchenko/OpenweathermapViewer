#ifndef ABSTRACTOPENWEATHERMAPAPICLIENT_H
#define ABSTRACTOPENWEATHERMAPAPICLIENT_H

#include <QObject>
#include <QJsonDocument>
#include <QUrl>

class AbstractOpenWeathermapApiClient : public QObject
{
	Q_OBJECT
public:
	explicit AbstractOpenWeathermapApiClient(QObject *parent = nullptr);
	virtual ~AbstractOpenWeathermapApiClient() {}

public slots:
	virtual void weatherByCityId(QString id, QString apiKey, QString lang) = 0;
signals:
	void weatherByCityIdFetched(QJsonDocument);
	void apiResponseErrorDetected(QUrl, QJsonDocument);
	void networkErrorOccured(QUrl, QString);

};

#endif // ABSTRACTOPENWEATHERMAPAPICLIENT_H
