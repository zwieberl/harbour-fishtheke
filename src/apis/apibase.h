#ifndef APIBASE_H
#define APIBASE_H

#include <QtGui>
#include <QObject>
#include "src/queryfilters.h"
//#include <QtDeclarative>
class QNetworkAccessManager;
class QNetworkReply;
class FilterElement;
class Datafetcher;

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

class APIBase : public QAbstractListModel
{
 Q_OBJECT

 friend class Datafetcher;

 protected:
    QNetworkAccessManager *manager;
    QJsonArray resultsArray;
    bool searching = false;
    int offset = 0;
    bool moreToLoad = true;
    int searchBlockSize = 25;
    SortKey::EnumSortKey sortedBy = SortKey::TIMESTAMP;
    SortOrder::EnumSortOrder sortOrder = SortOrder::DESC;
    bool future = false;
    QList <std::shared_ptr<FilterElement>> queryFilters;

 public:
    explicit APIBase(QObject *parent = Q_NULLPTR);

    Q_PROPERTY(bool moreToLoad READ getMoreToLoad)
    Q_PROPERTY(int searchBlockSize READ getSearchBlockSize WRITE setSearchBlockSize)
    Q_PROPERTY(SortKey::EnumSortKey sortedBy READ getSortedBy WRITE setSortedBy)
    Q_PROPERTY(SortOrder::EnumSortOrder sortOrder READ getSortOrder WRITE setSortOrder)
    Q_PROPERTY(bool future READ getFuture WRITE setFuture)

    virtual int rowCount(const QModelIndex&) const = 0;
    virtual QVariant data(const QModelIndex &index, int role) const = 0;
    Q_INVOKABLE virtual void search() = 0;
    Q_INVOKABLE virtual void loadMore() = 0;

    Q_INVOKABLE bool isSearchInProgress();
    Q_INVOKABLE void reset();
    QList<std::shared_ptr<FilterElement> > getQueryFilters() const;
    Q_INVOKABLE void setQueryFilters(const QList <std::shared_ptr<FilterElement>> &filters);

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
    virtual void handleQueryReply(QNetworkReply *reply) = 0;

signals:
   void searchStatusChanged();
   void queryError(const QString &);
};

#endif // APIBASE_H
