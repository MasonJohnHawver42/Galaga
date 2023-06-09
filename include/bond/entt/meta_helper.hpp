#pragma once

#include "entt/meta/factory.hpp"
#include "entt/meta/resolve.hpp"
#include "sol/sol.hpp"

#define AUTO_ARG(x) sol::wrap<decltype(x), x>

[[nodiscard]] entt::id_type get_type_id(const sol::table &obj) {
    const auto f = obj["type_id"].get<sol::function>();
    assert(f.valid() && "type_id not exposed to lua!");
    return f.valid() ? f().get<entt::id_type>() : -1;
}

[[nodiscard]] entt::id_type deduce_type(const sol::object &obj) {
  auto type = obj.get_type();
  if (type == sol::type::number) { return obj.as<entt::id_type>(); }
  if (type == sol::type::table) { return get_type_id(obj); }
  assert(false); return -1;
}

// @see
// https://github.com/skypjack/entt/wiki/Crash-Course:-runtime-reflection-system

template <typename... Args>
inline auto invoke_meta_func(entt::meta_type meta_type,
                             entt::id_type function_id, Args &&...args) {
  if (!meta_type) {
    // TODO: Warning message
  } else {
    if (auto meta_function = meta_type.func(function_id); meta_function)
      return meta_function.invoke({}, std::forward<Args>(args)...);
  }
  return entt::meta_any{};
}

template <typename... Args>
inline auto invoke_meta_func(entt::id_type type_id, entt::id_type function_id,
                             Args &&...args) {
  return invoke_meta_func(entt::resolve(type_id), function_id,
                          std::forward<Args>(args)...);
}
