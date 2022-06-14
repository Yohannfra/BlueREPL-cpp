#include "PresetParser.hpp"

#include <iostream>
#include <optional>

int Preset::Parser::parseFile(const std::filesystem::path &fp, Preset &dest)
{
    toml::table tbl;

    try {
        tbl = toml::parse_file(fp.string());
    } catch (const std::exception &e) {
        std::cerr << fp << ": " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return this->parseAll(tbl, dest);
}

int Preset::Parser::parseAll(toml::table &tbl, Preset &dest)
{
    // Device name
    auto device_name = tbl["DEVICE_NAME"].value<std::string>();
    if (device_name.has_value()) {
        dest.name = device_name.value();
    } else {
        std::cerr << "Invalid or missing 'DEVICE_NAME'" << std::endl;
    }

    // Device mac address
    auto device_addr = tbl["DEVICE_ADDRESS"].value<std::string>();
    if (device_addr.has_value()) {
        dest.mac = device_addr.value();
    } else {
        std::cerr << "Invalid or missing 'DEVICE_ADDRESS'" << std::endl;
    }

    // autoconnect
    auto is_autoconnect = tbl["autoconnect"].value<bool>();
    if (is_autoconnect.has_value()) {
        dest.autoconnect = is_autoconnect.value();
    } else {
        dest.autoconnect = false;
    }

    // Services
    if (tbl["services"] && tbl["services"].is_table()) {
        this->parseServices(*tbl["services"].as_table(), dest);
    }

    // Aliases
    if (tbl["aliases"]) {
        const auto aliasesNode = tbl["aliases"][0];
        for (const auto &[k, v] : *aliasesNode.as_table()) {
            std::pair<std::string, std::string> tmp = {
                std::string(k.str()),
                v.value<std::string>().value(),
            };
            dest.aliases[tmp.first] = tmp.second;
        }
    }

    // Commands
    if (tbl["commands"]) {
        const auto commandesNode = tbl["commands"][0];
        for (const auto &[cmd_name, _] : *commandesNode.as_table()) {
            toml::array val = (*commandesNode[cmd_name].as_array());

            if (val.size() < 3) {
                std::cerr << "Invalid command size (minimum 3 arguments)" << std::endl;
                return EXIT_FAILURE;
            }

            Command command;
            command.setName(cmd_name);
            if (command.setAction(val[0].value<std::string>().value()) == EXIT_FAILURE) {
                std::cerr << "Invalid command action in: " << cmd_name << std::endl;
                return EXIT_FAILURE;
            }
            command.setService(val[1].value<std::string>().value());
            command.setCharacteristic(val[2].value<std::string>().value());

            if (command.getAction() != Command::Action::WRITE) {
                goto add_command;
            }

            // payload(s)
            if (command.getAction() == Command::Action::WRITE) {
                for (std::size_t i = 3; i < val.size(); i++) {
                    if (val[i].type() == toml::node_type::integer) {
                        auto n = val[i].value<std::uint8_t>();
                        if (!n.has_value()) {
                            std::cerr << "Invalid integer payload in " << cmd_name<< std::endl;
                            return EXIT_FAILURE;
                        }
                        command.addPayload(n.value());
                    } else if (val[i].type() == toml::node_type::floating_point) {
                        const double s = val[i].value<double>().value();
                        command.addPayload(s);
                    } else if (val[i].type() == toml::node_type::string) {
                        const std::string s = val[i].value<std::string>().value();
                        command.addPayload(s);
                    } else {
                        std::cerr << "Invalid payload type " << val[i].type()
                                  << std::endl;
                        return EXIT_FAILURE;
                    }
                }
            }

        add_command:
            dest.commands.push_back(command);
        }
    }
    return EXIT_SUCCESS;
}

void Preset::Parser::parseCharacteristic(
    const std::string &name, toml::node_view<toml::node> characteristic, Service &service)
{
    Characteristic char_tmp;

    // Characteristic Name
    auto name_tmp = characteristic["NAME"].value<std::string>();
    if (name_tmp.has_value()) {
        char_tmp.name = name_tmp.value();
    }

    // Characteristic UUID
    auto uuid = characteristic["UUID"].value<std::string>();
    if (uuid.has_value()) {
        char_tmp.uuid = uuid.value();
    }

    service.characteristics.push_back(char_tmp);
}

void Preset::Parser::parseServices(toml::table services, Preset &dest)
{
    for (auto &[service_name, _] : services) {
        Service service_tmp;
        toml::node_view<toml::node> service_table = services[service_name][0];

        // Service Name
        auto name = service_table["NAME"].value<std::string>();
        if (!name.has_value()) {
            std::cerr << "Service: '" << service_name << "' has no NAME" << std::endl;
        } else {
            service_tmp.name = name.value();
        }

        // Service UUID
        auto uuid = service_table["UUID"].value<std::string>();
        if (!uuid.has_value()) {
            std::cerr << "Service: '" << service_name << "' has no UUID" << std::endl;
        } else {
            service_tmp.uuid = uuid.value();
        }

        // Characteristics
        if (service_table["characteristics"]) {
            for (const auto &[c, __] :
                *service_table["characteristics"].node()->as_table()) {
                auto current_characteristic = service_table["characteristics"][c][0];
                this->parseCharacteristic(
                    c.str().data(), current_characteristic, service_tmp);
            }
        }
        dest.services.push_back(service_tmp);
    }
}
