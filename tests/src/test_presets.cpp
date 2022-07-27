#include "presets/PresetManager.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>

TEST(test_presets, test_parser_nus)
{
    Preset::Manager prm;

    ASSERT_EQ(prm.loadFromFile("../../presets/nus.toml"), EXIT_SUCCESS);

    auto pr = prm.getPreset();

    // autoconnect should be false
    ASSERT_EQ(pr.autoconnect, false);

    // name
    ASSERT_EQ(pr.name, "my_device");

    // mac
    ASSERT_EQ(pr.mac, "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");

    // services
    ASSERT_EQ(pr.services.size(), 1);

    auto nus = pr.services.at(0);

    ASSERT_EQ(nus.name, "Nordic_UART_Service");
    ASSERT_EQ(nus.uuid, "6E400001-B5A3-F393-E0A9-E50E24DCCA9E");

    ASSERT_EQ(nus.characteristics.size(), 2);

    ASSERT_EQ(nus.characteristics.at(1).name, "tx");
    ASSERT_EQ(nus.characteristics.at(1).uuid, "6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

    ASSERT_EQ(nus.characteristics.at(0).name, "rx");
    ASSERT_EQ(nus.characteristics.at(0).uuid, "6E400002-B5A3-F393-E0A9-E50E24DCCA9E");

    // commands
    ASSERT_EQ(pr.commands.size(), 2);

    // connect_rx
    auto cmd_rx = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "connect_rx"; });
    ASSERT_NE(cmd_rx, pr.commands.end());
    ASSERT_EQ(cmd_rx.base()->getActionAsStr(), "notify");
    ASSERT_EQ(cmd_rx.base()->getService(), "Nordic_UART_Service");
    ASSERT_EQ(cmd_rx.base()->getCharacteristic(), "rx");

    // send_hello
    auto cmd_hello = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "send_hello"; });
    ASSERT_NE(cmd_hello, pr.commands.end());
    ASSERT_EQ(cmd_hello.base()->getActionAsStr(), "write");
    ASSERT_EQ(cmd_hello.base()->getService(), "Nordic_UART_Service");
    ASSERT_EQ(cmd_hello.base()->getCharacteristic(), "tx");
    ASSERT_EQ(cmd_hello.base()->getPayloadAsStr(), "'Hello\r\n'");
}

TEST(test_presets, test_parser_battery)
{
    Preset::Manager prm;

    ASSERT_EQ(prm.loadFromFile("../../presets/battery.toml"), EXIT_SUCCESS);

    auto pr = prm.getPreset();

    ASSERT_EQ(pr.autoconnect, true);

    // name
    ASSERT_EQ(pr.name, "my_device");

    // mac
    ASSERT_EQ(pr.mac, "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");

    // services
    ASSERT_EQ(pr.services.size(), 1);

    auto nus = pr.services.at(0);

    ASSERT_EQ(nus.name, "battery service");
    ASSERT_EQ(nus.uuid, "180f");

    ASSERT_EQ(nus.characteristics.size(), 1);

    ASSERT_EQ(nus.characteristics.at(0).name, "level");
    ASSERT_EQ(nus.characteristics.at(0).uuid, "2a19");

    // commands
    ASSERT_EQ(pr.commands.size(), 2);

    // read_battery
    auto cmd_read_battery = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "read_battery"; });
    ASSERT_NE(cmd_read_battery, pr.commands.end());
    ASSERT_EQ(cmd_read_battery.base()->getActionAsStr(), "read");
    ASSERT_EQ(cmd_read_battery.base()->getService(), "battery service");
    ASSERT_EQ(cmd_read_battery.base()->getCharacteristic(), "level");

    // sub battery
    auto cmd_sub_battery = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "sub_battery"; });
    ASSERT_NE(cmd_sub_battery, pr.commands.end());
    ASSERT_EQ(cmd_sub_battery.base()->getActionAsStr(), "notify");
    ASSERT_EQ(cmd_sub_battery.base()->getService(), "battery service");
    ASSERT_EQ(cmd_sub_battery.base()->getCharacteristic(), "level");
}

