#ifndef ABSTRACTAPPSETTINGS_H
#define ABSTRACTAPPSETTINGS_H

#include <QObject>
#include <QVariant>
#include <QVector>

class AbstractAppSettings : public QObject
{
	Q_OBJECT
public:
	explicit AbstractAppSettings(QObject *parent = nullptr);
	virtual ~AbstractAppSettings() override {}

	virtual QVariant value(const QString& key) const = 0;

	Q_INVOKABLE virtual void beginSetValues() {}
	Q_INVOKABLE virtual void endSetValues() {}

public slots:
	virtual void setValue(const QString &key, const QVariant &value) = 0;

signals:
	void settingsChanged();

};

#endif // ABSTRACTAPPSETTINGS_H
