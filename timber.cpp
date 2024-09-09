#include <SFML/Graphics.hpp>

int main(int argc, char const *argv[])
{
    sf::VideoMode vm(1920, 1080);   // Video mode object
    sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);   // Render game window

    sf::Texture textureBackground;  // Texture to hold graphic on the GPU
    textureBackground.loadFromFile("graphics/background.png");  // Load a graphics into the texture

    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);  // Attaching the texture into a sprite

    spriteBackground.setPosition(0,0);

    while (window.isOpen())
    {
        /* Handle players input */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        
        /* Update the scene */
        
        /* Draw the scene */

        window.clear(); // Clear everything from the last frame

        window.draw(spriteBackground);// Draw game scene here

        window.display();   // Display scene
    }
    

    return 0;
}

