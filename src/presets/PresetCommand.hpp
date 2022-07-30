#ifndef PRESETCOMMAND_HPP
#define PRESETCOMMAND_HPP

#include "BleController.hpp"
#include "commands/ICommand.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace Preset {
    class Command {
    public:
        enum class Action {
            READ,
            WRITE,
            NOTIFY,
            INDICATE,
            UNSUBSCRIBE,
        };

        enum class PayloadType {
            INT,
            STRING,
        };

        using PayloadVariant = std::variant<std::uint8_t, std::string>;

        Command();

        Command(Action action, const std::string &name, const std::string &service,
            const std::string &characteristic);

        Command(const Command &c);

        void setPayload(const std::vector<PayloadVariant> &payload);

        void addPayload(const PayloadVariant payload);
        void addPayload(std::uint8_t payload);
        void addPayload(const std::string &payload);
        bool hasPayload() const;

        int runAction(BleController &bt, std::vector<::Command::ICommand *> commands);

        void setAction(Action a);
        int setAction(const std::string &actionStr);
        void setName(const std::string_view &name);
        void setService(const std::string &service);
        void setCharacteristic(const std::string &characteristic);

        Action getAction() const;
        const std::string getActionAsStr() const;
        const std::string &getName() const;
        const std::string &getService() const;
        const std::string &getCharacteristic() const;
        const std::string getPayloadAsStr() const;

    private:
        Action _action;
        std::string _name;
        std::string _service;
        std::string _characteristic;
        std::vector<PayloadVariant> _payload;
    };
} // namespace Preset

// for cout
std::ostream &operator<<(std::ostream &os, Preset::Command const &m);

#endif
