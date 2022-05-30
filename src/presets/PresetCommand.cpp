#include "PresetCommand.hpp"

#include <iostream>

Preset::Command::Command()
{
}

Preset::Command::Command(Action action, const std::string &name,
    const std::string &service, const std::string &characteristic) :
    _action(action),
    _name(name), _service(service), _characteristic(characteristic)
{
}

Preset::Command::Command(const Command &c)
{
    _action = c._action;
    _name = c._name;
    _service = c._service;
    _characteristic = c._characteristic;
    _payload = c._payload;
}

int Preset::Command::runAction(BleController &bt)
{
    std::cout << "Running command:" << std::endl;
    std::cout << this->getActionAsStr() + " " + _service + " " + _characteristic + " " +
            this->getPayloadAsStr()
              << std::endl;
    // TODO run command
    return EXIT_SUCCESS;
}

void Preset::Command::setPayload(const std::vector<PayloadVariant> &payload)
{
    _payload = payload;
}

void Preset::Command::addPayload(const PayloadVariant payload)
{
    _payload.push_back(payload);
}

void Preset::Command::addPayload(std::uint8_t payload)
{
    PayloadVariant v = payload;
    _payload.push_back(v);
}

void Preset::Command::addPayload(const std::string &payload)
{
    PayloadVariant v = payload;
    _payload.push_back(v);
}

void Preset::Command::setAction(Action a)
{
    _action = a;
}

void Preset::Command::setName(const std::string_view &name)
{
    _name = name;
}

void Preset::Command::setService(const std::string &service)
{
    _service = service;
}

void Preset::Command::setCharacteristic(const std::string &characteristic)
{
    _characteristic = characteristic;
}

int Preset::Command::setAction(const std::string &actionStr)
{
    if (actionStr == "write") {
        _action = Action::WRITE;
    } else if (actionStr == "read") {
        _action = Action::READ;
    } else if (actionStr == "notify") {
        _action = Action::NOTIFY;
    } else if (actionStr == "indicate") {
        _action = Action::NOTIFY;
    } else if (actionStr == "unsubscribe") {
        _action = Action::UNSUBSCRIBE;
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

Preset::Command::Action Preset::Command::getAction() const
{
    return _action;
}

const std::string Preset::Command::getActionAsStr() const
{
    switch (_action) {
        case Action::READ:
            return "read";
        case Action::WRITE:
            return "write";
        case Action::NOTIFY:
            return "notify";
        case Action::INDICATE:
            return "indicate";
        case Action::UNSUBSCRIBE:
            return "unsubscribe";
    }
}

const std::string &Preset::Command::getName() const
{
    return _name;
}

const std::string &Preset::Command::getService() const
{
    return _service;
}

const std::string &Preset::Command::getCharacteristic() const
{
    return _characteristic;
}

const std::string Preset::Command::getPayloadAsStr() const
{
    std::string out = "";

    for (const auto &p : _payload) {
        try {
            auto n = std::get<std::uint8_t>(p);
            out += std::to_string(n) + " ";
        } catch (...) {
            auto s = std::get<std::string>(p);
            out += s + " ";
        }
    }

    return out;
}
