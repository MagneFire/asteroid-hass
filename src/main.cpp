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
 */

#include <QGuiApplication>
#include <QQuickView>
#include <QScreen>
#include <asteroidapp.h>

#include "hass.h"

void setupPaths() {
    if (QFile::exists(QDir::homePath() + "/.config/asteroid-hass.conf")) return;

    QDir().mkpath(QDir::homePath() + "/.config");

    QSettings settings(QDir::homePath() + "/.config/asteroid-hass.conf", QSettings::IniFormat);
    settings.setValue("HASS_HOST", "");
    settings.setValue("HASS_TOKEN", "");
    settings.sync();
    if(settings.status() == QSettings::NoError) {
        qDebug() << "Config file was properly setup.";
    } else {
        qDebug() << "Error while creating config file " << settings.status();
    }
}

int main(int argc, char *argv[])
{
    setupPaths();

    QScopedPointer<QGuiApplication> app(AsteroidApp::application(argc, argv));
    QScopedPointer<QQuickView> view(AsteroidApp::createView());
    qmlRegisterSingletonType<Hass>("org.asteroid.hass", 1, 0, "Hass", &Hass::qmlInstance);
    view->setSource(QUrl("qrc:/qml/main.qml"));
    view->resize(app->primaryScreen()->size());
    view->show();
    return app->exec();
}
