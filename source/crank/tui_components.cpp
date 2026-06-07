// Copyright (c) 2026 feetuh. All Rights Reserved.

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "crank/tui_components.hpp"

#include "crank/card.hpp"

namespace crank
{

void TUIComponents::run_game_loop(TUIGameWrapper& game)
{
  auto screen = ftxui::ScreenInteractive::Fullscreen();

  // Start the game
  game.start();

  // Create the main renderer with event handling
  auto renderer = ftxui::Renderer([&] { return render_game_screen(game); });

  // Add event handling
  auto component = ftxui::CatchEvent(renderer, [&](ftxui::Event event) {
    // Handle quit
    if (event == ftxui::Event::Character('q') || event == ftxui::Event::Escape) {
      screen.ExitLoopClosure()();
      return true;
    }

    // Check if game is over
    if (game.is_game_over()) {
      return false;  // Only allow quit when game is over
    }

    const auto current_hand = game.get_current_hand();

    // Handle arrow keys for navigation
    if (event == ftxui::Event::ArrowRight) {
      game.move_highlight(1);
      return true;
    }

    if (event == ftxui::Event::ArrowLeft) {
      game.move_highlight(-1);
      return true;
    }

    // Handle space bar to toggle selection of highlighted card
    if (event == ftxui::Event::Character(' ')) {
      const auto highlighted = game.get_highlighted_index();
      if (highlighted < current_hand.size()) {
        game.toggle_card_selection(highlighted);
        return true;
      }
    }

    // Handle Enter to play selected cards
    if (event == ftxui::Event::Return) {
      game.play_selected_cards();
      return true;
    }

    // Handle 'p' for pass take three
    if (event == ftxui::Event::Character('p') || event == ftxui::Event::Character('P')) {
      game.pass_take_three();
      return true;
    }

    // Handle 'o' for pass take all
    if (event == ftxui::Event::Character('o') || event == ftxui::Event::Character('O')) {
      game.pass_take_all();
      return true;
    }

    // Handle 'c' to clear selection
    if (event == ftxui::Event::Character('c') || event == ftxui::Event::Character('C')) {
      game.clear_selection();
      return true;
    }

    // Handle 'a' to select all cards
    if (event == ftxui::Event::Character('a') || event == ftxui::Event::Character('A')) {
      // Clear current selection first, then select all
      game.clear_selection();
      for (size_t i = 0; i < current_hand.size(); ++i) {
        game.toggle_card_selection(i);
      }
      return true;
    }

    return false;  // Event not handled
  });

  // Run the event loop
  screen.Loop(component);

  // Clean up
  screen.Exit();
}

auto TUIComponents::render_game_screen(const TUIGameWrapper& game)
    -> ftxui::Element
{
  auto view = game.get_view();
  auto hand = game.get_current_hand();
  auto stack = game.get_stack();
  auto valid_plays = game.get_valid_plays();
  auto selected_indices = game.get_selected_indices();

  // Check if game is over
  if (game.is_game_over()) {
    auto winner = game.get_winner();
    std::string winner_text = winner.has_value()
        ? "Game Over! Winner: " + winner.value()
        : "Game Over! No winner (draw)";

    auto game_over_content = ftxui::vbox({
        ftxui::text("Card Crank - Terminal Game")
            | ftxui::bold | ftxui::center,
        ftxui::separator(),
        ftxui::text(winner_text)
            | ftxui::bold | ftxui::center | ftxui::color(ftxui::Color::Green),
        ftxui::separator(),
        render_player_info(view),
        ftxui::separator(),
        render_stack_display(stack),
        ftxui::separator(),
        ftxui::text("Press Q or ESC to quit") | ftxui::center,
    });

    return game_over_content | ftxui::border;
  }

  auto main_content = ftxui::vbox({
                          ftxui::text("Card Crank - Terminal Game")
                              | ftxui::bold | ftxui::center,
                          ftxui::separator(),
                          render_player_info(view),
                          ftxui::separator(),
                          render_stack_display(stack),
                          ftxui::separator(),
                          render_hand_display(hand, selected_indices, game.get_highlighted_index(), valid_plays),
                          ftxui::separator(),
                          render_valid_moves(valid_plays),
                          ftxui::separator(),
                          render_controls(),
                      })
      | ftxui::border;

  return main_content;
}

auto TUIComponents::render_player_info(const GameView& view) -> ftxui::Element
{
  std::vector<ftxui::Element> player_elements;

  for (size_t i = 0; i < view.player_names.size(); ++i) {
    std::string player_info = view.player_names[i] + " ("
        + std::to_string(view.hand_sizes[i]) + " cards)";

    if (i == view.current_player_index) {
      player_info = "● " + player_info;  // Current player indicator
      player_elements.push_back(ftxui::text(player_info) | ftxui::bold
                                | ftxui::color(ftxui::Color::Yellow));
    } else {
      player_elements.push_back(ftxui::text("  " + player_info));
    }
  }

  return ftxui::vbox(player_elements);
}

auto TUIComponents::render_stack_display(const std::vector<Card>& stack)
    -> ftxui::Element
{
  if (stack.empty()) {
    return ftxui::text("Stack: empty");
  }

  std::vector<ftxui::Element> card_elements;

  // Render all cards in the stack (including the base 9♥)
  for (const auto& card : stack) {
    // Stack cards are always "valid" in context - they're already played
    card_elements.push_back(render_card(card, false, true));
  }

  return ftxui::hbox(card_elements);
}

auto TUIComponents::render_hand_display(const std::vector<Card>& hand,
                                        const std::vector<size_t>& selected,
                                        size_t highlighted_index,
                                        const std::vector<Play>& valid_plays)
    -> ftxui::Element
{
  if (hand.empty()) {
    return ftxui::text("Hand: empty");
  }

  // Custom comparator that compares both rank and suit
  auto card_comparator = [](const Card& a, const Card& b) {
    if (a.rank() != b.rank()) {
      return a.rank() < b.rank();
    }
    return a.suit() < b.suit();
  };

  // Build a set of cards that are valid moves (single card plays only for hand display)
  std::set<Card, decltype(card_comparator)> valid_single_cards(card_comparator);
  for (const auto& play : valid_plays) {
    if (play.cards.size() == 1) {
      valid_single_cards.insert(play.cards[0]);
    }
  }

  std::vector<ftxui::Element> card_elements;

  for (size_t i = 0; i < hand.size(); ++i) {
    bool isSelected =
        std::find(selected.begin(), selected.end(), i) != selected.end();
    bool isHighlighted = (i == highlighted_index);
    bool is_valid_move = valid_single_cards.contains(hand[i]);
    auto card_elem = render_card(hand[i], isSelected, is_valid_move);

    // Add visual indicator for highlighted card
    if (isHighlighted) {
      card_elem = card_elem | ftxui::underlined | ftxui::bold;
    }

    // Add index number above card (show highlighted card with >)
    std::string index_str = std::to_string(i + 1);
    if (isHighlighted) {
      index_str = ">" + index_str + "<";
    }
    auto indexed_card = ftxui::vbox({
        ftxui::text(index_str) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 4) | ftxui::center,
        card_elem
    });

    card_elements.push_back(indexed_card);

    // Add spacing between cards
    if (i < hand.size() - 1) {
      card_elements.push_back(ftxui::text(" "));
    }
  }

