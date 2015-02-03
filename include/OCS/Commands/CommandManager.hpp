/*Copyright (c) <2015> Kevin Miller - KevM1227@gmail.com

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#ifndef OCS_COMMAND_MANAGER_H
#define OCS_COMMAND_MANAGER_H

#include <memory>
#include <map>

#include "OCS/Commands/Command.hpp"
#include <OCS/Misc/Config.hpp>

namespace ocs
{

struct CommandInfo
{
    CommandInfo(ID _id, int _maxExecutes, const std::string& _alias) :
      id(_id),
      maxExecutes(_maxExecutes),
      alias(_alias)
    {}
    ID id;
    int maxExecutes;
    std::string alias;

    void execute()
    {
        if (command)
        {
            command->execute();
            --maxExecutes;
        }
        else
            maxExecutes = 0;
    }

    std::shared_ptr<Command> command;
};

class CommandManager
{
    public:
        CommandManager();

        template<typename CommandType, typename ... Args>
        ID add(Args&& ...);
        template<typename CommandType, typename ... Args>
        ID add(int, Args&& ...);
        template<typename CommandType, typename ... Args>
        ID add(const std::string&, Args&& ...);
        template<typename CommandType, typename ... Args>
        ID add(const std::string&, int, Args&& ...);

        bool exists(const std::string&) const;
        bool exists(ID) const;

        bool remove(const std::string&);
        bool remove(ID);

        bool execute(const std::string&);
        void executeAllAnonCmds();

        inline ID totalNamedCommands() const;
        inline ID totalAnonymousCommands() const;
        inline ID totalCommands() const;

    private:

        ID nextId;
        std::map<std::string, std::shared_ptr<CommandInfo>> namedCommands;
        std::map<ID, std::shared_ptr<CommandInfo>> commands;
};

template<typename CommandType, typename ... Args>
ID CommandManager::add(Args&& ... args)
{
    return add<CommandType>(1, args...);
}

template<typename CommandType, typename ... Args>
ID CommandManager::add(int maxExecutes, Args&& ... args)
{
    return add<CommandType>("", maxExecutes, args...);
}

template<typename CommandType, typename ... Args>
ID CommandManager::add(const std::string& alias, Args&& ... args)
{
    return add<CommandType>(alias, 1, args...);
}

template<typename CommandType, typename ... Args>
ID CommandManager::add(const std::string& alias, int maxExecutes, Args&& ... args)
{
    ID newId = nextId++;
    auto cmdInfo = std::make_shared<CommandInfo>(newId, maxExecutes, alias);
    cmdInfo->command = std::make_shared<CommandType>(std::forward<Args>(args)...);

    if (cmdInfo->alias == "")
      commands[cmdInfo->id] = cmdInfo;
    else
      namedCommands[cmdInfo->alias] = cmdInfo;

    return newId;
}

inline ID CommandManager::totalNamedCommands() const
{
    return namedCommands.size();
}

inline ID CommandManager::totalAnonymousCommands() const
{
    return commands.size();
}

inline ID CommandManager::totalCommands() const
{
    return totalNamedCommands() + totalCommands();
}

}//ocs

#endif
