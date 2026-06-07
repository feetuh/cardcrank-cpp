// Copyright (c) 2026 feetuh. All Rights Reserved.

#pragma once

#include "crank/card.hpp"
#include "crank/table.hpp"
#include <vector>

namespace crank
{

class Rules
{
public:
  // Play validation
  static bool is_valid_play(const Stack& stack, const Play& play, bool nine_hearts_played);
  static bool is_single_valid(const Stack& stack, Card card, bool nine_hearts_played);
  static bool is_four_of_kind_valid(const Stack& stack, Rank rank, bool nine_hearts_played);

  // Special cases
  static bool is_three_nines_special_valid(const Stack& stack, const Hand& hand);

  // 9♥ rules
  static bool must_place_nine_hearts_back(const std::vector<Card>& cards_taken);

  // Helper methods
  static Rank get_top_rank(const Stack& stack);
  static bool is_four_of_kind(const std::vector<Card>& cards);
  static bool is_three_nines_excluding_hearts(const std::vector<Card>& cards);
};

}  // namespace crank