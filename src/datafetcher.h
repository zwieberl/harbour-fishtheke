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
    int searchBlockSize = 25;
    QString sortedBy = "timestamp";
    QString sortOrder = "desc";
    bool future = false;

 public:
    QueryFilters queryFilters;

    explicit Datafetcher(QObject *parent = Q_NULLPTR);

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    Q_PROPERTY(bool moreToLoad READ getMoreToLoad)
    Q_PROPERTY(int searchBlockSize READ getSearchBlockSize WRITE setSearchBlockSize)
    Q_PROPERTY(QString sortedBy READ getSortedBy WRITE setSortedBy)
    Q_PROPERTY(QString sortOrder READ getSortOrder WRITE setSortOrder)
    Q_PROPERTY(bool future READ getFuture WRITE setFuture)

    Q_INVOKABLE void search();
    Q_INVOKABLE void loadMore();
    Q_INVOKABLE bool isSearchInProgress();
    Q_INVOKABLE void reset();

    bool getMoreToLoad() const;

    int getSearchBlockSize() const;
    void setSearchBlockSize(int value);

    QString getSortedBy() const;
    void setSortedBy(const QString &value);

    QString getSortOrder() const;
    void setSortOrder(const QString &value);

    bool getFuture() const;
    void setFuture(bool value);

public slots:
    void handleQueryReply(QNetworkReply *reply);

signals:
   void searchStatusChanged();
   void queryError(const QString &);

};

#endif // DATAFETCHER_H
