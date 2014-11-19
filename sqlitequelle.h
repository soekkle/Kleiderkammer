#ifndef SQLITEQUELLE_H
#define SQLITEQUELLE_H

#include "datenquelle.h"
#include <iostream>
#include <QtSql>

class SQLiteQuelle : public DatenQuelle
{
public:
    SQLiteQuelle(QString Pfad);
private:
    QSqlDatabase Datenbank;

    void createDB();
};

#endif // SQLITEQUELLE_H
