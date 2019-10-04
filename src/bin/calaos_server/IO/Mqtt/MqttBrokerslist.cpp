/******************************************************************************
 **  Copyright (c) 2006-2019, Calaos. All Rights Reserved.
 **
 **  This file is part of Calaos.
 **
 **  Calaos is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Calaos is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Foobar; if not, write to the Free Software
 **  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 **
 ******************************************************************************/
#include "MqttBrokersList.h"

using namespace Calaos;

MqttBrokersList::MqttBrokersList()
{
}

MqttClient *MqttBrokersList::get_client(const Params &p)
{
    string key, broker = "127.0.0.1", port = "1883";

    if (p.Exists("broker"))
        broker = p["broker"];
    if (p.Exists("port"))
        port = p["port"];

    /* Compute the key per broker */
    key = p["host"] + ":" + port;

    if (hashBrokers.find(key) != hashBrokers.end())
        return hashBrokers[key];

    hashBrokers[key] = new MqttClient(p);

    return hashBrokers[key];
}