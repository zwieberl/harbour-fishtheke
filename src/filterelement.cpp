#include <src/filterelement.h>


FilterElement::FilterElement(const FilterElement &s)
{
    query = s.query;
    description = s.description;
    channel = s.channel;
    title = s.title;
    topic = s.topic;
}

QString FilterElement::getQuery() const
{
    return query;
}

void FilterElement::setQuery(const QString &value)
{
    query = value;
}

bool FilterElement::getTopic() const
{
    return topic;
}

void FilterElement::setTopic(bool value)
{
    topic = value;
}

bool FilterElement::getTitle() const
{
    return title;
}

void FilterElement::setTitle(bool value)
{
    title = value;
}

bool FilterElement::getDescription() const
{
    return description;
}

void FilterElement::setDescription(bool value)
{
    description = value;
}

bool FilterElement::getChannel() const
{
    return channel;
}

void FilterElement::setChannel(bool value)
{
    channel = value;
}
