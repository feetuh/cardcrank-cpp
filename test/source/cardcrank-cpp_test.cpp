#include <gtest/gtest.h>

#include "crank/card.hpp"
#include "crank/deck.hpp"
#include "crank/table.hpp"
#include "crank/rules.hpp"
#include "crank/lib.hpp"

using namespace crank;

// ============================================================================
// Card Tests
// ============================================================================

TEST(CardTest, ConstructionAndProperties)
{
    Card card(Rank::KING, Suit::HEARTS);
    EXPECT_EQ(card.rank(), Rank::KING);
    EXPECT_EQ(card.suit(), Suit::HEARTS);
    EXPECT_FALSE(card.is_nine_of_hearts());
}

TEST(CardTest, IsNineOfHearts)
{
    Card nine_hearts(Rank::NINE, Suit::HEARTS);
    EXPECT_TRUE(nine_hearts.is_nine_of_hearts());

    Card nine_spades(Rank::NINE, Suit::SPADES);
    EXPECT_FALSE(nine_spades.is_nine_of_hearts());

    Card king_hearts(Rank::KING, Suit::HEARTS);
    EXPECT_FALSE(king_hearts.is_nine_of_hearts());
}

TEST(CardTest, ComparisonOperators)
{
    Card nine(Rank::NINE, Suit::HEARTS);
    Card ten(Rank::TEN, Suit::SPADES);
    Card king(Rank::KING, Suit::DIAMONDS);

    EXPECT_GT(ten, nine);
    EXPECT_LT(nine, ten);
    EXPECT_GE(king, ten);
    EXPECT_LE(ten, king);

    Card another_ten(Rank::TEN, Suit::CLUBS);
    EXPECT_EQ(ten, another_ten);
    EXPECT_NE(ten, nine);
}

// ============================================================================
// Deck Tests
// ============================================================================

TEST(DeckTest, GameDeckHas24Cards)
{
    auto deck = Deck::build_game_deck();
    EXPECT_EQ(deck.size(), 24);
}

TEST(DeckTest, GameDeckHasCorrectRanks)
{
    auto deck = Deck::build_game_deck();

    // Check all cards are 9-A
    for (const auto& card : deck) {
        EXPECT_GE(card.rank(), Rank::NINE);
        EXPECT_LE(card.rank(), Rank::ACE);
    }
}

TEST(DeckTest, DealsEvenlyTo4Players)
{
    auto deck = Deck::build_game_deck();
    auto hands = deck.deal(4);

    EXPECT_EQ(hands.size(), 4);

    // 24 cards / 4 players = 6 cards each
    for (const auto& hand : hands) {
        EXPECT_EQ(hand.size(), 6);
    }
}

TEST(DeckTest, DealsCorrectlyFor3Players)
{
    auto deck = Deck::build_game_deck();
    auto hands = deck.deal(3);

    EXPECT_EQ(hands.size(), 3);

    // 24 cards / 3 players = 8 cards each
    EXPECT_EQ(hands[0].size(), 8);
    EXPECT_EQ(hands[1].size(), 8);
    EXPECT_EQ(hands[2].size(), 8);
}

// ============================================================================
// Hand Tests
// ============================================================================

TEST(HandTest, AddAndContains)
{
    Hand hand;
    Card card1(Rank::NINE, Suit::HEARTS);
    Card card2(Rank::TEN, Suit::SPADES);

    hand.add(card1);
    EXPECT_TRUE(hand.contains(card1));
    EXPECT_FALSE(hand.contains(card2));

    hand.add(card2);
    EXPECT_TRUE(hand.contains(card2));
}

TEST(HandTest, SizeAndEmpty)
{
    Hand hand;
    EXPECT_EQ(hand.size(), 0);
    EXPECT_TRUE(hand.is_empty());

    hand.add(Card(Rank::NINE, Suit::HEARTS));
    EXPECT_EQ(hand.size(), 1);
    EXPECT_FALSE(hand.is_empty());
}

