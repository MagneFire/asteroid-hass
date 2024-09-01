/*
 * Copyright (C) 2024 - Darrel Griët <idanlcontact@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Based on a code from nemo-qml-plugin-systemsettings under the following license.
 *
 * Copyright (C) 2013 Jolla Ltd. <pekka.vuorela@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef HASS_H
#define HASS_H

#include "database.h"
#include "entity_model.h"
#include <QObject>
#include <QtQml>

class Hass : public QObject
{
    Q_OBJECT

    explicit Hass(QObject *parent = nullptr);

  public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    void Post(const QString &endpoint, const QString &data) const;
    QNetworkReply *Get(const QString &endpoint) const;
    Q_INVOKABLE void GetServices();
    Q_INVOKABLE void GetStates();

    void ParseServices(QNetworkReply *);
    void ParseStates(QNetworkReply *);
    Q_INVOKABLE void ToggleState(const QString &domain, const QString &id) const;
    Q_INVOKABLE void EnableInDashboard(const QString &domain, const QString &id);
    Q_INVOKABLE void DisableInDashboard(const QString &domain, const QString &id);

    Q_INVOKABLE QObject *getModel();
    Q_INVOKABLE QObject *getDashboardModel();

  private:
    void updateEntities();
    Database *m_database;
    QNetworkAccessManager *m_manager;
    QUrl m_host;
    QString m_token;
    EntityModel model;
    EntityModel dashboard_model;
};

#endif
