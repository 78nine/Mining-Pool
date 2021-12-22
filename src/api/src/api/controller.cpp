#include "api/controller.hpp"


namespace nexuspool
{
namespace api
{

Rest_controller::Rest_controller(Shared_data_reader::Sptr data_reader,
    common::Pool_api_data_exchange::Sptr pool_api_data_exchange,
    std::string auth_user,
    std::string auth_pw,
    std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper)
    : m_data_reader{ std::move(data_reader) }
    , m_pool_api_data_exchange{ std::move(pool_api_data_exchange) }
    , m_auth_user{ std::move(auth_user) }
    , m_auth_pw{ std::move(auth_pw) }
    , oatpp::web::server::api::ApiController(objectMapper)
{
    setDefaultAuthorizationHandler(std::make_shared<BasicAuthorizationHandler>("nexuspool"));
}

persistance::Config_data Rest_controller::get_config_data()
{
    auto config_updated = m_pool_api_data_exchange->is_config_updated();
    if (config_updated)
    {
        m_cached_config = m_data_reader->get_config();
        config_updated = false;
        m_pool_api_data_exchange->set_config_updated(config_updated);
    }
    return m_cached_config;
}

std::shared_ptr<ApiController::OutgoingResponse> Rest_controller::get_meta_infos()
{
    auto dto = Meta_infos_dto::createShared();
    auto const config = get_config_data();

    dto->pool_hashrate = m_data_reader->get_pool_hashrate();
    dto->round_duration = config.m_round_duration_hours;
    dto->fee = config.m_fee;
    dto->mining_mode = config.m_mining_mode.c_str();
    dto->active_miners = m_pool_api_data_exchange->get_active_miners();
    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<ApiController::OutgoingResponse> Rest_controller::get_latest_blocks()
{
    auto dto = Latest_blocks_dto::createShared();
    auto blocks = m_data_reader->get_latest_blocks();
    for (auto& block : blocks)
    {
        dto->blocks->push_back(Block_dto::createShared(block.m_height, block.m_hash.c_str(), block.m_mainnet_reward, block.m_block_found_time.c_str(), block.m_difficulty));
    }

    return createDtoResponse(Status::CODE_200, dto);
}

std::shared_ptr<ApiController::OutgoingResponse> Rest_controller::get_account_header(std::string account)
{
    // is account a valid nxs address  
    TAO::Register::Address address_check{ account };
    if (!address_check.IsValid())
    {
        return createResponse(Status::CODE_400, "invalid account");
    }

    if (m_data_reader->does_account_exists(account))
    {
        return createResponse(Status::CODE_200, "");
    }
    else
    {
        return createResponse(Status::CODE_404, "account doesn't exist");
    }
}

std::shared_ptr<ApiController::OutgoingResponse> Rest_controller::get_account_detail(std::string account)
{
    // is account a valid nxs address  
    TAO::Register::Address address_check{ account };
    if (!address_check.IsValid())
    {
        return createResponse(Status::CODE_400, "invalid account");
    }

    if (m_data_reader->does_account_exists(account))
    {
        auto dto = Account_dto::createShared();
        auto const account_data = m_data_reader->get_account(account);

        dto->account = account_data.m_address;
        dto->created_at = account_data.m_created_at;
        dto->last_active = account_data.m_last_active;
        dto->shares = account_data.m_shares;
        dto->hashrate = account_data.m_hashrate;

        return createDtoResponse(Status::CODE_200, dto);
    }
    else
    {
        return createResponse(Status::CODE_404, "account doesn't exist");
    }
}

std::shared_ptr<ApiController::OutgoingResponse> Rest_controller::get_account_payout(std::string account)
{
    // is account a valid nxs address  
    TAO::Register::Address address_check{ account };
    if (!address_check.IsValid())
    {
        return createResponse(Status::CODE_400, "invalid account");
    }

    if (m_data_reader->does_account_exists(account))
    {
        auto dto = Account_payouts_dto::createShared();
        auto const account_data = m_data_reader->get_account(account);

        auto const payments = m_data_reader->get_payments(account);
        for (auto const& payment : payments)
        {
            dto->payouts->push_back(Payout_dto::createShared(payment.m_payment_date_time.c_str(), payment.m_amount, payment.m_tx_id.c_str()));
        }

        return createDtoResponse(Status::CODE_200, dto);
    }
    else
    {
        return createResponse(Status::CODE_404, "account doesn't exist");
    }
}

}
}