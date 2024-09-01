#include "database.h"

#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

Database::Database(QObject *parent) : QObject(parent)
{
    connect();
    setup();
    fetchEntities();
}

void Database::connect()
{
    const QString driver = "QSQLITE";
    const QString database_path = QDir::homePath() + "/.config/asteroid-hass.db";
    if (!QSqlDatabase::isDriverAvailable(driver))
    {
        qWarning() << "sqlite driver is not available";
        return;
    }

    auto db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(database_path);
    if (!db.open())
    {
        qWarning() << "Error opening database" << db.lastError().text();
    }
}

void Database::setup()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS Entity (id text primary key, friendly_name text, in_dashboard integer);");
}

Entity *Database::getEntity(const QString &domain, const QString &id)
{
    const QList<Entity *>::iterator entity =
        std::find_if(m_entities.begin(), m_entities.end(),
                     [domain, id](const Entity *const a) { return a->domain() == domain && a->id() == id; });
    if (entity == m_entities.end())
    {
        return nullptr;
    }

    return *entity;
}

void Database::fetchEntities()
{
    QSqlQuery query;
    query.exec("SELECT id, friendly_name, in_dashboard FROM Entity");
    while (query.next())
    {
        auto entity_id = query.value(0).toString();
        auto entity_split = entity_id.split('.');
        const auto &domain = entity_split[0];
        const auto &id = entity_split[1];
        const auto friendly_name = query.value(1).toString();
        const auto in_dashboard = query.value(2).toBool();

        if (const auto entity = getEntity(domain, id); entity == nullptr)
        {
            auto new_entity = new Entity(domain, friendly_name, id, nullptr, in_dashboard);

            qDebug() << "Fetching:Adding" << entity_id;
            m_entities.append(new_entity);
        }
        else
        {
            qDebug() << "Fetching:Updating" << entity_id;
            entity->setDomain(domain);
            entity->setId(id);
            entity->setName(friendly_name);
            entity->setInDashboard(in_dashboard);
        }
    }
}

QList<Entity *> *Database::getAllEntities()
{
    return &m_entities;
}

void Database::removeEntity(const Entity *entity)
{
    const auto entityId = QString("%1.%2").arg(entity->domain(), entity->id());
    qDebug() << "Removing" << entityId;

    QSqlQuery query;
    query.prepare("DELETE FROM Entity WHERE id = :id");
    query.bindValue(":id", entityId);
    query.exec();

    auto domain = entity->domain();
    auto id = entity->id();
    m_entities.erase(
        std::remove_if(m_entities.begin(), m_entities.end(),
                       [domain, id](const Entity *const a) { return a->domain() == domain && a->id() == id; }),
        m_entities.end());
}

void Database::updateOrInsertEntity(const Entity *entity)
{
    const auto entityId = QString("%1.%2").arg(entity->domain(), entity->id());
    const auto existing_entity = getEntity(entity->domain(), entity->id());
    qDebug() << (existing_entity == nullptr ? "Adding" : "Updating") << entityId << entity->name()
             << entity->inDashboard();
    QSqlQuery query;
    if (existing_entity == nullptr)
    {
        query.prepare("INSERT INTO Entity (id, friendly_name, in_dashboard)"
                      "VALUES (:id, :friendly_name, :in_dashboard)");
    }
    else
    {
        query.prepare("UPDATE Entity SET friendly_name = :friendly_name, in_dashboard = :in_dashboard WHERE id = :id");
        existing_entity->setName(entity->name());
        existing_entity->setInDashboard(entity->inDashboard());
    }

    query.bindValue(":id", entityId);
    query.bindValue(":friendly_name", entity->name());
    query.bindValue(":in_dashboard", entity->inDashboard());

    if (const auto result = query.exec(); !result)
    {
        qDebug() << "Error " << (existing_entity == nullptr ? "adding" : "updating") << " entity" << entityId << ":"
                 << query.lastError();
    }
}
