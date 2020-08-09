#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "PreviewFilter.h"
#include "PreviewImageProvider.h"
#include "DetectorOutputTypeEnum.h"

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	qmlRegisterType<PreviewFilter>("com.videoSynopsisGui.classes", 1, 0, "PreviewFilter");
	qmlRegisterType<PreviewImageProvider>("com.videoSynopsisGui.classes", 1, 0, "PreviewImageProvider");


	QQmlApplicationEngine engine;
	PreviewImageProvider* imageProvider = new PreviewImageProvider;

	QStringList outputTypeList;
	for(auto && type : DetectorOutputTypeEnum::toArray()){
		outputTypeList.append(QString::fromStdString(type.toString()));
	}

    engine.rootContext()->setContextProperty("outputTypeList", outputTypeList);
	engine.rootContext()->setContextProperty("imageProvider", imageProvider);
	engine.addImageProvider(QLatin1String("previewprovider"), imageProvider);
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
			&app, [url](QObject* obj, const QUrl& objUrl)
			{
				if (!obj && url == objUrl)
				{
					QCoreApplication::exit(-1);
				}
			}, Qt::QueuedConnection);
	engine.load(url);

	return app.exec();
}
