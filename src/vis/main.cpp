#include "common.hpp"
#include "draw.hpp"
#include "net.hpp"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("IP address required.\n");
		return 1;
	}

	sf::Thread net(runNet, argv[1]);
	net.Launch();

	window.Create(sf::VideoMode(800, 800), "Tanks");
	window.UseVerticalSync(true);
	sf::View view(sf::FloatRect(-1100, -1100, 1100, 1100));
	window.SetView(view);

	sf::Clock clock;
	while (window.IsOpened() && running) {
		sf::Event event;
		while (window.GetEvent(event))
			if (event.Type == sf::Event::Closed) {
				window.Close();
				running = false;
			}

		printf("Tanks: %d Shells: %d\n", copyTanks.size(), copyShells.size());
		draw(window, clock);
		window.Display();
	}
}
