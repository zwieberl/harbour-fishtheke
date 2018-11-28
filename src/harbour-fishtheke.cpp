#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif
#include <QtQuick>
#include <sailfishapp.h>
#include "datafetcher.h"
#include "filterelement.h"
#include "queryfilters.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/fishtheke.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    QScopedPointer<QQuickView> view(SailfishApp::createView());
    QQmlContext *ctxt = view.data()->rootContext();

    Datafetcher dataFetcher;
    ctxt->setContextProperty("datafetcher", &dataFetcher);
    ctxt->setContextProperty("queryFilters", &dataFetcher.queryFilters);

    qmlRegisterType<FilterElement>("fishtheke.filterelement", 1, 0, "FilterElement");

    view->setSource(SailfishApp::pathTo("qml/harbour-fishtheke.qml"));
    view->show();
    return app->exec();
}
