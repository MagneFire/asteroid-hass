#include "hass.h"
#include <QDebug>

#define API "api"
#define SERVICES_ENDPOINT API"/services"
#define SERVICE_ACTION_ENDPOINT API"/services/%1/%2"
#define STATES_ENDPOINT API"/states"

Hass::Hass(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);

    QSettings settings(QDir::homePath() + "/.config/asteroid-hass.conf", QSettings::IniFormat);
    m_host = QUrl(settings.value("HASS_HOST").toString());
    m_token = settings.value("HASS_TOKEN").toString();
}

QObject * Hass::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    static Hass instance;
    return &instance;
}

void Hass::Post(QString endpoint, QString data)
{
    QUrl url = m_host.resolved(endpoint);
    qDebug() << "Hass::Post:" << url.url() << data;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QByteArray headerData = "Bearer " + m_token.toUtf8();
    request.setRawHeader("Authorization", headerData);
    manager->post(request, data.toUtf8());
}

QNetworkReply *Hass::Get(QString endpoint)
{
    QUrl url = m_host.resolved(endpoint);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QByteArray headerData = "Bearer " + m_token.toUtf8();
    request.setRawHeader("Authorization", headerData);
    return manager->get(request);
}

void Hass::ParseServices(QNetworkReply * reply) {
    qDebug() << "Parsing services";
    QString answer = reply->readAll();
    auto doc = QJsonDocument::fromJson(answer.toUtf8());

    if(doc.isNull())
    {
        qDebug() << "Not a valid document";
        return;
    }

    if(!doc.isArray())
    {
        qDebug() << "Unexpected data type";
        return;
    }

    auto obj = doc.array();

    for(const auto& text : obj) {
        auto service = text.toObject();
        qDebug() << service.value("domain");
        // qDebug() << text.toObject();
    }
}

void Hass::GetServices() {
    auto reply = Get(SERVICES_ENDPOINT);
    connect(reply, &QNetworkReply::finished, this, [=]() {ParseServices(reply);});
}

void Hass::ParseStates(QNetworkReply * reply) {
    qDebug() << "Parsing states";
    QString answer = reply->readAll();
    auto doc = QJsonDocument::fromJson(answer.toUtf8());

    if(doc.isNull())
    {
        qDebug() << "Not a valid document";
        return;
    }

    if(!doc.isArray())
    {
        qDebug() << "Unexpected data type";
        return;
    }

    auto obj = doc.array();

    for(const auto& text : obj) {
        auto service = text.toObject();
        auto attributes = service.value("attributes").toObject();
        auto entity = service.value("entity_id").toString();
        auto state = service.value("state").toString();
        auto entity_split = entity.split('.');
        auto domain = entity_split[0];
        auto id = entity_split[1];
        auto friendly_name = attributes.value("friendly_name").toString();

        if (state == "unavailable") continue;

        if (domain != "switch" && domain != "light" && domain != "button") {
            continue;
        }

        qDebug() << domain << id << friendly_name << state;
        model.addFile(Entity(domain, friendly_name, id, state));
    }
}

void Hass::GetStates() {
    auto reply = Get(STATES_ENDPOINT);
    connect(reply, &QNetworkReply::finished, this, [=]() {ParseStates(reply);});
}

void Hass::ToggleState(QString domain, QString id) {
    auto PIStr = QString("{\"entity_id\": \"%1.%2\"}").arg(domain, id);
    auto endpoint = QString(SERVICE_ACTION_ENDPOINT).arg(domain, "toggle");
    Post(endpoint, PIStr);
}

QObject* Hass::getModel() {
    return &model;
}