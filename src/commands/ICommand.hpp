#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

namespace Command {
    class ICommand {
    public:
        virtual void print_help() = 0;
    };
} // namespace Command

#endif
