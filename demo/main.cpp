#include <header.hpp>
//#include <signal.h>

int main(int argc, char *argv[]) {
  signal(SIGTERM, Hasher::SignalCatch);


  Hasher hasher(argc, argv);
  hasher.Start(true);
}