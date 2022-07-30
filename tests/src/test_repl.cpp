#include "Controllers/Mock/MockController.hpp"
#include "Repl/Repl.hpp"

#include <algorithm>
#include <gtest/gtest.h>
#include <iostream>

TEST(test_repl, test_quit)
{
    Repl r;
    bool quit = false;

    ASSERT_EQ(r.runCommand("quit", quit), EXIT_SUCCESS);
    EXPECT_TRUE(quit);

    quit = false;

    ASSERT_EQ(r.runCommand("exit", quit), EXIT_SUCCESS);
    EXPECT_TRUE(quit);

    quit = false;

    ASSERT_EQ(r.runCommand("bye", quit), EXIT_FAILURE);
    EXPECT_FALSE(quit);
}

TEST(test_repl, test_help)
{
    Repl r;
    bool quit;

    ASSERT_EQ(r.runCommand("help", quit), EXIT_SUCCESS);

    auto all_cmds = r.getAllCommandsNames();
    for (auto &c : all_cmds) {
        ASSERT_EQ(r.runCommand("help " + c, quit), EXIT_SUCCESS);
    }
}

TEST(test_repl, test_aliases)
{
    Repl r;
    bool quit;

    // basic usage
    ASSERT_EQ(r.runCommand("foo", quit), EXIT_FAILURE);
    ASSERT_EQ(r.runCommand("alias foo help", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("foo", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("alias foo", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("foo", quit), EXIT_FAILURE);

    // invalid key
    ASSERT_EQ(r.runCommand("alias foo12 help", quit), EXIT_FAILURE);
    ASSERT_EQ(r.runCommand("alias 12 foo help", quit), EXIT_FAILURE);
    ASSERT_EQ(r.runCommand("alias \"f oo\" help", quit), EXIT_FAILURE);

    // same key and value
    ASSERT_EQ(r.runCommand("alias foo foo", quit), EXIT_FAILURE);

    // space in value
    ASSERT_EQ(r.runCommand("alias foo \"help write\"", quit), EXIT_FAILURE);

    // nested aliases
    ASSERT_EQ(r.runCommand("alias foo bar", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("alias bar help", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("foo", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("bar", quit), EXIT_SUCCESS);

    // nested aliases but abused
    ASSERT_EQ(r.runCommand("alias fooa foob", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("alias foob fooc", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("alias fooc food", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("alias food fooe", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("alias fooe help", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("fooa", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("foob", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("fooc", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("food", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("fooe", quit), EXIT_SUCCESS);
}

TEST(test_repl, test_preset)
{
    Repl r;
    bool quit;

    BleController *bt = new MockController();
    r.setBleController(bt);

    // no preset loaded
    ASSERT_EQ(r.runCommand("preset", quit), EXIT_FAILURE);

    // loading a preset
    Preset::Manager prm;
    ASSERT_EQ(prm.loadFromFile("../../presets/nus.toml"), EXIT_SUCCESS);

    ASSERT_EQ(r.runCommand("connect_rx", quit), EXIT_FAILURE);
    ASSERT_EQ(r.runCommand("send_hello", quit), EXIT_FAILURE);

    ASSERT_EQ(r.loadPreset(prm), EXIT_SUCCESS);

    // preset loaded
    ASSERT_EQ(r.runCommand("preset", quit), EXIT_SUCCESS);

    // commands in preset
    ASSERT_EQ(r.runCommand("connect_rx", quit), EXIT_SUCCESS);
    ASSERT_EQ(r.runCommand("send_hello", quit), EXIT_SUCCESS);
}
