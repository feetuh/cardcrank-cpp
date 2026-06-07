// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/lib.hpp"
#include "crank/rules.hpp"
#include <stdexcept>
#include <algorithm>

namespace crank
{

GameLibrary::GameLibrary(GameConfig config)
    : game_state_(config.player_count), config_(config), game_started_(false)
{
  // Initialize RNG with seed if provided
  if (config.seed.has_value()) {
    rng_.seed(config.seed.value());
  } else {
    std::random_device rd;
    rng_.seed(rd());
  }
}

void GameLibrary::start()
{
  if (game_started_) {
    throw std::runtime_error("Game already started");
  }

  // Build and shuffle deck
  auto deck = Deck::build_game_deck();
  deck.shuffle(rng_);

  // Deal cards to players
  auto hands = deck.deal(config_.player_count);

  // Distribute cards to players
  for (std::size_t i = 0; i < hands.size(); ++i) {
    game_state_.player(i).hand().add(hands[i]);
  }

  // Find player with 9♥ and set as current player
  game_state_.start_game();

  game_started_ = true;
}

GameView GameLibrary::get_view() const
{
  GameView view;
  view.player_names.reserve(game_state_.player_count());
  view.hand_sizes.reserve(game_state_.player_count());

  for (std::size_t i = 0; i < game_state_.player_count(); ++i) {
    view.player_names.push_back(game_state_.player(i).name());
    view.hand_sizes.push_back(game_state_.player(i).hand().size());
  }

  view.current_player_index = game_state_.current_player_index();
  view.stack_size = game_state_.stack().size();
  view.winner = game_state_.winner();
  view.bonus_turn_active = false;  // We can add this to GameState if needed

  return view;
}

bool GameLibrary::play_card(std::size_t player_id, const std::vector<std::size_t>& card_indices)
{
  if (!game_started_) {
    return false;
  }

  // Verify it's this player's turn
  if (player_id != game_state_.current_player_index()) {
    return false;
  }

  try {
    Play play = create_play(card_indices, player_id);
    game_state_.execute_play(play);
    return true;
  } catch (...) {
    return false;
  }
}

bool GameLibrary::pass_take_three(std::size_t player_id)
{
  if (!game_started_) {
    return false;
  }

  // Verify it's this player's turn
  if (player_id != game_state_.current_player_index()) {
    return false;
  }

  try {
    game_state_.execute_pass_take_three();
    return true;
  } catch (...) {
    return false;
  }
}

bool GameLibrary::pass_take_all(std::size_t player_id)
{
  if (!game_started_) {
    return false;
  }

  // Verify it's this player's turn
  if (player_id != game_state_.current_player_index()) {
    return false;
  }

  try {
    game_state_.execute_pass_take_all();
    return true;
  } catch (...) {
    return false;
  }
}

std::optional<std::string> GameLibrary::winner() const
{
  return game_state_.winner();
}

bool GameLibrary::is_game_over() const
{
  return game_state_.is_game_over();
}

std::vector<Play> GameLibrary::get_valid_plays(std::size_t player_id) const
{
  if (!game_started_ || player_id != game_state_.current_player_index()) {
    return {};
  }

  std::vector<Play> valid_plays;
  const auto& hand = game_state_.player(player_id).hand();
  const auto& cards = hand.cards();

  // Check all single plays
  for (std::size_t i = 0; i < cards.size(); ++i) {
    Play play;
    play.cards = {cards[i]};
    play.type = PlayType::SINGLE;

    if (game_state_.is_valid_play(play)) {
      valid_plays.push_back(play);
    }
  }

  // Check all 4-of-a-kind plays
  for (Rank rank : {Rank::NINE, Rank::TEN, Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE}) {
    if (hand.contains_four_of_kind(rank)) {
      // Find all cards of this rank
      std::vector<Card> four_cards;
      for (const auto& card : cards) {
        if (card.rank() == rank) {
          four_cards.push_back(card);
        }
      }

      if (four_cards.size() == 4) {
        Play play;
        play.cards = four_cards;
        play.type = PlayType::FOUR_OF_A_KIND;

        if (game_state_.is_valid_play(play)) {
          valid_plays.push_back(play);
        }
      }
    }
  }

  // Check special three 9s play
  if (hand.contains_three_nines_excluding_hearts() &&
      game_state_.stack().is_only_nine_hearts()) {
    // Find all 9s except 9♥
    std::vector<Card> three_cards;
    for (const auto& card : cards) {
      if (card.rank() == Rank::NINE && card.suit() != Suit::HEARTS) {
        three_cards.push_back(card);
      }
    }

    if (three_cards.size() == 3) {
      Play play;
      play.cards = three_cards;
      play.type = PlayType::THREE_NINES_SPECIAL;
      valid_plays.push_back(play);
    }
  }

  return valid_plays;
}

const std::vector<Card>& GameLibrary::get_player_hand(std::size_t player_id) const
{
  return game_state_.player(player_id).hand().cards();
}

const std::deque<Card>& GameLibrary::get_stack_cards() const
{
  return game_state_.stack().cards();
}

Play GameLibrary::create_play(const std::vector<std::size_t>& card_indices, std::size_t player_id) const
{
  const auto& hand = game_state_.player(player_id).hand().cards();

  // Create play from card indices
  Play play;
  play.cards.reserve(card_indices.size());

  for (std::size_t index : card_indices) {
    if (index >= hand.size()) {
      throw std::out_of_range("Card index out of range");
    }
    play.cards.push_back(hand[index]);
  }

  // Determine play type
  if (play.cards.size() == 1) {
    play.type = PlayType::SINGLE;
  } else if (play.cards.size() == 4 && Rules::is_four_of_kind(play.cards)) {
    play.type = PlayType::FOUR_OF_A_KIND;
  } else if (play.cards.size() == 3 && Rules::is_three_nines_excluding_hearts(play.cards)) {
    play.type = PlayType::THREE_NINES_SPECIAL;
  } else {
    throw std::invalid_argument("Invalid play: cards don't form a valid play type");
  }

  // Validate play
  if (!game_state_.is_valid_play(play)) {
    throw std::invalid_argument("Invalid play: doesn't meet game rules");
  }

  return play;
}

}  // namespace crank
