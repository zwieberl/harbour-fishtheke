#ifndef DATAFETCHER_H
#define DATAFETCHER_H

#include <QtGui>
#include <QObject>
#include "queryfilters.h"
#include <map>
#include <memory>

class QNetworkAccessManager;
class QNetworkReply;
class FilterElement;
class APIBase;

// Wrapper-class needed to access Enum in QML
class APIs: public QObject
{
    Q_OBJECT
    public:
    APIs() : QObject() {}
    enum EnumAPIs
    {
        MEDIATHEKVIEWWEB_DE,
        ARCHIVE_ORG
    };
    Q_ENUMS(EnumAPIs)
};

class Datafetcher : public QObject
{
 Q_OBJECT

 private:
    std::map<APIs::EnumAPIs, std::shared_ptr<APIBase>> apis;
    APIs::EnumAPIs currentAPI = APIs::EnumAPIs::MEDIATHEKVIEWWEB_DE;
//    APIs::EnumAPIs currentAPI = APIs::EnumAPIs::ARCHIVE_ORG;
 public:
    explicit Datafetcher();
    Q_PROPERTY(APIs::EnumAPIs currentAPI READ getCurrentAPI WRITE setCurrentAPI)

    Q_INVOKABLE bool fileExists(QString file);
    Q_INVOKABLE bool runExternalCommand(const QString &cmd);
    Q_INVOKABLE APIBase* getCurrentAPIObject();

    APIs::EnumAPIs getCurrentAPI() const;
    void setCurrentAPI(const APIs::EnumAPIs &value);

public slots:
    void handleFiltersChanged(const QList <std::shared_ptr<FilterElement>> &filters);

signals:
    void apiChanged();
};

#endif // DATAFETCHER_H
