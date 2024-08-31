#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>

class Entity
{
  public:
    Entity(const QString &domain, const QString &name, const QString &id, const QString &state, const bool inDashboard);
    virtual ~Entity() = default;

    [[nodiscard]] QString domain() const;
    [[nodiscard]] QString name() const;
    [[nodiscard]] QString id() const;
    [[nodiscard]] QString state() const;
    [[nodiscard]] bool inDashboard() const;

  private:
    const QString m_domain;
    const QString m_name;
    const QString m_id;
    const QString m_state;
    const bool m_inDashboard;
};

#endif
