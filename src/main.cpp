#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QLocale>
#include <QTranslator>

#include "components/currentweather/currentweathermodel.h"
#include "components/openweathermapapiclient/openweathermapapiclient.h"

//
#include <QUrlQuery>
#include <QSettings>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	qRegisterMetaType<QJsonDocument>("QJsonDocument");
	qRegisterMetaType<QUrl>("QUrl");
	qmlRegisterType<CurrentWeatherModel>("OpenWeatherMapViewer", 1, 0, "CurrentWeatherModel");

	QGuiApplication app(argc, argv);

	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	for (const QString &locale : uiLanguages) {
		const QString baseName = "OpenwethermapViewer_" + QLocale(locale).name();
		if (translator.load(":/i18n/" + baseName)) {
			app.installTranslator(&translator);
			break;
		}
	}

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/forms/src/MainWindow.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
	&app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.load(url);

//	OpenWeatherMapApiClient* client = OpenWeatherMapApiClient::instance();
//	client->weatherByCityId("524901","d3025cfd37cbd1ae614d1f41de1f40d9","EN");

	QSettings sett("app.ini");
	sett.setValue("value", "avalue");
	sett.sync();

	return app.exec();
}
