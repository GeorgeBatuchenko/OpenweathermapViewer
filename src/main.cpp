#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

#include <QLocale>
#include <QTranslator>

#include "components/currentweather/currentweathermodel.h"
#include "components/dailyforecast/dailyforecastmodel.h"
#include "components/hourlyforecast/hourlyforecastmodel.h"
#include "pages/cities/citieslistmodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

	QLocale locale(QLocale("en_US"));
	QLocale::setDefault(locale);

	qRegisterMetaType<QJsonDocument>("QJsonDocument");
	qRegisterMetaType<QUrl>("QUrl");
	qmlRegisterType<CurrentWeatherModel>("OpenWeatherMapViewer", 1, 0, "CurrentWeatherModel");
	qmlRegisterType<DailyForecastModel>("OpenWeatherMapViewer", 1, 0, "DailyForecastModel");
	qmlRegisterType<HourlyForecastModel>("OpenWeatherMapViewer", 1, 0, "HourlyForecastModel");
	qmlRegisterType<CitiesListModel>("OpenWeatherMapViewer", 1, 0, "CitiesListModel");

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
