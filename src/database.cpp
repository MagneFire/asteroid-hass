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

bool Database::exists(const QString &domain, const QString &id)
{
    const auto iterator = findInModel(domain, id);
    return iterator != m_entities.end();
}

Entity Database::getEntity(const QString &domain, const QString &id)
{
    assert(exists(domain, id));
    const auto iterator = findInModel(domain, id);
    return *iterator;
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
        const auto in_dashboard = query.value(2).toInt();

        if (exists(domain, id))
        {
            qDebug() << "Fetching:Updating" << entity_id;
            removeFromModel(domain, id);
        }

        auto new_entity = Entity(domain, friendly_name, id, nullptr, in_dashboard);

        qDebug() << "Fetching:Adding" << entity_id;
        m_entities.append(new_entity);
    }
    sort();
}

QList<Entity> *Database::getAllEntities()
{
    return &m_entities;
}

void Database::removeEntity(const Entity &entity)
{
    const auto entityId = QString("%1.%2").arg(entity.domain(), entity.id());
    qDebug() << "Removing" << entityId;

    QSqlQuery query;
    query.prepare("DELETE FROM Entity WHERE id = :id");
    query.bindValue(":id", entityId);
    query.exec();

    const auto domain = entity.domain();
    const auto id = entity.id();
    removeFromModel(domain, id);
}

void Database::upsertEntity(const Entity &entity)
{
    const auto entityId = QString("%1.%2").arg(entity.domain(), entity.id());
    const auto exists1 = exists(entity.domain(), entity.id());
    const auto domain = entity.domain();
    const auto id = entity.id();
    qDebug() << (!exists1 ? "Adding" : "Updating") << entityId << entity.name() << entity.inDashboard();
    QSqlQuery query;
    if (!exists1)
    {
        query.prepare("INSERT INTO Entity (id, friendly_name, in_dashboard)"
                      "VALUES (:id, :friendly_name, :in_dashboard)");
    }
    else
    {
        query.prepare("UPDATE Entity SET friendly_name = :friendly_name, in_dashboard = :in_dashboard WHERE id = :id");

        removeFromModel(domain, id);
    }

    const auto new_entity = Entity(domain, entity.name(), id, nullptr, entity.inDashboard());

    query.bindValue(":id", entityId);
    query.bindValue(":friendly_name", entity.name());
    query.bindValue(":in_dashboard", entity.inDashboard());

    if (const auto result = query.exec(); !result)
    {
        qDebug() << "Error " << (!exists1 ? "adding" : "updating") << " entity" << entityId << ":" << query.lastError();
        return;
    }

    m_entities.append(new_entity);
    sort();
}

void Database::sort()
{
    std::sort(m_entities.begin(), m_entities.end(),
              [](const Entity &a, const Entity &b) { return a.inDashboard() > b.inDashboard(); });
}

QList<Entity>::iterator Database::findInModel(const QString &domain, const QString &id)
{
    const QList<Entity>::iterator iterator =
        std::find_if(m_entities.begin(), m_entities.end(),
                     [domain, id](const Entity &a) { return a.domain() == domain && a.id() == id; });
    return iterator;
}

void Database::removeFromModel(const QString &domain, const QString &id)
{
    m_entities.erase(findInModel(domain, id));
}
