#include <QtTest/QTest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include "components/currentweather/currentweathermodel.h"
#include "components/openweathermapapiclient/abstractopenweathermapapiclient.h"

static QByteArray jsonData;

class ApiClientPositiveMoc : public AbstractOpenWeathermapApiClient {
	Q_OBJECT
public:
	ApiClientPositiveMoc() : AbstractOpenWeathermapApiClient(nullptr) {}
	~ApiClientPositiveMoc() override {}

public slots:
	void weatherByCityId(QString id, QString apiKey, QString lang) override {
		Q_UNUSED(id);
		Q_UNUSED(apiKey);
		Q_UNUSED(lang);
		auto doc = QJsonDocument::fromJson(jsonData);
		emit weatherByCityIdFetched(doc);
	}
};

class ApiClientBadJsonMoc : public AbstractOpenWeathermapApiClient {
	Q_OBJECT
public:
	ApiClientBadJsonMoc() : AbstractOpenWeathermapApiClient(nullptr) {}
	~ApiClientBadJsonMoc() override {}

public slots:
	void weatherByCityId(QString id, QString apiKey, QString lang) override {
		Q_UNUSED(id);
		Q_UNUSED(apiKey);
		Q_UNUSED(lang);
		auto data = jsonData.mid(0, jsonData.size()-10);
		auto doc = QJsonDocument::fromJson(data);
		emit weatherByCityIdFetched(doc);
	}
};

class ApiClientBadFieldMoc : public AbstractOpenWeathermapApiClient {
	Q_OBJECT
public:
	ApiClientBadFieldMoc() : AbstractOpenWeathermapApiClient(nullptr) {
		m_doc = QJsonDocument::fromJson(jsonData);
	}
	~ApiClientBadFieldMoc() override {}

	template <class T>
	void setName(const T& val) {
		auto obj = m_doc.object();
		if (std::is_same<T, QJsonArray>() || std::is_same<T, QJsonObject>())
			obj.insert("name", val);
		else
			obj.insert("name", QJsonValue(val));
		m_doc.setObject(obj);
	}

	void removeName() {
		auto obj = m_doc.object();
		obj.remove("name");
		m_doc.setObject(obj);
	}

public slots:
	void weatherByCityId(QString id, QString apiKey, QString lang) override {
		Q_UNUSED(id);
		Q_UNUSED(apiKey);
		Q_UNUSED(lang);
		emit weatherByCityIdFetched(m_doc);
	}

private:
	QJsonDocument m_doc;
};

class CurrentWeatherModelTest : public QObject {
	Q_OBJECT
private slots:
	void initTestCase() {
		qRegisterMetaType<QJsonDocument>("QJsonDocument");

		QFile file(DATA_DIR"/api_response.json");
		QVERIFY(file.open(QIODevice::ReadOnly));
		jsonData = file.readAll();
	}

	void test_updatePositive() {
		ApiClientPositiveMoc apimoc;
		CurrentWeatherModel model(&apimoc, &apimoc);
		model.update();

		QVERIFY(model.ready());
		QVERIFY(!model.hasError());
		QCOMPARE(model.city(), "Moscow");
		QCOMPARE(model.iconFilePath(), "http://openweathermap.org/img/wn/01d@2x.png");
		QCOMPARE(model.temperature(), 16.8);
		QCOMPARE(model.humidity(), 69);
		QCOMPARE(model.windblow(), "5.4m/s, ");
		QCOMPARE(model.description(), "Feels like 16°C. Clear sky");
	}

	void test_updateWithBadJson() {
		ApiClientBadJsonMoc apimoc;
		CurrentWeatherModel model(&apimoc, &apimoc);
		model.update();

		QVERIFY(model.ready());
		QVERIFY(model.hasError());
	}

	void test_updateWithBadField() {
		ApiClientBadFieldMoc apimoc;
		CurrentWeatherModel model(&apimoc, &apimoc);

		model.update();
		QVERIFY(model.ready());
		QVERIFY(!model.hasError());

		apimoc.setName(1.0);
		model.update();
		QVERIFY(model.ready());
		QVERIFY(model.hasError());

		apimoc.setName(true);
		model.update();
		QVERIFY(model.ready());
		QVERIFY(model.hasError());

		apimoc.setName(QJsonArray());
		model.update();
		QVERIFY(model.ready());
		QVERIFY(model.hasError());

		apimoc.setName(QJsonObject());
		model.update();
		QVERIFY(model.ready());
		QVERIFY(model.hasError());

		apimoc.removeName();
		model.update();
		QVERIFY(model.ready());
		QVERIFY(model.hasError());

		apimoc.setName(QString("Berlin"));
		model.update();
		QVERIFY(model.ready());
		QVERIFY(!model.hasError());
	}

private:
};

QTEST_MAIN(CurrentWeatherModelTest)

#include "main.moc"
