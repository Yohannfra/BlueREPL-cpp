#include "PresetManager.hpp"

#include "utils.hpp"

#include <fmt/core.h>
#include <iostream>
#include <tabulate/table.hpp>
#include <variant>

Preset::Manager::Manager() : _preset({})
{
}

int Preset::Manager::loadFromFile(const std::filesystem::path &fp)
{
    _preset.fp = fp;
    if (_parser.parseFile(fp, _preset) == EXIT_FAILURE) {
        std::cerr << "Failed to parse '" << fp << "'" << std::endl;
        return EXIT_FAILURE;
    }
    /* this->print(); */
    return EXIT_SUCCESS;
}

void Preset::Manager::print() const
{
    constexpr uint32_t WIDTH = 80;
    tabulate::Table full;

    full.format()
        .border_left("|")
        .border_right("|")
        .border_top("─")
        .border_bottom("─")
        .width(WIDTH);

    full.add_row({_preset.fp})[0][0]
        .format()
        .font_align(tabulate::FontAlign::center)
        .font_style({tabulate::FontStyle::bold});

    // device name and address
    tabulate::Table top;
    top.add_row({"Device name:", _preset.name})[0].format().border(" ").corner(" ");
    top.add_row({"Device address:", _preset.mac})[1].format().border(" ").corner(" ");
    full.add_row({top})[0].format().hide_border_bottom();

    // services
    full.add_row({"Services:"})[2]
        .format()
        .font_align(tabulate::FontAlign::center)
        .font_style({tabulate::FontStyle::bold});

    tabulate::Table services;
    services.format().width(WIDTH - 4);
    for (const auto &s : _preset.services) {
        tabulate::Table top_service;
        top_service.add_row({"Name:", s.name}).format().hide_border();
        top_service.add_row({"UUID:", s.uuid});
        services.add_row({top_service});

        tabulate::Table tmp;

        for (const auto &c : s.characteristics) {
            tmp.add_row({c.name, c.uuid});
        }
        services.add_row({tmp});
    }
    full.add_row({services});

    // Aliases
    if (!_preset.aliases.empty()) {
        full.add_row({"Aliases:"})[4]
            .format()
            .font_align(tabulate::FontAlign::center)
            .font_style({tabulate::FontStyle::bold});

        tabulate::Table aliases;
        aliases.format().width(WIDTH / 2 - 3);
        aliases.add_row({"Key", "Value"});
        for (const auto &[k, v] : _preset.aliases) {
            aliases.add_row({k, v});
        }
        full.add_row({aliases});
    }

    // Commands
    if (!_preset.commands.empty()) {
        full.add_row({"Commands:"})[4]
            .format()
            .font_align(tabulate::FontAlign::center)
            .font_style({tabulate::FontStyle::bold});

        tabulate::Table commands;
        commands.add_row({"Name", "Type", "Service", "Characteristic", "Payload"});

        for (const auto &a : _preset.commands) {
            commands.add_row({
                a.getName(),
                a.getActionAsStr(),
                a.getService(),
                a.getCharacteristic(),
                a.getPayloadAsStr(),
            });
        }
        full.add_row({commands});
    }

    std::cout << full << std::endl;
}

Preset::Preset &Preset::Manager::getPreset()
{
    return _preset;
}
