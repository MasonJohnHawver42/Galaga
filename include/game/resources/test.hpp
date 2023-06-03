#pragma once

#include "resources/resources.hpp"

namespace game 
{

    struct Ass 
    {
        int x, y;
        Ass(int a, int b) : x(a), y(b) {}
    };

    struct AssLoader final 
    {
        std::shared_ptr<Ass> load(int a, int b) const 
        {
            return std::make_shared<Ass>(a, b);
        }
        void save  (std::shared_ptr<Ass> res) const {}
        void unload(std::shared_ptr<Ass> res) const {}
    };

}

namespace res 
{
    using AssCache = res::Cache<game::Ass, game::AssLoader>;
}
