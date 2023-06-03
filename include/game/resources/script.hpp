#pragma once

#include "resources/resources.hpp"
#include "sol/sol.hpp"

#include <memory>
#include <cassert>
#include <iostream>

namespace game 
{

    struct Script 
    {
        sol::protected_function script;
        std::string path;
        Script(sol::protected_function& s, const std::string& p) : script(s), path(p) {}
    };

    struct ScriptLoader final 
    {
        std::shared_ptr<Script> load(const std::string& path, sol::state &lua) const 
        {
            sol::load_result lr = lua.load_file(path);
            assert(lr.valid());
            sol::protected_function script = lr.get<sol::protected_function>();
            return std::make_shared<Script>(script, path);
        }
        void save  (std::shared_ptr<Script> res) const {}
        void unload(std::shared_ptr<Script> res) const {}
    };

}

namespace res 
{
    using ScriptCache = res::Cache<game::Script, game::ScriptLoader>;
}