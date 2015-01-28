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
#include <queue>

#include "OCS/Commands/Command.hpp"

namespace ocs
{

using CmdPtr = std::shared_ptr<Command>;

class CommandManager
{
    public:

        void addCommand(CmdPtr, bool = false);
        void addCommand(CmdPtr, const std::string&, bool = false);

    private:

        std::map<std::string, CmdPtr> namedCommands;
        std::queue<CmdPtr> commands;
};

}//ocs

#endif
