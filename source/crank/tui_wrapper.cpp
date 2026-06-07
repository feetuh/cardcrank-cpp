// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/tui_wrapper.hpp"
#include <algorithm>
#include <vector>
#include <optional>

namespace crank
{

TUIGameWrapper::TUIGameWrapper(GameConfig config)
    : game_library_(config),
      rng_(config.seed.value_or(std::random_device{}())),
      game_started_(false)
{
}

void TUIGameWrapper::start()
{
    game_library_.start();
    game_started_ = true;
    update_selection_size();
}

auto TUIGameWrapper::play_selected_cards() -> bool
{
    if (!game_started_) {
        return false;
    }

    const auto current_player = get_current_player_index();
    const auto selected_indices = get_selected_indices();

    if (selected_indices.empty()) {
        return false;
    }

    return game_library_.play_card(current_player, selected_indices);
}

auto TUIGameWrapper::pass_take_three() -> bool
{
    if (!game_started_) {
        return false;
    }

    const auto current_player = get_current_player_index();
    return game_library_.pass_take_three(current_player);
}

auto TUIGameWrapper::pass_take_all() -> bool
{
    if (!game_started_) {
        return false;
    }

    const auto current_player = get_current_player_index();
    return game_library_.pass_take_all(current_player);
}

auto TUIGameWrapper::get_view() const -> GameView
{
    if (!game_started_) {
        return {};
    }
    return game_library_.get_view();
}

auto TUIGameWrapper::get_current_hand() const -> std::vector<Card>
{
    if (!game_started_) {
        return {};
    }

    const auto current_player = get_current_player_index();
    return game_library_.get_player_hand(current_player);
}

auto TUIGameWrapper::get_stack() const -> std::vector<Card>
{
    if (!game_started_) {
        return {};
    }
    const auto& stack_deque = game_library_.get_stack_cards();
    return std::vector<Card>(stack_deque.begin(), stack_deque.end());
}

auto TUIGameWrapper::get_valid_plays() const -> std::vector<Play>
{
    if (!game_started_) {
        return {};
    }

    const auto current_player = get_current_player_index();
    return game_library_.get_valid_plays(current_player);
}

void TUIGameWrapper::toggle_card_selection(size_t card_index)
{
    if (!game_started_) {
        return;
    }

    ensure_current_player_hand();

    if (card_index < card_selection_.size()) {
        card_selection_[card_index] = !card_selection_[card_index];
    }
}

void TUIGameWrapper::clear_selection()
{
    std::fill(card_selection_.begin(), card_selection_.end(), false);
}

auto TUIGameWrapper::get_selected_indices() const -> std::vector<size_t>
{
    std::vector<size_t> selected;
    selected.reserve(card_selection_.size());

    for (size_t i = 0; i < card_selection_.size(); ++i) {
        if (card_selection_[i]) {
            selected.push_back(i);
        }
    }

    return selected;
}

auto TUIGameWrapper::is_game_over() const -> bool
{
    if (!game_started_) {
        return false;
    }
    return game_library_.is_game_over();
}

auto TUIGameWrapper::get_winner() const -> std::optional<std::string>
{
    if (!game_started_) {
        return std::nullopt;
    }
    return game_library_.winner();
}

auto TUIGameWrapper::get_current_player_index() const -> size_t
{
    if (!game_started_) {
        return 0;
    }
    return game_library_.get_view().current_player_index;
}

auto TUIGameWrapper::get_player_names() const -> const std::vector<std::string>&
{
    if (!game_started_) {
        static const std::vector<std::string> empty;
        return empty;
    }
    return game_library_.get_view().player_names;
}

void TUIGameWrapper::update_selection_size()
{
    const auto hand = get_current_hand();
    const auto new_size = hand.size();

    if (card_selection_.size() != new_size) {
        card_selection_.resize(new_size, false);
    }
}

void TUIGameWrapper::ensure_current_player_hand()
{
    update_selection_size();
}

}  // namespace crank