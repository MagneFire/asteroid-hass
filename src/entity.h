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
    void setDomain(const QString &domain);
    void setName(const QString &name);
    void setId(const QString &id);
    void setState(bool state);
    void setInDashboard(bool inDashboard);

  private:
    QString m_domain;
    QString m_name;
    QString m_id;
    QString m_state;
    bool m_inDashboard;
};

#endif
