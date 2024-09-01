#include "hass.h"
#include <QDebug>

#define API "api"
#define SERVICES_ENDPOINT API "/services"
#define SERVICE_ACTION_ENDPOINT API "/services/%1/%2"
#define STATES_ENDPOINT API "/states"

Hass::Hass(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);

    const QSettings settings(QDir::homePath() + "/.config/asteroid-hass.conf", QSettings::IniFormat);
    m_host = QUrl(settings.value("HASS_HOST").toString());
    m_token = settings.value("HASS_TOKEN").toString();
    m_database = new Database(this);
    updateEntities();
}

QObject *Hass::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    static Hass instance;
    return &instance;
}

void Hass::Post(const QString &endpoint, const QString &data) const
{
    const QUrl url = m_host.resolved(endpoint);
    qDebug() << "Hass::Post:" << url.url() << data;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    const QByteArray headerData = "Bearer " + m_token.toUtf8();
    request.setRawHeader("Authorization", headerData);
    m_manager->post(request, data.toUtf8());
}

QNetworkReply *Hass::Get(const QString &endpoint) const
{
    const QUrl url = m_host.resolved(endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    const QByteArray headerData = "Bearer " + m_token.toUtf8();
    request.setRawHeader("Authorization", headerData);
    return m_manager->get(request);
}

void Hass::ParseServices(QNetworkReply *reply)
{
    qDebug() << "Parsing services";
    const QString answer = reply->readAll();
    const auto doc = QJsonDocument::fromJson(answer.toUtf8());

    if (doc.isNull())
    {
        qDebug() << "Not a valid document";
        return;
    }

    if (!doc.isArray())
    {
        qDebug() << "Unexpected data type";
        return;
    }

    auto obj = doc.array();

    for (const auto &text : obj)
    {
        auto service = text.toObject();
        qDebug() << service.value("domain");
        // qDebug() << text.toObject();
    }
}

void Hass::GetServices()
{
    const auto reply = Get(SERVICES_ENDPOINT);
    const auto connection = connect(reply, &QNetworkReply::finished, this, [=]() { ParseServices(reply); });
    Q_ASSERT(connection);
}

void Hass::ParseStates(QNetworkReply *reply)
{
    qDebug() << "Parsing states";
    QString answer = reply->readAll();
    auto doc = QJsonDocument::fromJson(answer.toUtf8());

    if (doc.isNull())
    {
        qDebug() << "Not a valid document";
        return;
    }

    if (!doc.isArray())
    {
        qDebug() << "Unexpected data type";
        return;
    }

    auto obj = doc.array();
    qDebug() << "Retreived all" << obj.count();

    for (const auto &text : obj)
    {
        auto service = text.toObject();
        auto attributes = service.value("attributes").toObject();
        auto entity_id = service.value("entity_id").toString();
        auto state = service.value("state").toString();
        auto entity_split = entity_id.split('.');
        const auto &domain = entity_split[0];
        const auto &id = entity_split[1];
        auto friendly_name = attributes.value("friendly_name").toString();

        if (state == "unavailable")
        {
            continue;
        }

        if (domain != "switch" && domain != "light" && domain != "button")
        {
            continue;
        }

        if (!m_database->exists(domain, id))
        {
            auto entity = Entity(domain, friendly_name, id, state, -1);
            m_database->upsertEntity(entity);
        }
        else
        {
            auto entity = m_database->getEntity(domain, id);
            entity.setName(friendly_name);
            entity.setState(state);
            m_database->upsertEntity(entity);
        }
    }
    updateEntities();
}

void Hass::GetStates()
{
    const auto reply = Get(STATES_ENDPOINT);
    const auto connection = connect(reply, &QNetworkReply::finished, this, [=]() { ParseStates(reply); });
    Q_ASSERT(connection);
}

void Hass::ToggleState(const QString &domain, const QString &id) const
{
    const auto data = QString(R"({"entity_id": "%1.%2"})").arg(domain, id);
    const auto endpoint = QString(SERVICE_ACTION_ENDPOINT).arg(domain, "toggle");
    Post(endpoint, data);
}

QObject *Hass::getModel()
{
    return &model;
}

void Hass::updateEntities()
{
    const auto entities = m_database->getAllEntities();
    model.clear();
    for (const auto &entity : *entities)
    {
        model.addFile(entity);
    }
}