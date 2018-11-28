#ifndef SEARCHELEMENT_H
#define SEARCHELEMENT_H

#include <QObject>

class FilterElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString query READ getQuery WRITE setQuery NOTIFY dummy)
    Q_PROPERTY(bool channel READ getChannel WRITE setChannel NOTIFY dummy)
    Q_PROPERTY(bool topic READ getTopic WRITE setTopic NOTIFY dummy)
    Q_PROPERTY(bool title READ getTitle WRITE setTitle NOTIFY dummy)
    Q_PROPERTY(bool description READ getDescription WRITE setDescription NOTIFY dummy)

public:
    FilterElement() = default;
    FilterElement(const FilterElement &);
    QString getQuery() const;
    void setQuery(const QString &value);

    bool getChannel() const;
    void setChannel(bool value);

    bool getTopic() const;
    void setTopic(bool value);

    bool getTitle() const;
    void setTitle(bool value);

    bool getDescription() const;
    void setDescription(bool value);
signals:
    void dummy();

private:
    QString query;
    bool description = false;
    bool channel = false;
    bool title = false;
    bool topic = false;
};
// Registering it for use in QVariant
Q_DECLARE_METATYPE(FilterElement)

#endif // SEARCHELEMENT_H
