#ifndef ITEMS_UTIL_H
#define ITEMS_UTIL_H

#include <QString>

template <typename T> bool is_struct(T *lista, QString name)
{
    for (auto s : *lista)
        if (s->name == name)
            return true;

    return false;
}

template <typename T> void erase_by_name(T *lista, QString name)
{
    int i = 0;
    for (auto s : *lista)
    {
        if (s->name == name)
            break;

        i++;
    }

    lista->erase(lista->begin() + i, lista->begin() + i + 1);
}

#endif // ITEMS_UTIL_H
