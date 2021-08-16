#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

#include <QLocale>
#include <QTranslator>

#include "modules/appsettings/appsettings.h"
#include "components/currentweather/currentweathermodel.h"
#include "pages/cities/citieslistmodel.h"

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
	qmlRegisterType<CitiesListModel>("OpenWeatherMapViewer", 1, 0, "CitiesListModel");
	qmlRegisterSingletonType<AppSettings>("OpenWeatherMapViewer", 1, 0, "AppSettings", &AppSettings::instance);

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

	return app.exec();
}
