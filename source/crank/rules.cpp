// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/rules.hpp"
#include <algorithm>

namespace crank
{

bool Rules::is_valid_play(const Stack& stack, const Play& play, bool nine_hearts_played)
{
  switch (play.type) {
    case PlayType::SINGLE:
      if (play.cards.size() != 1) {
        return false;
      }
      return is_single_valid(stack, play.cards[0], nine_hearts_played);

    case PlayType::FOUR_OF_A_KIND:
      if (play.cards.size() != 4) {
        return false;
      }
      if (!is_four_of_kind(play.cards)) {
        return false;
      }
      return is_four_of_kind_valid(stack, play.cards[0].rank(), nine_hearts_played);

    case PlayType::THREE_NINES_SPECIAL:
      if (play.cards.size() != 3) {
        return false;
      }
      return is_three_nines_excluding_hearts(play.cards);

    default:
      return false;
  }
}

bool Rules::is_single_valid(const Stack& stack, Card card, bool nine_hearts_played)
{
  // If stack is empty and 9♥ hasn't been played yet, only 9♥ is valid
  if (stack.is_empty() && !nine_hearts_played) {
    return card.is_nine_of_hearts();
  }

  if (stack.is_empty()) {
    return true;  // Any card is valid on empty stack after 9♥ has been played
  }

  Rank top_rank = get_top_rank(stack);
  return card.rank() >= top_rank;
}

bool Rules::is_four_of_kind_valid(const Stack& stack, Rank rank, bool nine_hearts_played)
{
  // 4-of-a-kind is never valid as the first play (9♥ must be played first)
  if (stack.is_empty() && !nine_hearts_played) {
    return false;
  }

  if (stack.is_empty()) {
    return true;  // Any 4-of-a-kind is valid on empty stack after 9♥ has been played
  }

  Rank top_rank = get_top_rank(stack);
  return rank > top_rank;  // Must be HIGHER (not equal)
}

bool Rules::is_three_nines_special_valid(const Stack& stack, const Hand& hand)
{
  // Only valid when stack is only 9♥
  if (!stack.is_only_nine_hearts()) {
    return false;
  }

  // Hand must contain the other three 9s
  return hand.contains_three_nines_excluding_hearts();
}

bool Rules::must_place_nine_hearts_back(const std::vector<Card>& cards_taken)
{
  Card nine_hearts(Rank::NINE, Suit::HEARTS);
  return std::find(cards_taken.begin(), cards_taken.end(), nine_hearts) != cards_taken.end();
}

Rank Rules::get_top_rank(const Stack& stack)
{
  if (stack.is_empty()) {
    throw std::runtime_error("Cannot get top rank from empty stack");
  }
  return stack.top_rank();
}

bool Rules::is_four_of_kind(const std::vector<Card>& cards)
{
  if (cards.size() != 4) {
    return false;
  }

  // Check if all cards have the same rank
  Rank rank = cards[0].rank();
  for (const auto& card : cards) {
    if (card.rank() != rank) {
      return false;
    }
  }
  return true;
}

bool Rules::is_three_nines_excluding_hearts(const std::vector<Card>& cards)
{
  if (cards.size() != 3) {
    return false;
  }

  // Check if all are 9s and none is 9♥
  for (const auto& card : cards) {
    if (card.rank() != Rank::NINE || card.suit() == Suit::HEARTS) {
      return false;
    }
  }
  return true;
}

}  // namespace crank