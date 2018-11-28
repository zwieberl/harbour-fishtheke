#include "queryfilters.h"
#include <memory>

QueryFilters::~QueryFilters()
{
    clearAllFilters();
}

int QueryFilters::rowCount(const QModelIndex &) const
{
    return filters.count();
}

QVariant QueryFilters::data(const QModelIndex &index, int role) const
{
    qDebug() << "model index = " << index.row();

    if(!index.isValid() || role != Qt::DisplayRole || index.row() >= filters.length()) {
        return QVariant();
    }
    QVariant res;
    res.setValue(filters.at(index.row()).get());
    return res;
}

bool QueryFilters::removeRow(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    filters.removeAt(row);
    endRemoveRows();
    emit filtersChanged();
    return true;
}

void QueryFilters::addFilter(FilterElement *s)
{
    qDebug() << "Added filter for: " << s->getQuery();
    int id = filters.length();
    beginInsertRows(QModelIndex(), id, id);
    filters.push_back(std::make_shared<FilterElement>(*s));
    endInsertRows();

    emit filtersChanged();
}

void QueryFilters::clearAllFilters()
{
    beginResetModel();
    filters.clear();
    endResetModel();
    emit filtersChanged();
}

FilterElement* QueryFilters::createNewFilter()
{
    return new FilterElement{};
}
