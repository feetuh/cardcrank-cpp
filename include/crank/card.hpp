// Copyright (c) 2026 feetuh. All Rights Reserved.

#include <cstdint>

namespace crank
{

enum class Rank : uint8_t
{
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  SEVEN = 7,
  EIGHT = 8,
  NINE = 9,
  TEN = 10,
  JACK = 11,
  QUEEN = 12,
  KING = 13,
  ACE = 14,
};

class Card
{
public:
  explicit Card(Rank rank);

  Rank rank;
};
}  // namespace crank
