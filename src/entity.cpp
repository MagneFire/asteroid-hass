#include "entity.h"

Entity::Entity(const QString &domain, const QString &name, const QString &id, const QString &state)
    : m_domain(domain)
    , m_name(name)
    , m_id(id)
    , m_state(state)
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