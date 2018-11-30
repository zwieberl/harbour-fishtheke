#include "datafetcher.h"
#include "filterelement.h"
#include <QtNetwork>

QString Datafetcher::seconds_to_DHMS(int duration)
{
  QString res;
  int seconds = (int) (duration % 60);
  duration /= 60;
  int minutes = (int) (duration % 60);
  duration /= 60;
  int hours = (int) (duration % 24);
  int days = (int) (duration / 24);
  if((hours == 0)&&(days == 0))
      return res.sprintf("%02d:%02d", minutes, seconds);
  if (days == 0)
      return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
  return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

void Datafetcher::reset()
{
    searching = false;
    resultsObject = {};
}

Datafetcher::Datafetcher(QObject *parent) : QAbstractListModel(parent)
{
    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, this, &Datafetcher::handleQueryReply);
}

int Datafetcher::rowCount(const QModelIndex &) const
{
    if (resultsObject.contains("results")) {
        return resultsObject["results"].toArray().size();
    } else {
        return 0;
    }
}

QVariant Datafetcher::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) {
        return QVariant();
    }
    if(role == Qt::DisplayRole) {
        const QJsonArray &results = resultsObject["results"].toArray();
        const QJsonObject &curr = results[index.row()].toObject();
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
    payload += "],\"size\":999}";
    qDebug() << "Searching: " << payload;

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain;charset=UTF-8"));
    request.setUrl(QUrl("https://mediathekviewweb.de/api/query"));
    searching = true;
    emit searchStatusChanged();
    manager->post(request, payload.toLatin1());
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

    beginResetModel();
    if (!jsonObject["err"].isNull()) {
        qDebug() << "Was an error!";
        emit queryError(jsonObject["err"].toString());
    } else {
        resultsObject = jsonObject["result"].toObject();
        emit searchStatusChanged();
    }
    endResetModel();
}
