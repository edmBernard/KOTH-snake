#include <game/player.hpp>

#include <chrono>
#include <iomanip>

DIRECTION bot(int x, int y, int dim) {
  return DIRECTION::DOWN;
}

int main(int argc, const char **argv) {

  int dim = 100;
  int n_players = 100;
  int n_iter = 1'000'000;

  std::vector<int> playerIndexes;
  playerIndexes.reserve(n_players);

  States state(dim, dim);

  for (int i = 0; i < n_players; ++i) {
    playerIndexes[i] = state.addPlayer(i / dim, i % dim, i + 1);
  }

  auto start1 = std::chrono::high_resolution_clock::now();

  for (int k = 0; k < n_iter; ++k) {
    for (int i = 0; i < n_players; ++i) {
      auto [x, y] = state.getPosition(playerIndexes[i]);
      state.directions[i] = bot(x, y, dim);
    }

    state.update();
  }

  std::chrono::duration<double, std::micro> elapsed1 = std::chrono::high_resolution_clock::now() - start1;
  std::cout << "Time : " << n_iter << " iterations " << std::setw(20) << std::right << elapsed1.count()/n_iter << " us/iter\n";
}
