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
    bool exists(const QString &domain, const QString &id);
    Entity getEntity(const QString &domain, const QString &id);
    void fetchEntities();
    QList<Entity> *getAllEntities();
    void removeEntity(const Entity &entity);
    void upsertEntity(const Entity &entity);

  private:
    QList<Entity>::iterator findInModel(const QString &domain, const QString &id);
    void removeFromModel(const QString &domain, const QString &id);
    QList<Entity> m_entities;
};

#endif
