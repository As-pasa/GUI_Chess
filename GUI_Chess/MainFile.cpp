#include "Interfaces.h"
int main() {
	
 
	sf::RenderWindow window(sf::VideoMode(400, 400), "SFMLworks!");
	LogicManager manager;
	manager.Init();
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonPressed:
				manager._input.SetIsPressed(true);
				break;

			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
					manager._input.SetIsPressed(false);
				 
				break;
			case sf::Event::MouseMoved:
				manager._input.UpdateInut(event.mouseMove.y, event.mouseMove.x, manager.GetGrid());
				 
			 
				break;
			default:
				break;
			}
		}
		window.clear();
		manager.Test(window);
		window.display();
	}
}