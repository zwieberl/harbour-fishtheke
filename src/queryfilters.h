#ifndef QUERYFILTERS_H
#define QUERYFILTERS_H

#include <QObject>
#include <QtGui>
#include <memory>
#include "filterelement.h"

class QueryFilters : public QAbstractListModel
{
     Q_OBJECT

     private:
        QList <std::shared_ptr<FilterElement>> filters;

     public:
        explicit QueryFilters(QObject *parent = Q_NULLPTR) : QAbstractListModel(parent) {}
        virtual ~QueryFilters();
        virtual int rowCount(const QModelIndex&) const;
        virtual QVariant data(const QModelIndex &index, int role) const;
        Q_INVOKABLE virtual bool removeRow(int row, const QModelIndex &parent);
        const QList<std::shared_ptr<FilterElement>>& getFilters() {return filters;}
        Q_INVOKABLE void addFilter(FilterElement *s);
        Q_INVOKABLE void clearAllFilters();
        Q_INVOKABLE FilterElement* createNewFilter();

     signals:
       void filtersChanged(const QList <std::shared_ptr<FilterElement>> &filters);
};

#endif // QUERYFILTERS_H
