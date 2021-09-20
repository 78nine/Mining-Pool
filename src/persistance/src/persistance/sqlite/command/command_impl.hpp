#ifndef NEXUSPOOL_PERSISTANCE_SQLITE_COMMAND_COMMAND_IMPL_HPP
#define NEXUSPOOL_PERSISTANCE_SQLITE_COMMAND_COMMAND_IMPL_HPP

#include "persistance/command/command.hpp"
#include "persistance/sqlite/types.hpp"
#include "sqlite/sqlite3.h"
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <iostream>

namespace spdlog { class logger; }
namespace nexuspool {
namespace persistance {
namespace command {

class Command_base_database_sqlite : public Command_base_database
{
public:
	virtual ~Command_base_database_sqlite() { sqlite3_finalize(m_stmt); }
	Command_base_database_sqlite(sqlite3* handle) : m_handle{ handle }, m_stmt{ nullptr } {}
	Class get_class() const override { return Class::database_sqlite; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt} }; }
	void reset() override;

protected:

	void bind_param(sqlite3_stmt* stmt, const char* name, std::string const& value);
	void bind_param(sqlite3_stmt* stmt, const char* name, int value);
	void bind_param(sqlite3_stmt* stmt, const char* name, int64_t value);
	void bind_param(sqlite3_stmt* stmt, const char* name, double value);

	sqlite3* m_handle;
	sqlite3_stmt* m_stmt;

};


class Command_banned_user_and_ip_impl : public Command_base_database_sqlite
{
public:

	explicit Command_banned_user_and_ip_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_banned_user_and_ip; }
	void set_params(std::any params) override;
};

class Command_banned_api_ip_impl : public Command_base_database_sqlite
{
public:

	explicit Command_banned_api_ip_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_banned_api_ip; }
	void set_params(std::any params) override;
};

class Command_account_exists_impl : public Command_base_database_sqlite
{
public:

	explicit Command_account_exists_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::account_exists; }
	void set_params(std::any params) override;
};

class Command_get_account_impl : public Command_base_database_sqlite
{
public:

	explicit Command_get_account_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_account; }
	void set_params(std::any params) override;
};

class Command_get_blocks_impl : public Command_base_database_sqlite
{
public:

	explicit Command_get_blocks_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_blocks; }
};

class Command_get_latest_round_impl : public Command_base_database_sqlite
{
public:

	explicit Command_get_latest_round_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_latest_round; }
};

class Command_get_round_impl : public Command_base_database_sqlite
{
public:

	explicit Command_get_round_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_round; }
	void set_params(std::any params) override;
};

class Command_get_payments_impl : public Command_base_database_sqlite
{
public:

	explicit Command_get_payments_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_payments; }
	void set_params(std::any params) override;
};

class Command_get_config_impl : public Command_base_database_sqlite
{
public:

	explicit Command_get_config_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_config; }
	void set_params(std::any params) override { /* no params */ };
};

class Command_get_active_accounts_from_round_impl : public Command_base_database_sqlite
{
public:

	explicit Command_get_active_accounts_from_round_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_active_accounts_from_round; }
	void set_params(std::any params) override { /* no params */ };
};

class Command_get_blocks_from_round_impl : public Command_base_database_sqlite
{
public:

	explicit Command_get_blocks_from_round_impl(sqlite3* handle);

	std::any get_command() const override;
	Type get_type() const override { return Type::get_blocks_from_round; }
	void set_params(std::any params) override;
};

// ------------------------------------------------------------------------------------
// Write commands

class Command_create_account_impl : public Command_base_database_sqlite
{
public:

	explicit Command_create_account_impl(sqlite3* handle);

	Type get_type() const override { return Type::create_account; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
	void set_params(std::any params) override;
};

struct Command_add_payment_params
{
	std::string m_account;
	double m_amount;
	double m_shares;
	std::string m_payment_datetime;
	std::int64_t m_round;
};

class Command_add_payment_impl : public Command_base_database_sqlite
{
public:

	explicit Command_add_payment_impl(sqlite3* handle);

	Type get_type() const override { return Type::add_payment; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
	void set_params(std::any params) override;
};

class Command_create_round_impl : public Command_base_database_sqlite
{
public:

	explicit Command_create_round_impl(sqlite3* handle);

	Type get_type() const override { return Type::create_round; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
	void set_params(std::any params) override;
};

struct Command_update_account_params
{
	std::string m_last_active;
	int m_connection_count;
	double m_shares;
	double m_hashrate;
	std::string m_name;
};

class Command_update_account_impl : public Command_base_database_sqlite
{
public:

	explicit Command_update_account_impl(sqlite3* handle);

	Type get_type() const override { return Type::update_account; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
	void set_params(std::any params) override;
};

struct Command_config_params
{
	int m_difficulty_divider;
	int m_fee;
	std::string m_mining_mode;
	int m_round_duration_hours;
};

class Command_create_config_impl : public Command_base_database_sqlite
{
public:

	explicit Command_create_config_impl(sqlite3* handle);

	Type get_type() const override { return Type::create_config; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
	void set_params(std::any params) override;
};

class Command_update_config_impl : public Command_base_database_sqlite
{
public:

	explicit Command_update_config_impl(sqlite3* handle);

	Type get_type() const override { return Type::update_config; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
	void set_params(std::any params) override;
};

class Command_reset_shares_from_accounts_impl : public Command_base_database_sqlite
{
public:

	explicit Command_reset_shares_from_accounts_impl(sqlite3* handle);

	Type get_type() const override { return Type::reset_shares_from_accounts; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
};

struct Command_add_block_params
{
	std::string m_hash;
	int m_height;
	std::string m_type;
	double m_difficulty;
	int m_orphan;
	std::string m_block_finder;
	std::int64_t m_round;
	double m_mainnet_reward;
};

class Command_add_block_impl : public Command_base_database_sqlite
{
public:

	explicit Command_add_block_impl(sqlite3* handle);

	Type get_type() const override { return Type::add_block; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
	void set_params(std::any params) override;
 };

struct Command_update_block_reward_params
{
	int m_orphan;
	double m_mainnet_reward;
	std::string m_hash;
};

class Command_update_block_rewards_impl : public Command_base_database_sqlite
{
public:

	explicit Command_update_block_rewards_impl(sqlite3* handle);

	Type get_type() const override { return Type::update_block_rewards; }
	std::any get_command() const override { return Command_type_sqlite{ {m_stmt}, {}, Command_type_sqlite::Type::no_result }; }
	void set_params(std::any params) override;
};

}
}
}

#endif 
