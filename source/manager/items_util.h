#ifndef ITEMS_UTIL_H
#define ITEMS_UTIL_H

#include <QString>

template <typename T> bool contains(T *items, QString name)
{
    for (auto item : *items)
        if (item->name == name)
            return true;

    return false;
}

template <typename T> void erase_by_name(T *items, QString name)
{
    int i = 0;
    for (auto item : *items)
    {
        if (item->name == name)
            break;

        i++;
    }

    items->erase(items->begin() + i, items->begin() + i + 1);
}

#endif // ITEMS_UTIL_H