TEST(HandTest, ContainsFourOfAKind)
{
    Hand hand;
    hand.add(Card(Rank::KING, Suit::HEARTS));
    hand.add(Card(Rank::KING, Suit::DIAMONDS));
    hand.add(Card(Rank::KING, Suit::CLUBS));
    hand.add(Card(Rank::KING, Suit::SPADES));

    EXPECT_TRUE(hand.contains_four_of_kind(Rank::KING));
    EXPECT_FALSE(hand.contains_four_of_kind(Rank::QUEEN));
}

TEST(HandTest, ContainsThreeNinesExcludingHearts)
{
    Hand hand;
    hand.add(Card(Rank::NINE, Suit::DIAMONDS));
    hand.add(Card(Rank::NINE, Suit::CLUBS));
    hand.add(Card(Rank::NINE, Suit::SPADES));

    EXPECT_TRUE(hand.contains_three_nines_excluding_hearts());

    // Adding 9♥ should still return true (we check for other 3)
    hand.add(Card(Rank::NINE, Suit::HEARTS));
    EXPECT_TRUE(hand.contains_three_nines_excluding_hearts());
}

TEST(HandTest, RemoveSingleCard)
{
    Hand hand;
    Card card(Rank::NINE, Suit::HEARTS);
    hand.add(card);

    EXPECT_TRUE(hand.remove(card));
    EXPECT_TRUE(hand.is_empty());
    EXPECT_FALSE(hand.remove(card));  // Already removed
}

TEST(HandTest, RemoveMultipleCards)
{
    Hand hand;
    Card c1(Rank::KING, Suit::HEARTS);
    Card c2(Rank::KING, Suit::DIAMONDS);
    Card c3(Rank::KING, Suit::CLUBS);
    Card c4(Rank::KING, Suit::SPADES);

    hand.add(c1);
    hand.add(c2);
    hand.add(c3);
    hand.add(c4);

    std::vector<Card> to_remove = {c1, c2, c3, c4};
    EXPECT_TRUE(hand.remove(to_remove));
    EXPECT_TRUE(hand.is_empty());
}

// ============================================================================
// Stack Tests
// ============================================================================

TEST(StackTest, PlaceSingleCard)
{
    Stack stack;
    Card card(Rank::NINE, Suit::HEARTS);

    stack.place(card);
    EXPECT_EQ(stack.size(), 1);
    EXPECT_TRUE(stack.top_card() == card);
    EXPECT_EQ(stack.last_play_type(), PlayType::SINGLE);
}

TEST(StackTest, PlaceMultipleCards)
{
    Stack stack;
    Card c1(Rank::KING, Suit::HEARTS);
    Card c2(Rank::KING, Suit::DIAMONDS);
    Card c3(Rank::KING, Suit::CLUBS);
    Card c4(Rank::KING, Suit::SPADES);

    std::vector<Card> cards = {c1, c2, c3, c4};
    stack.place(cards);

    EXPECT_EQ(stack.size(), 4);
    EXPECT_EQ(stack.top_card(), c4);
    EXPECT_EQ(stack.last_play_type(), PlayType::FOUR_OF_A_KIND);
}

TEST(StackTest, ContainsNineOfHearts)
{
    Stack stack;
    EXPECT_FALSE(stack.contains_nine_of_hearts());

    stack.place(Card(Rank::NINE, Suit::HEARTS));
    EXPECT_TRUE(stack.contains_nine_of_hearts());
}

TEST(StackTest, IsOnlyNineOfHearts)
{
    Stack stack;
    stack.place(Card(Rank::NINE, Suit::HEARTS));

    // Check if stack contains only 9♥ by checking size and contents
    EXPECT_EQ(stack.size(), 1);
    EXPECT_TRUE(stack.contains_nine_of_hearts());

    stack.place(Card(Rank::TEN, Suit::SPADES));
    // Stack should no longer contain only 9♥
    EXPECT_EQ(stack.size(), 2);
}

