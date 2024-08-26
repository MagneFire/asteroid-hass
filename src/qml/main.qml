/*
 * Copyright (C) 2024 Darrel Griët <idanlcontact@gmail.com>
 *               2022 Timo Könnecke <github.com/eLtMosen>
 *               2019 Florent Revest <revestflo@gmail.com>
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

import QtQuick 2.9
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import org.asteroid.hass 1.0
import org.nemomobile.systemsettings 1.0
import Nemo.KeepAlive 1.1

Application {
    Hass { id: hass }
    centerColor: "#00A698"
    outerColor: "#000C07"
    id: app

    Component.onCompleted: {
        DisplayBlanking.preventBlanking = true
        hass.GetStates()
    }

    Component {
        id: spacer
        Item {
            height: app.height * (DeviceInfo.hasRoundScreen ? 0.06 : 0.02)
            width: height
        }
    }

    ListView {
        id: romBrowser
        anchors.fill: parent
        header: spacer
        footer: spacer

        model: hass.getModel()
        delegate: Entity {
            clip: false
            title: name
            iconName: domain == "button" ? "ios-radio-button-on" :
                      domain == "light" ? "ios-bulb-outline":
                      domain == "switch" ? "ios-switch-outline":
                      "";
            onClicked: {
                console.log("Click " + domain + "." + id)
                hass.ToggleState(domain, id);
            }
        }
    }
}
