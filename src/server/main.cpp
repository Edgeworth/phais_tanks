#include <csignal>
#include "common.hpp"
#include "add_client.hpp"
#include "game.hpp"

int main() {
	signal(SIGPIPE, SIG_IGN);
	sf::Thread add(addClients), r(run);
	add.Launch();
	r.Launch();

	while (1) {
		char inp[1000];
		scanf("%s", inp);
	}
	running = false;
	add.Wait();
	r.Wait();
}
