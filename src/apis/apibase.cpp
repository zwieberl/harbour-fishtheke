#include "apibase.h"
#include "src/filterelement.h"
#include <QtNetwork>

void APIBase::reset()
{
    searching = false;
    beginResetModel();
    resultsArray = {};
    offset = 0;
    moreToLoad = true;
    endResetModel();
}

void APIBase::setQueryFilters(const QList <std::shared_ptr<FilterElement>> &filters)
{
    queryFilters = filters;
}

bool APIBase::getMoreToLoad() const
{
    return moreToLoad;
}

int APIBase::getSearchBlockSize() const
{
    return searchBlockSize;
}

void APIBase::setSearchBlockSize(int value)
{
    searchBlockSize = value;
}

SortKey::EnumSortKey APIBase::getSortedBy() const
{
    return sortedBy;
}

void APIBase::setSortedBy(const SortKey::EnumSortKey &value)
{
    sortedBy = value;
}

SortOrder::EnumSortOrder APIBase::getSortOrder() const
{
    return sortOrder;
}

void APIBase::setSortOrder(const SortOrder::EnumSortOrder &value)
{
    sortOrder = value;
}

bool APIBase::getFuture() const
{
    return future;
}

void APIBase::setFuture(bool value)
{
    future = value;
}

APIBase::APIBase(QObject *parent) : QAbstractListModel(parent)
{
    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, this, &APIBase::handleQueryReply);
}

bool APIBase::isSearchInProgress()
{
    return searching;
}