TEST(StackTest, TakeThree)
{
    Stack stack;
    stack.place(Card(Rank::NINE, Suit::HEARTS));
    stack.place(Card(Rank::TEN, Suit::SPADES));
    stack.place(Card(Rank::JACK, Suit::DIAMONDS));
    stack.place(Card(Rank::QUEEN, Suit::CLUBS));

    auto taken = stack.take_three();
    EXPECT_EQ(taken.size(), 3);
    EXPECT_EQ(stack.size(), 1);  // Only QUEEN left
}

TEST(StackTest, TakeThreeWhenLessThanThreeCards)
{
    Stack stack;
    stack.place(Card(Rank::NINE, Suit::HEARTS));
    stack.place(Card(Rank::TEN, Suit::SPADES));

    auto taken = stack.take_three();
    EXPECT_EQ(taken.size(), 2);  // Takes whatever is available
    EXPECT_TRUE(stack.is_empty());
}

TEST(StackTest, TakeAll)
{
    Stack stack;
    stack.place(Card(Rank::NINE, Suit::HEARTS));
    stack.place(Card(Rank::TEN, Suit::SPADES));

    auto taken = stack.take_all();
    EXPECT_EQ(taken.size(), 2);
    EXPECT_TRUE(stack.is_empty());
}

// ============================================================================
// Rules Tests
// ============================================================================

TEST(RulesTest, IsSingleValidOnEmptyStack)
{
    Stack stack;
    Card card(Rank::NINE, Suit::HEARTS);

    EXPECT_TRUE(Rules::is_single_valid(stack, card));
}

TEST(RulesTest, IsSingleValidRankComparison)
{
    Stack stack;
    stack.place(Card(Rank::TEN, Suit::SPADES));

    EXPECT_TRUE(Rules::is_single_valid(stack, Card(Rank::TEN, Suit::HEARTS)));  // Equal
    EXPECT_TRUE(Rules::is_single_valid(stack, Card(Rank::JACK, Suit::HEARTS))); // Higher
    EXPECT_FALSE(Rules::is_single_valid(stack, Card(Rank::NINE, Suit::HEARTS))); // Lower
}

TEST(RulesTest, IsFourOfKindValidOnEmptyStack)
{
    Stack stack;
    EXPECT_TRUE(Rules::is_four_of_kind_valid(stack, Rank::KING));
}

TEST(RulesTest, IsFourOfKindRequiresHigherRank)
{
    Stack stack;
    stack.place(Card(Rank::TEN, Suit::SPADES));

    EXPECT_TRUE(Rules::is_four_of_kind_valid(stack, Rank::JACK));  // Higher
    EXPECT_FALSE(Rules::is_four_of_kind_valid(stack, Rank::TEN));  // Equal (not allowed)
    EXPECT_FALSE(Rules::is_four_of_kind_valid(stack, Rank::NINE));  // Lower
}

TEST(RulesTest, IsFourOfKindDetection)
{
    std::vector<Card> four_kings = {
        Card(Rank::KING, Suit::HEARTS),
        Card(Rank::KING, Suit::DIAMONDS),
        Card(Rank::KING, Suit::CLUBS),
        Card(Rank::KING, Suit::SPADES)
    };

    EXPECT_TRUE(Rules::is_four_of_kind(four_kings));

    std::vector<Card> not_four = {
        Card(Rank::KING, Suit::HEARTS),
        Card(Rank::KING, Suit::DIAMONDS),
        Card(Rank::KING, Suit::CLUBS),
        Card(Rank::QUEEN, Suit::SPADES)
    };

    EXPECT_FALSE(Rules::is_four_of_kind(not_four));
}

