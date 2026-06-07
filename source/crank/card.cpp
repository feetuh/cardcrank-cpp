// Copyright (c) 2026 feetuh. All Rights Reserved.

#include "crank/card.hpp"
#include <algorithm>

namespace crank
{

Card::Card(Rank rank, Suit suit)
    : rank_(rank), suit_(suit)
{
}

bool Card::operator==(const Card& other) const
{
  return rank_ == other.rank_;
}

bool Card::operator!=(const Card& other) const
{
  return rank_ != other.rank_;
}

bool Card::operator<(const Card& other) const
{
  return rank_ < other.rank_;
}

bool Card::operator<=(const Card& other) const
{
  return rank_ <= other.rank_;
}

bool Card::operator>(const Card& other) const
{
  return rank_ > other.rank_;
}

bool Card::operator>=(const Card& other) const
{
  return rank_ >= other.rank_;
}

bool Card::is_nine_of_hearts() const
{
  return rank_ == Rank::NINE && suit_ == Suit::HEARTS;
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
  const char* rank_names[] = {"9", "10", "J", "Q", "K", "A"};
  const char* suit_symbols[] = {"♥", "♦", "♣", "♠"};

  os << rank_names[static_cast<int>(card.rank_) - 9] << suit_symbols[static_cast<int>(card.suit_)];
  return os;
}

}  // namespace crank