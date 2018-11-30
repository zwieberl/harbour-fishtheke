#ifndef DATAFETCHER_H
#define DATAFETCHER_H

#include <QtGui>
#include <QObject>
#include "queryfilters.h"
//#include <QtDeclarative>
class QNetworkAccessManager;
class QNetworkReply;
class FilterElement;

class Datafetcher : public QAbstractListModel
{
 Q_OBJECT

 private:
    QNetworkAccessManager *manager;
    QJsonArray resultsArray;
    bool searching = false;
    int offset = 0;
    bool moreToLoad = true;

 public:
    QueryFilters queryFilters;

    explicit Datafetcher(QObject *parent = Q_NULLPTR);

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    Q_PROPERTY(bool moreToLoad READ getMoreToLoad)

    Q_INVOKABLE void search();
    Q_INVOKABLE void loadMore();
    Q_INVOKABLE bool isSearchInProgress();
    Q_INVOKABLE static QString seconds_to_DHMS(int duration);
    Q_INVOKABLE void reset();

    bool getMoreToLoad() const;

public slots:
    void handleQueryReply(QNetworkReply *reply);

 signals:
   void searchStatusChanged();
   void queryError(const QString &);

};

#endif // DATAFETCHER_H
