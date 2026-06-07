// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/deck.hpp"
#include <algorithm>

namespace crank
{

Deck Deck::build_game_deck()
{
  Deck deck;

  // Create cards for each suit (9, 10, J, Q, K, A)
  for (Suit suit : {Suit::HEARTS, Suit::DIAMONDS, Suit::CLUBS, Suit::SPADES}) {
    for (Rank rank : {Rank::NINE, Rank::TEN, Rank::JACK, Rank::QUEEN, Rank::KING, Rank::ACE}) {
      deck.emplace_back(rank, suit);
    }
  }

  return deck;
}

void Deck::shuffle(std::mt19937& rng)
{
  std::shuffle(this->begin(), this->end(), rng);
}

std::vector<std::vector<Card>> Deck::deal(std::size_t player_count)
{
  std::vector<std::vector<Card>> hands(player_count);

  // Deal cards round-robin style
  for (std::size_t i = 0; i < this->size(); ++i) {
    hands[i % player_count].push_back((*this)[i]);
  }

  return hands;
}

}  // namespace crank
