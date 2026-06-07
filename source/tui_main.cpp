// Copyright (c) 2026 feetuh. All Rights Reserved.

#include <iostream>

#include "crank/tui_components.hpp"
#include "crank/tui_wrapper.hpp"

auto main() -> int
{
  constexpr auto s_player_count = 2;
  constexpr auto s_seed = 42;
  // Create game configuration
  crank::GameConfig config {
      .player_count = s_player_count,  // 2 players
      .seed = s_seed  // seeded for reproducibility
  };

  // Create TUI wrapper
  crank::TUIGameWrapper game(config);

  // Run TUI components
  try {
    crank::TUIComponents::run_game_loop(game);
  } catch (const std::exception& e) {
    std::cerr << "Error running TUI: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Unknown error running TUI" << std::endl;
    return 1;
  }

  return 0;
}
