#pragma once
// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/lib.hpp"
#include <vector>
#include <optional>

namespace crank
{

/**
 * @brief TUI wrapper for Card Crank game
 *
 * This class bridges the GameLibrary and TUI components,
 * handling TUI-specific state management and converting
 * user input to game actions.
 */
class TUIGameWrapper
{
public:
    explicit TUIGameWrapper(GameConfig config);
    TUIGameWrapper() = delete;

    // Game control
    void start();

    // Player actions - return true if successful
    auto play_selected_cards() -> bool;
    auto pass_take_three() -> bool;
    auto pass_take_all() -> bool;

    // State queries for TUI rendering
    auto get_view() const -> GameView;
    auto get_current_hand() const -> std::vector<Card>;
    auto get_stack() const -> std::vector<Card>;
    auto get_valid_plays() const -> std::vector<Play>;

    // Card selection management
    auto toggle_card_selection(size_t card_index) -> void;
    auto clear_selection() -> void;
    auto get_selected_indices() const -> std::vector<size_t>;

    // Game flow
    auto is_game_over() const -> bool;
    auto get_winner() const -> std::optional<std::string>;

    // Helper methods
    auto get_current_player_index() const -> size_t;
    auto get_player_names() const -> const std::vector<std::string>&;

private:
    GameLibrary game_library_;
    std::mt19937 rng_;

    // TUI-specific state
    std::vector<bool> card_selection_;
    bool game_started_;

    // Helper methods
    void update_selection_size();
    void ensure_current_player_hand();
};

}  // namespace crank