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

QString Datafetcher::getSortedBy() const
{
    return sortedBy;
}

void Datafetcher::setSortedBy(const QString &value)
{
    sortedBy = value;
}

QString Datafetcher::getSortOrder() const
{
    return sortOrder;
}

void Datafetcher::setSortOrder(const QString &value)
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
    QList<QString> filter;
    for (auto& elem : queryFilters.getFilters()) {
        QString filterstring;
        filterstring += "{\"fields\":[";
        QList<QString> fields;

        if (elem->getTitle()) {
            fields.push_back("\"title\"");
        }
        if (elem->getChannel()) {
            fields.push_back("\"channel\"");
        }
        if (elem->getDescription()) {
            fields.push_back("\"description\"");
        }
        if (elem->getTopic()) {
            fields.push_back("\"topic\"");
        }
        filterstring += fields.join(',');
        filterstring += "],\"query\":\"" + elem->getQuery() + "\"}";

        filter.push_back(filterstring);
    }
    QString payload = "{\"queries\":[";
    payload += filter.join(',');
    payload += "],\"size\":" + QString::number(searchBlockSize)
              + ",\"offset\":" + QString::number(offset)
              + ",\"sortBy\":\"" + sortedBy + "\""
              + ",\"sortOrder\":\"" + sortOrder + "\"";
    if (future) {
        payload += ",\"future\":true";
    }
    payload += "}";
    qDebug() << "Searching: " << payload;

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain;charset=UTF-8"));
    request.setUrl(QUrl("https://mediathekviewweb.de/api/query"));
    searching = true;
    emit searchStatusChanged();
    manager->post(request, payload.toLatin1());
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
}