TEST(test_presets, test_parser_hrs)
{
    Preset::Manager prm;

    ASSERT_EQ(prm.loadFromFile("../../presets/hrs.toml"), EXIT_SUCCESS);

    auto pr = prm.getPreset();

    ASSERT_EQ(pr.autoconnect, false);

    // name
    ASSERT_EQ(pr.name, "my_device");

    // mac
    ASSERT_EQ(pr.mac, "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");

    // services
    ASSERT_EQ(pr.services.size(), 1);

    auto nus = pr.services.at(0);

    ASSERT_EQ(nus.name, "Heart rate services");
    ASSERT_EQ(nus.uuid, "180d");

    ASSERT_EQ(nus.characteristics.size(), 2);

    ASSERT_EQ(nus.characteristics.at(1).name, "Heart rate mesurement");
    ASSERT_EQ(nus.characteristics.at(1).uuid, "2a37");

    ASSERT_EQ(nus.characteristics.at(0).name, "Body sensor location");
    ASSERT_EQ(nus.characteristics.at(0).uuid, "2a38");

    // commands
    ASSERT_EQ(pr.commands.size(), 2);

    // read_battery
    auto cmd_read_battery = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "get_location"; });
    ASSERT_NE(cmd_read_battery, pr.commands.end());
    ASSERT_EQ(cmd_read_battery.base()->getActionAsStr(), "read");
    ASSERT_EQ(cmd_read_battery.base()->getService(), "Heart rate services");
    ASSERT_EQ(cmd_read_battery.base()->getCharacteristic(), "Body sensor location");

    // sub battery
    auto cmd_sub_battery = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "get_rate"; });
    ASSERT_NE(cmd_sub_battery, pr.commands.end());
    ASSERT_EQ(cmd_sub_battery.base()->getActionAsStr(), "read");
    ASSERT_EQ(cmd_sub_battery.base()->getService(), "Heart rate services");
    ASSERT_EQ(cmd_sub_battery.base()->getCharacteristic(), "Heart rate mesurement");
}

TEST(test_presets, test_base)
{
    Preset::Manager prm;

    ASSERT_EQ(prm.loadFromFile("../../presets/base.toml"), EXIT_SUCCESS);

    auto pr = prm.getPreset();

    ASSERT_EQ(pr.autoconnect, false);

    ASSERT_EQ(pr.name, "my_device");

    ASSERT_EQ(pr.mac, "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");
}

TEST(test_presets, test_color)
{
    Preset::Manager prm;

    ASSERT_EQ(prm.loadFromFile("../../presets/color.toml"), EXIT_SUCCESS);

    auto pr = prm.getPreset();

    ASSERT_EQ(pr.autoconnect, false);
    ASSERT_EQ(pr.name, "my_device");
    ASSERT_EQ(pr.mac, "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");

    // services
    ASSERT_EQ(pr.services.size(), 1);

    auto color_service = pr.services.at(0);

    ASSERT_EQ(color_service.name, "color service");
    ASSERT_EQ(color_service.uuid, "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");

    ASSERT_EQ(color_service.characteristics.size(), 1);

    ASSERT_EQ(color_service.characteristics.at(0).name, "rgb");
    ASSERT_EQ(
        color_service.characteristics.at(0).uuid, "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX");

    // commands
    ASSERT_EQ(pr.commands.size(), 4);

    // led_red
    auto cmd_led_red = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "led_red"; });
    ASSERT_NE(cmd_led_red, pr.commands.end());
    ASSERT_EQ(cmd_led_red.base()->getActionAsStr(), "write");
    ASSERT_EQ(cmd_led_red.base()->getService(), "color service");
    ASSERT_EQ(cmd_led_red.base()->getCharacteristic(), "rgb");
    ASSERT_EQ(cmd_led_red.base()->getPayloadAsStr(), "255 0 0");

    // led_green
    auto cmd_led_green = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "led_green"; });
    ASSERT_NE(cmd_led_green, pr.commands.end());
    ASSERT_EQ(cmd_led_green.base()->getActionAsStr(), "write");
    ASSERT_EQ(cmd_led_green.base()->getService(), "color service");
    ASSERT_EQ(cmd_led_green.base()->getCharacteristic(), "rgb");
    ASSERT_EQ(cmd_led_green.base()->getPayloadAsStr(), "0 255 0");

    // led_blue
    auto cmd_led_blue = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "led_blue"; });
    ASSERT_NE(cmd_led_blue, pr.commands.end());
    ASSERT_EQ(cmd_led_blue.base()->getActionAsStr(), "write");
    ASSERT_EQ(cmd_led_blue.base()->getService(), "color service");
    ASSERT_EQ(cmd_led_blue.base()->getCharacteristic(), "rgb");
    ASSERT_EQ(cmd_led_blue.base()->getPayloadAsStr(), "0 0 255");

    // get_color
    auto cmd_get_color = std::find_if(pr.commands.begin(), pr.commands.end(),
        [](Preset::Command c) { return c.getName() == "get_color"; });
    ASSERT_NE(cmd_get_color, pr.commands.end());
    ASSERT_EQ(cmd_get_color.base()->getActionAsStr(), "read");
    ASSERT_EQ(cmd_get_color.base()->getService(), "color service");
    ASSERT_EQ(cmd_get_color.base()->getCharacteristic(), "rgb");
}
