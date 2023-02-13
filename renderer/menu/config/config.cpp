#include "config.hpp"

#include "../menu.hpp"

std::variant< bool, int, float, engine::structures::vector_t, engine::structures::vector4_t > menu::config::config_map[]
{
	true,
	5,
	1,
	50,
	true,
	true,
	50,
	0,
	VK_RBUTTON,

	500,

	true,
	false,
	false,
	false,
	false,
	false,

	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },

	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 1.f, 1.f, 1.f, 1.f },
	engine::structures::vector4_t { 0.f, 1.f, 0.f, 1.f },
};