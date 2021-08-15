#ifndef ABSTRACTAPPSETTINGS_H
#define ABSTRACTAPPSETTINGS_H

#include <QObject>
#include <QVariant>

class AbstractAppSettings : public QObject
{
	Q_OBJECT
public:
	explicit AbstractAppSettings(QObject *parent = nullptr);
	virtual ~AbstractAppSettings() override {}

	virtual QVariant value(const QString& key) const = 0;

public slots:
	virtual void setValue(const QString &key, const QVariant &value) = 0;

signals:

};

#endif // ABSTRACTAPPSETTINGS_H
