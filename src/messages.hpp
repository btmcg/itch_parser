#pragma once

#include <cstdint>


namespace itch {

    inline namespace v5_0 {

        constexpr std::uint8_t StockLen = 8;
        constexpr std::uint8_t TimestampLen = 6;

        struct header
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
        }  __attribute__((packed));

        struct system_event
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char event_code;
        }  __attribute__((packed));

        struct stock_directory
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char stock[8];
            char market_category;
            char financial_status_indicator;
            std::uint32_t round_lot_size;
            char round_lots_only;
            char issue_classification;
            char issue_subtype[2];
            char authenticity;
            char short_sale_threshold_indicator;
            char ipo_flag;
            char luld_reference_price_tier;
            char etp_flag;
            std::uint32_t etp_leverage_factor;
            char inverse_indicator;
        }  __attribute__((packed));

        struct stock_trading_action
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char stock[8];
            char trading_state;
            char reserved;
            char reason[4];
        }  __attribute__((packed));

        struct reg_sho_restriction
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char stock[8];
            char reg_sho_action;
        }  __attribute__((packed));

        struct market_participant_position
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char mpid[4];
            char stock[8];
            char primary_market_maker;
            char market_maker_mode;
            char market_participant_state;
        }  __attribute__((packed));

        struct mwcb_decline_level
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t level1;
            std::uint64_t level2;
            std::uint64_t level3;
        }  __attribute__((packed));

        struct mwcb_status
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char breached_level;
        }  __attribute__((packed));

        struct ipo_quoting_period_update
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char stock[8];
            std::uint32_t ipo_quotation_release_time;
            char ipo_quotation_release_qualifier;
            std::uint32_t ipo_price;
        }  __attribute__((packed));

        struct luld_auction_collar
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char stock[8];
            std::uint32_t auction_collar_reference_price;
            std::uint32_t upper_auction_collar_price;
            std::uint32_t lower_auction_collar_price;
            std::uint32_t auction_collar_extension;
        }  __attribute__((packed));

        struct operational_halt
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            char stock[8];
            std::uint32_t market_code;
            std::uint32_t operational_halt_action;
        }  __attribute__((packed));

        struct add_order
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t order_reference_number;
            char buy_sell_indicator;
            std::uint32_t shares;
            char stock[8];
            std::uint32_t price;
        }  __attribute__((packed));

        struct add_order_with_mpid
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t order_reference_number;
            char buy_sell_indicator;
            std::uint32_t shares;
            char stock[8];
            std::uint32_t price;
            char attribution[4];
        }  __attribute__((packed));

        struct order_executed
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t order_reference_number;
            std::uint32_t executed_shares;
            std::uint64_t match_number;
        }  __attribute__((packed));

        struct order_executed_with_price
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t order_reference_number;
            std::uint32_t executed_shares;
            std::uint64_t match_number;
            char printable;
            std::uint32_t execution_price;
        }  __attribute__((packed));

        struct order_cancel
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t order_reference_number;
            std::uint32_t cancelled_shares;
        }  __attribute__((packed));

        struct order_delete
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t order_reference_number;
        }  __attribute__((packed));

        struct order_replace
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t original_order_reference_number;
            std::uint64_t new_order_reference_number;
            std::uint32_t shares;
            std::uint32_t price;
        }  __attribute__((packed));

        struct trade_non_cross
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t order_reference_number;
            char buy_sell_indicator;
            std::uint32_t shares;
            char stock[8];
            std::uint32_t price;
            std::uint64_t match_number;
        }  __attribute__((packed));

        struct trade_cross
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t shares;
            char stock[8];
            std::uint32_t cross_price;
            std::uint64_t match_number;
            char cross_type;
        }  __attribute__((packed));

        struct broken_trade
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t match_number;
        }  __attribute__((packed));

        struct noii
        {
            std::uint16_t length;
            char msg_type;
            std::uint16_t stock_locate;
            std::uint16_t tracking_number;
            std::uint8_t timestamp[6];
            std::uint64_t paired_shares;
            std::uint64_t imbalance_shares;
            char imbalance_direction;
            char stock[8];
            std::uint32_t far_price;
            std::uint32_t near_price;
            std::uint32_t current_reference_price;
            char cross_type;
            char price_variation_indicator;
        }  __attribute__((packed));

    } // namespace v5_0

} // namespace itch