  return ftxui::hbox(card_elements);
}

auto TUIComponents::render_valid_moves(const std::vector<Play>& valid_plays)
    -> ftxui::Element
{
  if (valid_plays.empty()) {
    return ftxui::text("Valid moves: None");
  }

  // Group plays by rank
  struct RankInfo
  {
    std::vector<Suit> single_suits;  // Suits valid as single plays
    bool has_four_of_a_kind = false;
  };
  std::map<Rank, RankInfo> rank_map;

  for (const auto& play : valid_plays) {
    if (play.type == PlayType::SINGLE && play.cards.size() == 1) {
      rank_map[play.cards[0].rank()].single_suits.push_back(play.cards[0].suit());
    } else if (play.type == PlayType::FOUR_OF_A_KIND) {
      rank_map[play.cards[0].rank()].has_four_of_a_kind = true;
    }
  }

  std::vector<ftxui::Element> move_elements;
  move_elements.push_back(ftxui::text("Valid moves: "));

  bool first = true;
  for (const auto& [rank, info] : rank_map) {
    if (!first) {
      move_elements.push_back(ftxui::text(", "));
    }
    first = false;

    std::string rank_str = get_rank_display(rank);

    if (info.has_four_of_a_kind) {
      // Four of a kind: show as "rank[4]"
      move_elements.push_back(ftxui::text(rank_str + "[4]"));
    } else if (info.single_suits.size() > 1) {
      // Multiple suits: show as "rank[*]"
      move_elements.push_back(ftxui::text(rank_str + "[*]"));
    } else if (info.single_suits.size() == 1) {
      // Single suit: show as "rank[suit]"
      move_elements.push_back(ftxui::text(rank_str + "["
                                               + get_card_symbol(info.single_suits[0])
                                               + "]"));
    }
  }

  return ftxui::hbox(move_elements);
}

auto TUIComponents::render_controls() -> ftxui::Element
{
  return ftxui::vbox({
      ftxui::text("←/→=Move Highlight"),
      ftxui::text("Space=Toggle Selection"),
      ftxui::text("Enter=Play Selected"),
      ftxui::text("P=Pass Take Three"),
      ftxui::text("O=Pass Take All"),
      ftxui::text("A=Select All"),
      ftxui::text("C=Clear Selection"),
      ftxui::text("Q/ESC=Quit"),
  });
}

auto TUIComponents::render_card(const Card& card, bool selected, bool is_valid_move)
    -> ftxui::Element
{
  std::string card_text = "[" + get_rank_display(card.rank())
      + TUIComponents::get_card_symbol(card.suit()) + "]";

  auto element = ftxui::text(card_text);

  if (selected) {
    element = element | ftxui::bgcolor(ftxui::Color::GrayDark);
  }

  if (!is_valid_move) {
    element = element | ftxui::color(ftxui::Color::Red);
  }

  return element;
}

auto TUIComponents::get_card_symbol(Suit suit) -> std::string
{
  switch (suit) {
    case Suit::HEARTS:
      return "♥";
    case Suit::DIAMONDS:
      return "♦";
    case Suit::CLUBS:
      return "♣";
    case Suit::SPADES:
      return "♠";
    default:
      return "?";
  }
}

auto TUIComponents::get_rank_display(Rank rank) -> std::string
{
  switch (rank) {
    case Rank::NINE:
      return "9";
    case Rank::TEN:
      return "T";
    case Rank::JACK:
      return "J";
    case Rank::QUEEN:
      return "Q";
    case Rank::KING:
      return "K";
    case Rank::ACE:
      return "A";
    default:
      return "?";
  }
}

}  // namespace crank
