#include <header.hpp>

int main(int argc, char *argv[]) {
  signal(SIGINT, Hasher::SignalCatch);
  Hasher hasher(argc, argv);
  hasher.Start(true);
}