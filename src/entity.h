#ifndef ENTITY_H
#define ENTITY_H

#include <QObject>

class Entity
{
  public:
    Entity(const QString &domain, const QString &name, const QString &id, const QString &state, int inDashboard);
    virtual ~Entity() = default;

    [[nodiscard]] QString domain() const;
    [[nodiscard]] QString name() const;
    [[nodiscard]] QString id() const;
    [[nodiscard]] QString state() const;
    [[nodiscard]] int inDashboard() const;
    void setDomain(const QString &domain);
    void setName(const QString &name);
    void setId(const QString &id);
    void setState(const QString &state);
    void setInDashboard(int inDashboard);

  private:
    QString m_domain;
    QString m_name;
    QString m_id;
    QString m_state;
    int m_inDashboard;
};

#endif
