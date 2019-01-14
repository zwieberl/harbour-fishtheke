#include "mediathekview.h"
#include <QtNetwork>

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

MediathekviewAPI::MediathekviewAPI(QObject *parent) : APIBase(parent)
{
    // nothing
}

int MediathekviewAPI::rowCount(const QModelIndex &) const
{
    return resultsArray.size();
}

QVariant MediathekviewAPI::data(const QModelIndex &index, int role) const
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

void MediathekviewAPI::search()
{
    QJsonArray filters;
    for (auto& elem : queryFilters) {
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

void MediathekviewAPI::loadMore()
{
    offset += searchBlockSize;
    search();
}

void MediathekviewAPI::handleQueryReply(QNetworkReply *reply)
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
