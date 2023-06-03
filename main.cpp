#include "raylib.h"

#include "entt/entt.hpp"
#include "sol/sol.hpp"
#include "bond/bond.hpp"
#include "resources/resources.hpp"
#include "game/game.hpp"

#include <iostream>
#include <sstream>
#include <set>

// #include "utill.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// static bool open = true;

struct Test 
{
  int x;
  int y;

  Test() {}
  Test(int a, int b) : x(a), y(b) {}

  [[nodiscard]] std::string to_string() const {
    std::stringstream ss;
    ss << "{ x=" << std::to_string(x) << ", y=" << std::to_string(y) << " }";
    return ss.str();
  }
};

sol::state     lua;
entt::registry g_Registry;
res::Resources g_Resources;

void tick();

using namespace entt::literals;

int main(int argc, char const *argv[])
{

  g_Resources = res::Resources{};
  res::AssCache& ass_cache = g_Resources.add<game::Ass, game::AssLoader>();
  res::ScriptCache& script_cache = g_Resources.add<game::Script, game::ScriptLoader>();
  ass_cache.load(0, 10, 20);
  script_cache.load(0, "assets/scripts/test.lua", lua);

  if (ass_cache.valid(0)) 
  {
    game::Ass& ass = *ass_cache[0];
    std::cout << "ASS " << ass.x << ", " << ass.y << std::endl;
  }
 
  lua = sol::state{};
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
  lua.require("registry", sol::c_call<AUTO_ARG(&open_registry)>, false);

  register_meta_component<Test>();

  lua.new_usertype<Test>("Test",
    sol::call_constructor,
    sol::factories([](int x, int y) { return Test{ x, y }; }),
    "type_id", &entt::type_hash<Test>::value,
    "x", &Test::x, 
    "y", &Test::y, 
    sol::meta_function::to_string, &Test::to_string
  );

  g_Registry = entt::registry{};
  lua["g_reg"] = std::ref(g_Registry);

  auto test = g_Registry.create();
  g_Registry.emplace<Test>(test, Test{1, 2});

  // lua.do_file("assets/scripts/test.lua");
  // sol::load_result lr = lua.load_file("assets/scripts/test.lua");
  // assert(lr.valid());
  // sol::protected_function target = lr.get<sol::protected_function>();
  // target();

  assert(script_cache.valid(0));
  game::Script& script = *script_cache[0];
  std::cout << "here" << std::endl;
  assert(script.script.valid());
  std::cout << "here" << std::endl;
  script.script();
  std::cout << "here" << std::endl;
  lua.do_file("assets/scripts/iterate_entities.lua");
  

  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
  InitWindow(1080, 720, "raylib [core] example - basic window");
  SetTargetFPS(144);
  // rlImGuiSetup(true);

  #if defined(PLATFORM_WEB)
  emscripten_set_main_loop(tick, 144, 1);
  #else
  while (!WindowShouldClose()) { tick(); }
  #endif

  // rlImGuiShutdown();
  CloseWindow();

  return 0;
}

void tick() 
{
  float dt = GetFrameTime();

  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

  // start ImGui Conent
  // rlImGuiBegin();
  // if (open) { ImGui::ShowDemoWindow(&open); }
  // rlImGuiEnd();

  EndDrawing(); 
}