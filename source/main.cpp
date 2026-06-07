#include <iostream>

#include "crank/lib.hpp"

auto main() -> int
{
  // Create a 2-player game
  crank::GameConfig config {
      2, std::uint32_t {42}};  // 2 players, seeded for reproducibility
  crank::GameLibrary game {config};

  // Start the game
  game.start();

  std::cout << "Card Crank game started with " << config.player_count
            << " players!\n";

  // Get initial game view
  auto view = game.get_view();
  std::cout << "Current player: "
            << view.player_names[view.current_player_index] << "\n";
  std::cout << "Stack size: " << view.stack_size << "\n";

  return 0;
}