TEST(RulesTest, IsThreeNinesExcludingHearts)
{
    std::vector<Card> three_nines = {
        Card(Rank::NINE, Suit::DIAMONDS),
        Card(Rank::NINE, Suit::CLUBS),
        Card(Rank::NINE, Suit::SPADES)
    };

    EXPECT_TRUE(Rules::is_three_nines_excluding_hearts(three_nines));

    std::vector<Card> with_heart = {
        Card(Rank::NINE, Suit::HEARTS),
        Card(Rank::NINE, Suit::DIAMONDS),
        Card(Rank::NINE, Suit::CLUBS)
    };

    EXPECT_FALSE(Rules::is_three_nines_excluding_hearts(with_heart));
}

TEST(RulesTest, MustPlaceNineHeartsBack)
{
    std::vector<Card> with_nine_hearts = {
        Card(Rank::NINE, Suit::HEARTS),
        Card(Rank::TEN, Suit::SPADES),
        Card(Rank::JACK, Suit::DIAMONDS)
    };

    EXPECT_TRUE(Rules::must_place_nine_hearts_back(with_nine_hearts));

    std::vector<Card> without_nine_hearts = {
        Card(Rank::TEN, Suit::SPADES),
        Card(Rank::JACK, Suit::DIAMONDS)
    };

    EXPECT_FALSE(Rules::must_place_nine_hearts_back(without_nine_hearts));
}

// ============================================================================
// GameLibrary Tests
// ============================================================================

TEST(GameLibraryTest, Construction)
{
    GameConfig config {2};
    GameLibrary game(config);

    EXPECT_FALSE(game.is_game_over());
    EXPECT_FALSE(game.winner().has_value());
}

TEST(GameLibraryTest, StartGame)
{
    GameConfig config {2, 42};
    GameLibrary game(config);

    game.start();

    auto view = game.get_view();
    EXPECT_EQ(view.player_names.size(), 2);
    EXPECT_GT(view.hand_sizes[0], 0);
    EXPECT_GT(view.hand_sizes[1], 0);
}

TEST(GameLibraryTest, ThreePlayerGameDealsCorrectly)
{
    GameConfig config {3, 42};
    GameLibrary game(config);

    game.start();

    auto view = game.get_view();
    EXPECT_EQ(view.player_names.size(), 3);
    EXPECT_EQ(view.hand_sizes[0], 8);  // 24 / 3 = 8
    EXPECT_EQ(view.hand_sizes[1], 8);
    EXPECT_EQ(view.hand_sizes[2], 8);
}

TEST(GameLibraryTest, CannotPlayBeforeStart)
{
    GameConfig config {2};
    GameLibrary game(config);

    // Cannot play before game starts
    EXPECT_FALSE(game.play_card(0, {0}));
}

TEST(GameLibraryTest, OnlyCurrentPlayerCanPlay)
{
    GameConfig config {2, 42};
    GameLibrary game(config);
    game.start();

    auto current = game.get_view().current_player_index;
    auto other = (current + 1) % 2;

    // Non-current player cannot play
    EXPECT_FALSE(game.play_card(other, {0}));

    // Current player can play
    EXPECT_TRUE(game.play_card(current, {0}));
}

TEST(GameLibraryTest, InvalidPlayReturnsFalse)
{
    GameConfig config {2, 42};
    GameLibrary game(config);
    game.start();

    // Try to play with invalid index
    EXPECT_FALSE(game.play_card(0, {999}));
}

TEST(GameLibraryTest, PassTakeThree)
{
    GameConfig config {2, 42};
    GameLibrary game(config);
    game.start();

    auto view_before = game.get_view();
    auto stack_size_before = view_before.stack_size;

    // Current player passes and takes 3
    EXPECT_TRUE(game.pass_take_three(view_before.current_player_index));

    auto view_after = game.get_view();
    // Current player should have changed
    EXPECT_NE(view_after.current_player_index, view_before.current_player_index);
}

TEST(GameLibraryTest, GetValidPlays)
{
    GameConfig config {2, 42};
    GameLibrary game(config);
    game.start();

    auto plays = game.get_valid_plays(0);
    // Should have some valid plays (at minimum, single card plays)
    EXPECT_GT(plays.size(), 0);
}

// ============================================================================
// Main
// ============================================================================

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
