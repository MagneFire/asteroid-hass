#ifndef DATABASE_H
#define DATABASE_H

#include "entity.h"

#include <QObject>

class Database : public QObject
{
    Q_OBJECT

  public:
    explicit Database(QObject *parent = nullptr);
    void connect();
    void setup();
    Entity *getEntity(const QString &domain, const QString &id);
    void fetchEntities();
    QList<Entity *> *getAllEntities();
    void removeEntity(const Entity *entity);
    void updateOrInsertEntity(const Entity *entity);

  private:
    QList<Entity *> m_entities;
};

#endif
