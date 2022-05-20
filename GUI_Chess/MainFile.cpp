#include "Interfaces.h"
int main() {
	int GRIDSIZEX = 8;
	int GRIDSIZEY = 8;
	Grid*MainGrid =new Grid(GRIDSIZEY, GRIDSIZEX);
	DisplayManager mm(MainGrid);
	InputManager inp;
 
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
				manager._input.UpdateInut(event.mouseMove.y, event.mouseMove.x, MainGrid);
				 
			 
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