#include "entity.h"

Entity::Entity(const QString &domain, const QString &name, const QString &id, const QString &state, int inDashboard)
    : m_domain(domain), m_name(name), m_id(id), m_state(state), m_inDashboard(inDashboard)
{
}

QString Entity::domain() const
{
    return m_domain;
}

QString Entity::name() const
{
    return m_name;
}

QString Entity::id() const
{
    return m_id;
}

QString Entity::state() const
{
    return m_state;
}

int Entity::inDashboard() const
{
    return m_inDashboard;
}

void Entity::setDomain(const QString &domain)
{
    m_domain = domain;
}

void Entity::setName(const QString &name)
{
    m_name = name;
}

void Entity::setId(const QString &id)
{
    m_id = id;
}

void Entity::setState(const QString &state)
{
    m_state = state;
}

void Entity::setInDashboard(const int inDashboard)
{
    m_inDashboard = inDashboard;
}
