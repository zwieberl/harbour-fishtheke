#ifndef MEDIATHEKVIEWAPI_H
#define MEDIATHEKVIEWAPI_H

#include <QtGui>
#include <QObject>
#include "apibase.h"

class QNetworkAccessManager;
class QNetworkReply;
class FilterElement;

class MediathekviewAPI : public APIBase
{
 Q_OBJECT

 private:

 public:
    explicit MediathekviewAPI(QObject *parent = Q_NULLPTR);

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual void search();
    virtual void loadMore();

public slots:
    void handleQueryReply(QNetworkReply *reply);
};

#endif // MEDIATHEKVIEWAPI_H
