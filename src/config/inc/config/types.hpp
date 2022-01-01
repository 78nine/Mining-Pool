#ifndef NEXUSPOOL_CONFIG_TYPES_HPP
#define NEXUSPOOL_CONFIG_TYPES_HPP

#include <string>
#include <variant>

namespace nexuspool
{
namespace config
{

enum class Persistance_type : std::uint8_t 
{
	database = 0,
	sqlite
};

struct Persistance_config
{
	Persistance_type m_type{ Persistance_type::database};
	std::string m_file{};
};

struct Pool_config
{
	std::string m_account{};
	std::string m_pin{};
	std::uint16_t m_fee{};
	std::uint16_t m_difficulty_divider{};
	std::uint16_t m_round_duration_hours{};
};
}
}
#endif
