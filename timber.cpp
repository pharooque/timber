#include <SFML/Graphics.hpp>

int main(int argc, char const *argv[])
{
    sf::VideoMode vm(1920, 1080); // Video mode object

    // Render game window
    sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);

    while (window.isOpen())
    {
        /* Handle players input */
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        
        /* Update the scene */
        
        /* Draw the scene */

        // Clear everything from the last frame
        window.clear();

        // Draw game scene here

        // Display scene
        window.display();
    }
    

    return 0;
}

