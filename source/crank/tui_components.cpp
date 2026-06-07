// Copyright (c) 2026 feetuh. All Rights Reserved.

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "crank/tui_components.hpp"

#include "crank/card.hpp"

namespace crank
{

void TUIComponents::run_game_loop(TUIGameWrapper& game)
{
  auto screen = ftxui::ScreenInteractive::Fullscreen();

  // State for the game loop
  std::atomic<bool> running = true;

  // Start the game
  game.start();

  // Event handler function
  auto on_event = [&](ftxui::Event event)
  {
    if (event == ftxui::Event::Character('q')) {
      running = false;
      return true;
    }
    return false;
  };

  // Create the main renderer
  auto renderer = ftxui::Renderer([&] { return render_game_screen(game); });

  // Run the event loop
  screen.Loop(renderer);

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
  std::vector<size_t> selected_indices;

  auto main_content = ftxui::vbox({
                          ftxui::text("Card Crank - Terminal Game")
                              | ftxui::bold | ftxui::center,
                          ftxui::separator(),
                          render_player_info(view),
                          ftxui::separator(),
                          render_stack_display(stack),
                          ftxui::separator(),
                          render_hand_display(hand, selected_indices),
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

  // Add the base card (always 9♥)
  card_elements.push_back(render_card(Card {Rank::NINE, Suit::HEARTS}, false));

  // Add other cards if any
  for (const auto& card : stack) {
    if (!(card.rank() == Rank('9') && card.suit() == Suit('h'))) {
      card_elements.push_back(render_card(card, false));
    }
  }

  return ftxui::hbox(card_elements);
}

auto TUIComponents::render_hand_display(const std::vector<Card>& hand,
                                        const std::vector<size_t>& selected)
    -> ftxui::Element
{
  if (hand.empty()) {
    return ftxui::text("Hand: empty");
  }

  std::vector<ftxui::Element> card_elements;

  for (size_t i = 0; i < hand.size(); ++i) {
    bool isSelected =
        std::find(selected.begin(), selected.end(), i) != selected.end();
    card_elements.push_back(render_card(hand[i], isSelected));

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

  std::string valid_text = "Valid moves: ";

  for (size_t i = 0; i < valid_plays.size(); ++i) {
    if (i > 0) {
      valid_text += ", ";
    }

    // For single cards, show rank
    if (valid_plays[i].cards.size() == 1) {
      valid_text +=
          std::to_string(static_cast<int>(valid_plays[i].cards.at(0).rank()));
    } else {
      // For multiple cards, show count
      valid_text += std::to_string(valid_plays[i].cards.size()) + " cards";
    }
  }

  return ftxui::text(valid_text);
}

auto TUIComponents::render_controls() -> ftxui::Element
{
  return ftxui::text(
      "Controls: Space=Select, Enter=Play, P=Pass3, O=PassAll, Q=Quit");
}

auto TUIComponents::render_card(const Card& card, bool selected)
    -> ftxui::Element
{
  std::string card_text = "[" + std::to_string(static_cast<int>(card.rank()))
      + TUIComponents::get_card_symbol(card.suit()) + "]";

  if (selected) {
    return ftxui::text(card_text) | ftxui::bgcolor(ftxui::Color::GrayDark);
  }

  return ftxui::text(card_text);
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

void TUIComponents::handle_events(ftxui::ScreenInteractive& screen,
                                  TUIGameWrapper& game,
                                  std::atomic<bool>& running,
                                  std::vector<size_t>& selected_indices)
{
  // This is a placeholder for event handling
  // In a full implementation, this would handle keyboard input
}

}  // namespace crank
