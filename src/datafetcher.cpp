#include "datafetcher.h"
#include "filterelement.h"
#include "apis/mediathekview.h"
#include "apis/archiveorgapi.h"
#include "apis/apibase.h"
#include <QtNetwork>

APIs::EnumAPIs Datafetcher::getCurrentAPI() const
{
    return currentAPI;
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

void Datafetcher::setCurrentAPI(const APIs::EnumAPIs &value)
{
    APIs::EnumAPIs old = currentAPI;
    currentAPI = value;
    if (old != value) {
        apis[currentAPI]->setSortedBy(apis[old]->getSortedBy());
        apis[currentAPI]->setSortOrder(apis[old]->getSortOrder());
        apis[currentAPI]->setQueryFilters(apis[old]->getQueryFilters());
        emit apiChanged();
    }
}

void Datafetcher::handleFiltersChanged(const QList<std::shared_ptr<FilterElement> > &filters)
{
    apis[currentAPI]->setQueryFilters(filters);
}

Datafetcher::Datafetcher() : QObject()
{
    apis = {
            {APIs::EnumAPIs::ARCHIVE_ORG,         std::make_shared<ArchiveOrgAPI>(this)},
            {APIs::EnumAPIs::MEDIATHEKVIEWWEB_DE, std::make_shared<MediathekviewAPI>(this)},
    };
}

APIBase *Datafetcher::getCurrentAPIObject()
{
    return apis[currentAPI].get();
}
