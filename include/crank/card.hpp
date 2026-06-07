// Copyright (c) 2026 feetuh. All Rights Reserved.

#pragma once

#include <cstdint>
#include <ostream>

namespace crank
{

enum class Rank : uint8_t
{
  NINE = 9,
  TEN = 10,
  JACK = 11,
  QUEEN = 12,
  KING = 13,
  ACE = 14,
};

enum class Suit : uint8_t
{
  HEARTS,
  DIAMONDS,
  CLUBS,
  SPADES,
};

class Card
{
public:
  Card(Rank rank, Suit suit);

  Rank rank() const { return rank_; }
  Suit suit() const { return suit_; }

  // Comparison operators (rank only)
  bool operator==(const Card& other) const;
  bool operator!=(const Card& other) const;
  bool operator<(const Card& other) const;
  bool operator<=(const Card& other) const;
  bool operator>(const Card& other) const;
  bool operator>=(const Card& other) const;

  // Special check for 9 of Hearts
  bool is_nine_of_hearts() const;

  // Friend for output
  friend std::ostream& operator<<(std::ostream& os, const Card& card);

private:
  Rank rank_;
  Suit suit_;
};

}  // namespace crank
