#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif
#include <QtQuick>
#include <sailfishapp.h>
#include "datafetcher.h"
#include "filterelement.h"
#include "queryfilters.h"
#include "apis/apibase.h"
#include "apis/mediathekview.h"

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

    // These should be part of APIBase, but QML can't handle this. So we have
    // to have it stand-alone
    QueryFilters queryFilters;
    ctxt->setContextProperty("queryFilters", &queryFilters);
    QObject::connect(&queryFilters, &QueryFilters::filtersChanged, &dataFetcher, &Datafetcher::handleFiltersChanged);

    qmlRegisterType<FilterElement>("fishtheke.filterelement", 1, 0, "FilterElement");
    qmlRegisterType<SortKey>("fishtheke.sortkey", 1, 0, "SortKey");
    qmlRegisterType<SortOrder>("fishtheke.sortorder", 1, 0, "SortOrder");
    qmlRegisterType<APIs>("fishtheke.apis", 1, 0, "APIs");

    qmlRegisterInterface<APIBase>("APIBase");

    view->setSource(SailfishApp::pathTo("qml/harbour-fishtheke.qml"));
    view->show();
    return app->exec();
}
