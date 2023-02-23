#include "snake.h"

int main() {
    int width = sf::VideoMode::getDesktopMode().width;
    int height = sf::VideoMode::getDesktopMode().height;

    

    sf::RenderWindow window;
    window.create(sf::VideoMode(width, height), "Hello", sf::Style::Fullscreen);

    Game game;

    window.clear();
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {

                case sf::Keyboard::Escape: {
                    window.close();
                    break;
                }

                case sf::Keyboard::Up: {
                    game.setMoveDirection(MoveDirection::UP);
                    break;
                }

                case sf::Keyboard::Left: {
                    game.setMoveDirection(MoveDirection::LEFT);
                    break;
                }

                case sf::Keyboard::Down: {
                    game.setMoveDirection(MoveDirection::DOWN);
                    break;
                }

                case sf::Keyboard::Right: {
                    game.setMoveDirection(MoveDirection::RIGHT);
                    break;
                }

                }
            }
        }


        game.update();
        game.render(window);

        window.display();
        sf::sleep(sf::milliseconds(33));
    }

    return 0;
}