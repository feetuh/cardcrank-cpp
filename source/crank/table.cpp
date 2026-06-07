// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/table.hpp"
#include "crank/rules.hpp"
#include <stdexcept>
#include <algorithm>

namespace crank
{

// ============================================================================
// Hand Implementation
// ============================================================================

namespace
{
  // Comparator for sorting cards: by rank ascending, then by suit
  auto card_compare(const Card& lhs, const Card& rhs) -> bool
  {
    if (lhs.rank() != rhs.rank()) {
      return lhs.rank() < rhs.rank();
    }
    return lhs.suit() < rhs.suit();
  }
}  // namespace

void Hand::add(Card card)
{
  cards_.push_back(card);
  std::sort(cards_.begin(), cards_.end(), card_compare);
}

void Hand::add(std::vector<Card> cards)
{
  cards_.insert(cards_.end(), cards.begin(), cards.end());
  std::sort(cards_.begin(), cards_.end(), card_compare);
}

bool Hand::remove(Card card)
{
  auto it = std::find(cards_.begin(), cards_.end(), card);
  if (it != cards_.end()) {
    cards_.erase(it);
    return true;
  }
  return false;
}

bool Hand::remove(std::vector<Card> cards)
{
  // Check if all cards exist in hand
  for (const auto& card : cards) {
    auto it = std::find(cards_.begin(), cards_.end(), card);
    if (it == cards_.end()) {
      return false;  // Card not found
    }
  }

  // Remove all cards
  for (const auto& card : cards) {
    remove(card);
  }
  return true;
}

bool Hand::contains(Card card) const
{
  return std::find(cards_.begin(), cards_.end(), card) != cards_.end();
}

bool Hand::contains_four_of_kind(Rank rank) const
{
  int count = 0;
  for (const auto& card : cards_) {
    if (card.rank() == rank) {
      ++count;
    }
  }
  return count == 4;
}

bool Hand::contains_three_nines_excluding_hearts() const
{
  int count = 0;
  for (const auto& card : cards_) {
    if (card.rank() == Rank::NINE && card.suit() != Suit::HEARTS) {
      ++count;
    }
  }
  return count == 3;
}

std::size_t Hand::size() const
{
  return cards_.size();
}

bool Hand::is_empty() const
{
  return cards_.empty();
}

// ============================================================================
// Stack Implementation
// ============================================================================

void Stack::place(Card card)
{
  cards_.push_back(card);
  last_play_type_ = PlayType::SINGLE;
}

void Stack::place(std::vector<Card> cards)
{
  for (const auto& card : cards) {
    cards_.push_back(card);
  }

  // Determine play type based on number of cards
  if (cards.size() == 4) {
    last_play_type_ = PlayType::FOUR_OF_A_KIND;
  } else if (cards.size() == 3) {
    last_play_type_ = PlayType::THREE_NINES_SPECIAL;
  } else {
    last_play_type_ = PlayType::SINGLE;
  }
}

std::vector<Card> Stack::take_three()
{
  std::vector<Card> taken;

  if (cards_.empty()) {
    return taken;
  }

  // Take up to 3 cards from the top (back of deque)
  std::size_t take_count = std::min(static_cast<std::size_t>(3), cards_.size());

  for (std::size_t i = 0; i < take_count; ++i) {
    taken.push_back(cards_.back());
    cards_.pop_back();
  }

  // Reverse to maintain order (top card first)
  std::reverse(taken.begin(), taken.end());

  return taken;
}

std::vector<Card> Stack::take_all()
{
  std::vector<Card> taken(cards_.begin(), cards_.end());
  cards_.clear();
  return taken;
}

bool Stack::contains_nine_of_hearts() const
{
  for (const auto& card : cards_) {
    if (card.is_nine_of_hearts()) {
      return true;
    }
  }
  return false;
}

void Stack::ensure_nine_hearts_at_base()
{
  // Find 9♥
  Card nine_hearts(Rank::NINE, Suit::HEARTS);
  auto it = std::find(cards_.begin(), cards_.end(), nine_hearts);

  // If 9♥ is not at the front, move it there
  if (it != cards_.end() && it != cards_.begin()) {
    cards_.erase(it);
    cards_.push_front(nine_hearts);
  }
}

std::size_t Stack::size() const
{
  return cards_.size();
}

bool Stack::is_empty() const
{
  return cards_.empty();
}

Card Stack::top_card() const
{
  if (cards_.empty()) {
    throw std::runtime_error("Cannot get top card from empty stack");
  }
  return cards_.back();
}

PlayType Stack::last_play_type() const
{
  return last_play_type_;
}

bool Stack::is_only_nine_hearts() const
{
  return cards_.size() == 1 && cards_.front().is_nine_of_hearts();
}

Rank Stack::top_rank() const
{
  if (cards_.empty()) {
    throw std::runtime_error("Cannot get top rank from empty stack");
  }
  return cards_.back().rank();
}

// ============================================================================
// Player Implementation
// ============================================================================

Player::Player(std::string name)
    : name_(std::move(name))
{
}

bool Player::has_empty_hand() const
{
  return hand_.is_empty();
}

// ============================================================================
// GameState Implementation
// ============================================================================

GameState::GameState(std::size_t player_count)
{
  if (player_count < 2 || player_count > 4) {
    throw std::invalid_argument("Player count must be between 2 and 4");
  }

  // Create players
  for (std::size_t i = 0; i < player_count; ++i) {
    players_.emplace_back("Player " + std::to_string(i + 1));
  }
}

void GameState::start_game()
{
  // Find player with 9♥
  for (std::size_t i = 0; i < players_.size(); ++i) {
    auto& hand = players_[i].hand();
    for (const auto& card : hand.cards()) {
      if (card.is_nine_of_hearts()) {
        current_player_index_ = i;
        nine_hearts_played_ = false;
        return;
      }
    }
  }
}

Player& GameState::current_player()
{
  return players_[current_player_index_];
}

const Player& GameState::current_player() const
{
  return players_[current_player_index_];
}

Player& GameState::next_player()
{
  std::size_t next_index = (current_player_index_ + 1) % players_.size();
  return players_[next_index];
}

void GameState::advance_turn()
{
  if (bonus_turn_active_) {
    // Bonus turn ends, don't advance
    bonus_turn_active_ = false;
  } else {
    // Normal turn, advance clockwise
    current_player_index_ = (current_player_index_ + 1) % players_.size();
  }
}

void GameState::grant_bonus_turn()
{
  bonus_turn_active_ = true;
}

bool GameState::is_valid_play(const Play& play) const
{
  return Rules::is_valid_play(stack_, play);
}

void GameState::execute_play(const Play& play)
{
  // Validate play
  if (!is_valid_play(play)) {
    throw std::invalid_argument("Invalid play");
  }

  // Remove cards from hand
  auto& hand = current_player().hand();
  if (!hand.remove(play.cards)) {
    throw std::runtime_error("Failed to remove cards from hand");
  }

  // Place cards on stack
  stack_.place(play.cards);

  // Check for win condition
  if (hand.is_empty() && !winner_.has_value()) {
    winner_ = current_player().name();
  }

  // Check for bonus turn (4-of-a-kind or three 9s special)
  if (play.type == PlayType::FOUR_OF_A_KIND || play.type == PlayType::THREE_NINES_SPECIAL) {
    grant_bonus_turn();
  } else {
    advance_turn();
  }
}

void GameState::execute_pass_take_three()
{
  // Take three cards from stack
  auto taken = stack_.take_three();

  // Check if 9♥ was taken and needs to be placed back
  if (Rules::must_place_nine_hearts_back(taken)) {
    stack_.ensure_nine_hearts_at_base();
    taken.erase(std::remove(taken.begin(), taken.end(), Card(Rank::NINE, Suit::HEARTS)), taken.end());
  }

  // Add cards to current player's hand
  current_player().hand().add(taken);

  advance_turn();
}

void GameState::execute_pass_take_all()
{
  // Take all cards from stack
  auto taken = stack_.take_all();

  // 9♥ must be placed back as base
  stack_.place(Card(Rank::NINE, Suit::HEARTS));
  stack_.ensure_nine_hearts_at_base();

  // Remove 9♥ from taken cards
  taken.erase(std::remove(taken.begin(), taken.end(), Card(Rank::NINE, Suit::HEARTS)), taken.end());

  // Add remaining cards to current player's hand
  current_player().hand().add(taken);

  advance_turn();
}

std::optional<std::string> GameState::winner() const
{
  return winner_;
}

bool GameState::is_game_over() const
{
  // Game is over if we have a winner
  return winner_.has_value();
}

Player& GameState::player(std::size_t index)
{
  if (index >= players_.size()) {
    throw std::out_of_range("Player index out of range");
  }
  return players_[index];
}

const Player& GameState::player(std::size_t index) const
{
  if (index >= players_.size()) {
    throw std::out_of_range("Player index out of range");
  }
  return players_[index];
}

}  // namespace crank