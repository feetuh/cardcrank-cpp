#pragma once
// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/table.hpp"
#include "crank/deck.hpp"
#include <string>
#include <vector>
#include <optional>
#include <random>

namespace crank
{

/**
 * @brief Configuration for creating a new game
 */
struct GameConfig
{
  std::size_t player_count;
  std::optional<std::uint32_t> seed;  // For reproducible games
};

/**
 * @brief Read-only view of the game state for UI rendering
 */
struct GameView
{
  std::vector<std::string> player_names;
  std::vector<std::size_t> hand_sizes;
  std::size_t current_player_index;
  std::size_t stack_size;
  std::optional<std::string> winner;
  bool bonus_turn_active;
};

/**
 * @brief Main game library interface for wrapping (terminal GUI, HTTP, etc.)
 */
class GameLibrary
{
public:
  explicit GameLibrary(GameConfig config);
  GameLibrary() = delete;

  // Game control
  void start();
  GameView get_view() const;

  // Player actions - return true if successful
  bool play_card(std::size_t player_id, const std::vector<std::size_t>& card_indices);
  bool pass_take_three(std::size_t player_id);
  bool pass_take_all(std::size_t player_id);

  // Query
  const GameState& state() const { return game_state_; }
  std::optional<std::string> winner() const;
  bool is_game_over() const;

  // Validation (for UI to show valid moves)
  std::vector<Play> get_valid_plays(std::size_t player_id) const;

  // Get player's hand for rendering
  const std::vector<Card>& get_player_hand(std::size_t player_id) const;
  const std::deque<Card>& get_stack_cards() const;

private:
  GameState game_state_;
  GameConfig config_;
  std::mt19937 rng_;
  bool game_started_;

  Play create_play(const std::vector<std::size_t>& card_indices, std::size_t player_id) const;
};

}  // namespace crank
