/******************************************************************************
 **  Copyright (c) 2006-2014, Calaos. All Rights Reserved.
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
#include <OutputLightRGB.h>
#include <IPC.h>

using namespace Calaos;

OutputLightRGB::OutputLightRGB(Params &_p):
    Output(_p),
    timer_auto(NULL)
{
    set_param("gui_type", "light_rgb");
    if (!get_params().Exists("visible")) set_param("visible", "true");

    cInfoDom("output") << "OutputLightRGB::OutputLightRGB(" << get_param("id") << "): Ok";
}

OutputLightRGB::~OutputLightRGB()
{
    cInfoDom("output") << "OutputLightRGB::~OutputLightRGB(): Ok";
}

/* List of actions where value is in percent
**  set <value>
**  on
**  off
**  toggle
*/
bool OutputLightRGB::set_value(std::string val)
{
    bool ret = true;

    cInfoDom("output") << "OutputLightRGB(" << get_param("id") << "): got action, " << val;

    if (val == "on" || val == "true")
    {
        //switch the light on only if it was off
        if (!state)
        {
            setColor(color, true);
            cmd_state = "on";
            state = true;
        }

        DELETE_NULL(timer_auto);
    }
    else if (val == "off" || val == "false")
    {
        //switch the light off only if it was on
        if (state)
        {
            setColor(ColorValue::fromRgb(0, 0, 0), false);
            cmd_state = "off";
            state = false;
        }

        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 8, "set off ") == 0)
    {
        val.erase(0, 4);

        ColorValue c(val);
        if (c.isValid())
        {
            color = c;
            if (state)
                setColor(color, state);

            cmd_state = "set off " + color.toString();

            DELETE_NULL(timer_auto);
        }
    }
    else if (val == "toggle")
    {
        set_value(!state);
    }
    else if (val.compare(0, 4, "set ") == 0)
    {
        val.erase(0, 4);

        ColorValue c(val);
        if (c.isValid())
        {
            color = c;
            state = true;

            setColor(color, state);

            cmd_state = "set " + color.toString();

            DELETE_NULL(timer_auto);
        }
    }
    else if (val.compare(0, 8, "set_red ") == 0)
    {
        val.erase(0, 8);
        int percent;
        from_string(val, percent);

        color.setRed(double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 10, "set_green ") == 0)
    {
        val.erase(0, 10);
        int percent;
        from_string(val, percent);

        color.setGreen(double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 9, "set_blue ") == 0)
    {
        val.erase(0, 9);
        int percent;
        from_string(val, percent);

        color.setBlue(double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 7, "up_red ") == 0)
    {
        val.erase(0, 7);
        int percent;
        from_string(val, percent);

        color.setRed(color.getRed() + double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 9, "down_red ") == 0)
    {
        val.erase(0, 9);
        int percent;
        from_string(val, percent);

        color.setRed(color.getRed() - double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 9, "up_green ") == 0)
    {
        val.erase(0, 9);
        int percent;
        from_string(val, percent);

        color.setGreen(color.getGreen() + double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 11, "down_green ") == 0)
    {
        val.erase(0, 11);
        int percent;
        from_string(val, percent);

        color.setGreen(color.getGreen() - double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 8, "up_blue ") == 0)
    {
        val.erase(0, 8);
        int percent;
        from_string(val, percent);

        color.setBlue(color.getBlue() + double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 10, "down_blue ") == 0)
    {
        val.erase(0, 10);
        int percent;
        from_string(val, percent);

        color.setBlue(color.getBlue() - double(percent) * 255. / 100.);
        state = true;
        setColor(color, state);
        DELETE_NULL(timer_auto);
    }
    else if (val.compare(0, 12, "auto_change ") == 0)
    {
        val.erase(0, 12);
        int timems;
        from_string(val, timems);

        DELETE_NULL(timer_auto);
        timer_auto = new EcoreTimer((double)timems / 1000.,
                                    (sigc::slot<void>)sigc::mem_fun(*this, &OutputLightRGB::TimerAutoChange) );
        //Directly start the first time
        TimerAutoChange();
    }

    return ret;
}

void OutputLightRGB::emitChange()
{
    int v = state?((color.getRed() << 16) +
            (color.getGreen() << 8) +
            color.getBlue()):0;

    string sig = "output ";
    sig += get_param("id") + " ";
    sig += Utils::url_encode(string("state:") + Utils::to_string(v));
    IPC::Instance().SendEvent("events", sig);
}

void OutputLightRGB::setColor(const ColorValue &c, bool s)
{
    cmd_state = "set " + color.toString();

    setColorReal(c, s);

    EmitSignalOutput();
    emitChange();
}

void OutputLightRGB::TimerAutoChange()
{
    //choose a random color
    setColor(ColorValue::fromRgb(rand() % 255, rand() % 255, rand() % 255), true);
}

void OutputLightRGB::stateUpdated(const ColorValue &c, bool s)
{
    color = c;
    state = s;
    cmd_state = "set " + get_value_string();

    EmitSignalOutput();
    emitChange();
}

bool OutputLightRGB::check_condition_value(std::string cvalue, bool equal)
{
    if (cvalue == "on" || cvalue == "true")
    {
        if ((equal && state) ||
            (!equal && !state))
            return true;
    }
    else if (cvalue == "off" || cvalue == "false")
    {
        if ((!equal && state) ||
            (equal && !state))
            return true;
    }
    else
    {
        ColorValue c(cvalue);
        if ((equal && c == color) ||
            (!equal && c != color))
            return true;
    }

    return false;
}
