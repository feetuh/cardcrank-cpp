// Copyright (c) 2026 feetuh. All Rights Reserved.

#pragma once

#include <vector>
#include <random>

#include "crank/card.hpp"

namespace crank
{

class Deck : public std::vector<Card>
{
public:
  Deck() = default;

  // Build the game deck (9-A of each suit, 24 cards total)
  static Deck build_game_deck();

  // Shuffle the deck
  void shuffle(std::mt19937& rng);

  // Deal cards evenly to n players (returns vector of hands)
  std::vector<std::vector<Card>> deal(std::size_t player_count);
};

}  // namespace crank
