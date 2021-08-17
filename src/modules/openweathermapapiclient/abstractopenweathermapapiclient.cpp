#include "abstractopenweathermapapiclient.h"
#include <QString>
#include <QTextStream>

AbstractOpenWeathermapApiClient::AbstractOpenWeathermapApiClient(QObject *parent) : QObject(parent)
{

}

AbstractOpenWeathermapApiClient::~AbstractOpenWeathermapApiClient()
{

}

std::pair<AbstractOpenWeathermapApiClient::FieldError, QString>
AbstractOpenWeathermapApiClient::checkField(
	const QJsonValue& field,
	QJsonValue::Type targetType,
	const QString& path)
{
	QString errString;
	QTextStream stream(&errString);
	if (field.isUndefined()) {
		stream<<"Field "<<path<<" is not found in json.";

		return {FieldError::FieldNotFound, errString};
	}

	if (field.type() != targetType) {
		stream<<"Unexpected field type: "<<path;
		return {FieldError::UnexpectedFieldType, errString};
	}

	return {FieldError::NoError, "No error"};
}
