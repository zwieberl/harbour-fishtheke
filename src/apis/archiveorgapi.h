#ifndef ARCHIVEORGAPI_H
#define ARCHIVEORGAPI_H
#include <QtGui>
#include <QObject>
#include "apibase.h"

class QNetworkAccessManager;
class QNetworkReply;
class FilterElement;


class ArchiveOrgAPI : public APIBase
{
public:
    explicit ArchiveOrgAPI(QObject *parent = Q_NULLPTR);

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual void search();
    virtual void loadMore();

public slots:
    void handleQueryReply(QNetworkReply *reply);
};

#endif // ARCHIVEORGAPI_H
