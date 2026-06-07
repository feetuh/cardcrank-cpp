#pragma once
// Copyright (c) 2026 feetuh. All Rights Reserved.

#include <string>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "crank/lib.hpp"
#include "crank/tui_wrapper.hpp"

namespace crank
{

/**
 * @brief Simple TUI UI components for Card Crank game
 *
 * This class provides a simplified TUI implementation for the game.
 */
class TUIComponents
{
public:
  TUIComponents() = delete;

  // Main game loop
  static void run_game_loop(TUIGameWrapper& game);

private:
  // UI Helpers
  static auto render_game_screen(const TUIGameWrapper& game) -> ftxui::Element;
  static auto render_player_info(const GameView& view) -> ftxui::Element;
  static auto render_stack_display(const std::vector<Card>& stack)
      -> ftxui::Element;
  static auto render_hand_display(const std::vector<Card>& hand,
                                  const std::vector<size_t>& selected,
                                  size_t highlighted_index,
                                  const std::vector<Play>& valid_plays)
      -> ftxui::Element;
  static auto render_valid_moves(const std::vector<Play>& valid_plays)
      -> ftxui::Element;
  static auto render_controls() -> ftxui::Element;

  // Card rendering helpers
  static auto render_card(const Card& card, bool selected, bool is_valid_move)
      -> ftxui::Element;
  static auto get_card_symbol(Suit suit) -> std::string;
  static auto get_rank_display(Rank rank) -> std::string;
};

}  // namespace crank
