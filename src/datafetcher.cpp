#include "datafetcher.h"
#include "filterelement.h"
#include <QtNetwork>

void Datafetcher::reset()
{
    searching = false;
    beginResetModel();
    resultsArray = {};
    offset = 0;
    moreToLoad = true;
    endResetModel();
}

bool Datafetcher::fileExists(QString file)
{
    return QFile(file).exists();
}

bool Datafetcher::runExternalCommand(const QString &cmd)
{
    qDebug() << "Trying to start " + cmd;
    QProcess *proc = new QProcess();
    return proc->startDetached(cmd);
}

bool Datafetcher::getMoreToLoad() const
{
    return moreToLoad;
}

int Datafetcher::getSearchBlockSize() const
{
    return searchBlockSize;
}

void Datafetcher::setSearchBlockSize(int value)
{
    searchBlockSize = value;
}

SortKey::EnumSortKey Datafetcher::getSortedBy() const
{
    return sortedBy;
}

void Datafetcher::setSortedBy(const SortKey::EnumSortKey &value)
{
    sortedBy = value;
}

SortOrder::EnumSortOrder Datafetcher::getSortOrder() const
{
    return sortOrder;
}

void Datafetcher::setSortOrder(const SortOrder::EnumSortOrder &value)
{
    sortOrder = value;
}

bool Datafetcher::getFuture() const
{
    return future;
}

void Datafetcher::setFuture(bool value)
{
    future = value;
}

static QString sortedByString(SortKey::EnumSortKey sortedBy)
{
    switch (sortedBy) {
    case SortKey::CHANNEL:
        return "channel";
        break;
    case SortKey::DURATION:
        return "duration";
        break;
    default:
    case SortKey::TIMESTAMP:
        return "timestamp";
        break;
    }
}

static QString sortOrderString(SortOrder::EnumSortOrder sortOrder)
{
    switch (sortOrder) {
    case SortOrder::ASC:
        return "asc";
        break;
    case SortOrder::DESC:
    default:
        return "desc";
        break;
    }
}

Datafetcher::Datafetcher(QObject *parent) : QAbstractListModel(parent)
{
    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, this, &Datafetcher::handleQueryReply);
}

int Datafetcher::rowCount(const QModelIndex &) const
{
    return resultsArray.size();
}

QVariant Datafetcher::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }
    if(role == Qt::DisplayRole) {
        const QJsonObject &curr = resultsArray[index.row()].toObject();
        QMap<QString,QVariant> resultMap;
        for (auto &key : curr.keys()) {
            switch (curr[key].type()) {
                case QJsonValue::Type::Array:
                case QJsonValue::Type::Bool:
                case QJsonValue::Type::Object:
                case QJsonValue::Type::Null:
                case QJsonValue::Type::Undefined:
                    continue;

                case QJsonValue::Type::Double:
                    resultMap.insert(key, QVariant(curr[key].toDouble()));
                    break;
                case QJsonValue::Type::String:
                    resultMap.insert(key, QVariant(curr[key].toString()));
                    break;
            }
        }
        return QVariant(resultMap);
    }
    return QVariant();

}

void Datafetcher::search()
{   
    QJsonArray filters;
    for (auto& elem : queryFilters.getFilters()) {
        QJsonObject filter;
        QJsonArray fields;

        if (elem->getTitle()) {
            fields.append("title");
        }
        if (elem->getChannel()) {
            fields.append("channel");
        }
        if (elem->getDescription()) {
            fields.append("description");
        }
        if (elem->getTopic()) {
            fields.append("topic");
        }
        filter.insert("fields", fields);
        filter.insert("query", elem->getQuery());

        filters.append(filter);
    }
    QJsonObject query;
    query.insert("queries", filters);
    query.insert("size", searchBlockSize);
    query.insert("offset", offset);
    query.insert("sortBy", sortedByString(sortedBy));
    query.insert("sortOrder", sortOrderString(sortOrder));
    if (future) {
        query.insert("future", true);
    }

    qDebug() << "Searching: " << QJsonDocument(query).toJson(QJsonDocument::JsonFormat::Compact);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain;charset=UTF-8"));
    request.setUrl(QUrl("https://mediathekviewweb.de/api/query"));
    searching = true;
    emit searchStatusChanged();
    manager->post(request, QJsonDocument(query).toJson(QJsonDocument::JsonFormat::Compact));
}

void Datafetcher::loadMore()
{
    offset += searchBlockSize;
    search();
}

bool Datafetcher::isSearchInProgress()
{
    return searching;
}

void Datafetcher::handleQueryReply(QNetworkReply *reply)
{
    qDebug() << "Got a reply!";
    searching = false;
    if (reply->error()) {
        qDebug() << "Was an network-error: " << reply->errorString();
        emit queryError(reply->errorString());
        return;
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonObject = jsonResponse.object();

    if (!jsonObject["err"].isNull()) {
        qDebug() << "Was an error!";
        emit queryError(jsonObject["err"].toString());
    } else {
        QJsonArray arr = jsonObject["result"].toObject()["results"].toArray();
        int numOfResults = arr.size();
        if (numOfResults < searchBlockSize) {
            moreToLoad = false;
        }
        emit searchStatusChanged();
        for (auto elem : arr) {
            beginInsertRows(QModelIndex(), resultsArray.size(), resultsArray.size());
            resultsArray += elem;
            endInsertRows();
        }
    }

    reply->deleteLater();
}
