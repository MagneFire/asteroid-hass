#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>

class Entity
{
public:
    Entity(const QString &domain, const QString &name, const QString &id, const QString &state);
    virtual ~Entity() {};

    QString domain() const;
    QString name() const;
    QString id() const;
    QString state() const;

private:
    const QString m_domain;
    const QString m_name;
    const QString m_id;
    const QString m_state;
};

#endif
