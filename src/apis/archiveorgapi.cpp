#include "archiveorgapi.h"
#include <QtNetwork>

static QString sortedByString(SortKey::EnumSortKey sortedBy)
{
    switch (sortedBy) {
    case SortKey::CHANNEL:
        return "publisher";
        break;
    case SortKey::DURATION:
        return "item_size"; // We can't sort according to runtime, sadly
        break;
    default:
    case SortKey::TIMESTAMP:
        return "date";
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

int ArchiveOrgAPI::rowCount(const QModelIndex &) const
{
    return resultsArray.size();
}

QVariant ArchiveOrgAPI::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }
    if(role == Qt::DisplayRole) {
        const QJsonObject &curr = resultsArray[index.row()].toObject();
        QMap<QString,QVariant> resultMap;

        resultMap.insert("title",       curr["title"].toString());
        resultMap.insert("description", curr["description"].toString() + "\n" + curr["runtime"].toString());
        resultMap.insert("duration",    QTime(0,0).secsTo(QTime::fromString(curr["runtime"].toString(), Qt::ISODate)));
        resultMap.insert("channel",     curr["publisher"].toString());
        resultMap.insert("timestamp",   QDateTime::fromString(curr["date"].toString(), Qt::ISODate).toMSecsSinceEpoch() / 1000);
        resultMap.insert("url_website", "https://archive.org/details/" + curr["identifier"].toString());

        return QVariant(resultMap);
    }
    return QVariant();
}

void ArchiveOrgAPI::search()
{
    QStringList titles;
    QStringList publishers;
    QStringList descriptions;
    QStringList collections;
    QStringList mediatypes;

    for (auto& elem : queryFilters) {
        QString searchQuery = elem->getQuery();
        if (elem->getTitle()) {
            titles.append("title:" + searchQuery);
        }
        if (elem->getChannel()) {
            publishers.append("publisher:" + searchQuery);
        }
        if (elem->getDescription()) {
            descriptions.append("description:" + searchQuery);
        }
        if (elem->getTopic()) {
            collections.append("collection:" + searchQuery);
        }
    }
    mediatypes.append("mediatype:movies"); // For now only videos

    QStringList filters;

    if (!titles.empty())       filters.append(titles.join("+AND+"));
    if (!publishers.empty())   filters.append(publishers.join("+AND+"));
    if (!descriptions.empty()) filters.append(descriptions.join("+AND+"));
    if (!collections.empty())  filters.append(collections.join("+AND+"));
    if (!mediatypes.empty())   filters.append(mediatypes.join("+AND+"));

    QStringList returnKeys = {"date", "description", "format", "identifier",
                             "mediatype", "publisher", "title", "runtime"};

    QUrl url("https://archive.org/advancedsearch.php");

    QUrlQuery urlquery;
    for (auto &key : returnKeys) {
        urlquery.addQueryItem("fl[]", key);
    }
    urlquery.addQueryItem("rows", QString::number(searchBlockSize));
    urlquery.addQueryItem("q", filters.join(",+"));
    urlquery.addQueryItem("page", QString::number(offset));
    urlquery.addQueryItem("output", "json");
    urlquery.addQueryItem("sort[]", sortedByString(sortedBy) + "+" + sortOrderString(sortOrder));

    url.setQuery(urlquery.query());

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded;charset=UTF-8"));
    request.setRawHeader("Accept", "application/json");
    searching = true;
    emit searchStatusChanged();
    qDebug() << url;
    manager->get(request);
}

void ArchiveOrgAPI::loadMore()
{
    ++offset;
    search();
}

void ArchiveOrgAPI::handleQueryReply(QNetworkReply *reply)
{
    qDebug() << "Got a reply!";
    searching = false;
    if (reply->error()) {
        qDebug() << "Was an network-error: " << reply->errorString();
        emit queryError(reply->errorString());
        return;
    }

    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
//    qDebug() << jsonResponse.toJson();
    QJsonObject jsonObject = jsonResponse.object()["response"].toObject();

    QJsonArray arr = jsonObject["docs"].toArray();

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

    reply->deleteLater();
}

ArchiveOrgAPI::ArchiveOrgAPI(QObject *parent) : APIBase(parent)
{
    offset = 1; // page starts counting at 1
}
