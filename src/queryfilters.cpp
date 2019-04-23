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
    emit filtersChanged(filters);
    return true;
}

void QueryFilters::addFilter(FilterElement *s)
{
    qDebug() << "Added filter for: " << s->getQuery();
    int id = filters.length();
    beginInsertRows(QModelIndex(), id, id);
    filters.push_back(std::make_shared<FilterElement>(*s));
    endInsertRows();

    emit filtersChanged(filters);
}

void QueryFilters::clearAllFilters()
{
    beginResetModel();
    filters.clear();
    endResetModel();
    emit filtersChanged(filters);
}

FilterElement* QueryFilters::createNewFilter()
{
    return new FilterElement{};
}

void QueryFilters::update(const QModelIndex &/*index*/)
{
    // It should be possible to simply update the index,
    // but this does not work. So I currently reset the whole model
    //    emit dataChanged(index, index);
    beginResetModel();
    endResetModel();
    emit filtersChanged(filters);
}
