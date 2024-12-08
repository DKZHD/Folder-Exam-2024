#pragma once
#include "sol/state.hpp"

class LuaCustom
{
public:
	sol::state& GetState();
	void init();
	void LoadInitialFIles();
	template<typename ...Args>
	typename std::enable_if<(std::is_convertible_v<Args, std::string> && ...), void>::type
	LoadFiles(const Args& ...args);
private:
	sol::state luaState;
};

template <typename ... Args>
typename std::enable_if<(std::is_convertible_v<Args, std::string> && ...), void>::type LuaCustom::LoadFiles(
	const Args&... args)
{
	(luaState.script_file(args), ...);
}
