#ifndef DATAFETCHER_H
#define DATAFETCHER_H

#include <QtGui>
#include <QObject>
#include "queryfilters.h"
//#include <QtDeclarative>
class QNetworkAccessManager;
class QNetworkReply;
class FilterElement;

// Wrapper-class needed to access Enum in QML
class SortOrder : public QObject
{
    Q_OBJECT
    public:
    SortOrder() : QObject() {}
    enum EnumSortOrder
    {
        DESC,
        ASC
    };
    Q_ENUMS(EnumSortOrder)
};

class SortKey : public QObject
{
    Q_OBJECT
    public:
    SortKey() : QObject() {}
    enum EnumSortKey
    {
        TIMESTAMP,
        CHANNEL,
        DURATION
    };
    Q_ENUMS(EnumSortKey)
};

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
    SortKey::EnumSortKey sortedBy = SortKey::TIMESTAMP;
    SortOrder::EnumSortOrder sortOrder = SortOrder::DESC;
    bool future = false;

 public:
    QueryFilters queryFilters;

    explicit Datafetcher(QObject *parent = Q_NULLPTR);

    virtual int rowCount(const QModelIndex&) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    Q_PROPERTY(bool moreToLoad READ getMoreToLoad)
    Q_PROPERTY(int searchBlockSize READ getSearchBlockSize WRITE setSearchBlockSize)
    Q_PROPERTY(SortKey::EnumSortKey sortedBy READ getSortedBy WRITE setSortedBy)
    Q_PROPERTY(SortOrder::EnumSortOrder sortOrder READ getSortOrder WRITE setSortOrder)
    Q_PROPERTY(bool future READ getFuture WRITE setFuture)

    Q_INVOKABLE void search();
    Q_INVOKABLE void loadMore();
    Q_INVOKABLE bool isSearchInProgress();
    Q_INVOKABLE void reset();
    Q_INVOKABLE bool fileExists(QString file);

    bool getMoreToLoad() const;

    int getSearchBlockSize() const;
    void setSearchBlockSize(int value);

    SortKey::EnumSortKey getSortedBy() const;
    void setSortedBy(const SortKey::EnumSortKey &value);

    SortOrder::EnumSortOrder getSortOrder() const;
    void setSortOrder(const SortOrder::EnumSortOrder &value);

    bool getFuture() const;
    void setFuture(bool value);

public slots:
    void handleQueryReply(QNetworkReply *reply);

signals:
   void searchStatusChanged();
   void queryError(const QString &);
};

#endif // DATAFETCHER_H
