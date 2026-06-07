#pragma once
// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/card.hpp"
#include <deque>
#include <vector>
#include <string>
#include <optional>
#include <algorithm>

namespace crank
{

enum class PlayType
{
  SINGLE,
  FOUR_OF_A_KIND,
  THREE_NINES_SPECIAL
};

struct Play
{
  std::vector<Card> cards;
  PlayType type;
};

class Hand
{
public:
  Hand() = default;

  void add(Card card);
  void add(std::vector<Card> cards);
  bool remove(Card card);
  bool remove(std::vector<Card> cards);
  bool contains(Card card) const;
  bool contains_four_of_kind(Rank rank) const;
  bool contains_three_nines_excluding_hearts() const;
  std::size_t size() const;
  bool is_empty() const;

  const std::vector<Card>& cards() const { return cards_; }

private:
  std::vector<Card> cards_;
};

class Stack
{
public:
  Stack() = default;

  // Stack operations
  void place(Card card);
  void place(std::vector<Card> cards);
  std::vector<Card> take_three();
  std::vector<Card> take_all();

  // 9♥ handling
  bool contains_nine_of_hearts() const;
  void ensure_nine_hearts_at_base();

  // Query
  std::size_t size() const;
  bool is_empty() const;
  Card top_card() const;
  PlayType last_play_type() const;
  bool is_only_nine_hearts() const;
  Rank top_rank() const;

  const std::deque<Card>& cards() const { return cards_; }

private:
  std::deque<Card> cards_;
  PlayType last_play_type_ = PlayType::SINGLE;
};

class Player
{
public:
  explicit Player(std::string name);

  const std::string& name() const { return name_; }
  Hand& hand() { return hand_; }
  const Hand& hand() const { return hand_; }
  bool has_empty_hand() const;

private:
  std::string name_;
  Hand hand_;
};

class GameState
{
public:
  explicit GameState(std::size_t player_count);
  void start_game();

  // Turn management
  Player& current_player();
  const Player& current_player() const;
  Player& next_player();
  void advance_turn();
  void grant_bonus_turn();

  // Game flow
  bool is_valid_play(const Play& play) const;
  void execute_play(const Play& play);
  void execute_pass_take_three();
  void execute_pass_take_all();

  // Win condition
  std::optional<std::string> winner() const;
  bool is_game_over() const;

  // Query
  const Stack& stack() const { return stack_; }
  std::size_t player_count() const { return players_.size(); }
  Player& player(std::size_t index);
  const Player& player(std::size_t index) const;
  std::size_t current_player_index() const { return current_player_index_; }
  bool nine_hearts_played() const { return nine_hearts_played_; }

private:
  std::vector<Player> players_;
  std::size_t current_player_index_ = 0;
  Stack stack_;
  bool nine_hearts_played_ = false;
  std::optional<std::string> winner_;
  bool bonus_turn_active_ = false;
};

}  // namespace crank
