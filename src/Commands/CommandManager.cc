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

#include <vector>

#include <OCS/Commands/CommandManager.hpp>

namespace ocs
{

CommandManager::CommandManager() :
    nextId(0)
{}

struct RemoveAnonCommand : public ocs::Command
{
    RemoveAnonCommand(ID _id, CommandManager& _cmdManager) :
        id(_id),
        cmdManager(_cmdManager)
    {}
    ID id;
    CommandManager& cmdManager;

    void execute()
    {
        cmdManager.remove(id);
    }

};

bool CommandManager::exists(const std::string& alias) const
{
    return namedCommands.find(alias) != namedCommands.end();
}

bool CommandManager::exists(ID id) const
{
    return commands.find(id) != commands.end();
}

bool CommandManager::remove(const std::string& alias)
{
    bool removed = false;
    if (exists(alias))
        removed = namedCommands.erase(alias);
    return removed;
}

bool CommandManager::remove(ID id)
{
    bool removed = false;
    if (exists(id))
        removed = commands.erase(id);
    return removed;
}

bool CommandManager::execute(const std::string& alias)
{
    if (exists(alias))
    {
        namedCommands[alias]->execute();
        if (namedCommands[alias]->maxExecutes == 0)
            remove(alias);
        return true;
    }
    return false;

}

void CommandManager::executeAllAnonCmds()
{
    std::vector<RemoveAnonCommand> toRemove;

    for (auto& cmd : commands)
    {
        cmd.second->execute();
        if (commands[cmd.second->id]->maxExecutes == 0)
            toRemove.emplace_back(cmd.second->id, *this);
    }

    for (auto& rem : toRemove)
        rem.execute();
}


} //ocs
