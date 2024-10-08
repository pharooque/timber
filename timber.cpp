#include <sstream>
#include <SFML/Graphics.hpp>
#include <random>

void updateBranches (int seed);


const int NUM_BRANCHES { 6 };
sf::Sprite branches [NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions [ NUM_BRANCHES ];

int main(int argc, char const *argv[])
{
    sf::VideoMode vm(1920, 1080);                                   // Video mode object
    sf::RenderWindow window(vm, "Timber", sf::Style::Fullscreen);   // Render game window

    sf::Texture textureBackground;                                  // Texture to hold graphic on the GPU
    textureBackground.loadFromFile("graphics/background.png");      // Load a graphics into the texture

    sf::Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground); // Attaching the texture into a sprite
    spriteBackground.setPosition(0, 0);

    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    sf::Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);              // Added a tree sprite

    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    sf::Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    bool beeActive {false};             
    float beeSpeed {0.0f};                      // The Bee's speed

    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    // Loading the cloud texture into 3 different sprites
    sf::Sprite spriteCloud[3]{sf::Sprite(textureCloud), sf::Sprite(textureCloud), sf::Sprite(textureCloud)};

    // Positioning the clouds from the left at three different height levels
    spriteCloud[0].setPosition(0, 0);
    spriteCloud[1].setPosition(0, 250);
    spriteCloud[2].setPosition(0, 500);

    bool cloudActive[3] { false, false, false };
    float cloudSpeed[3] { 0.0f, 0.0f, 0.0f };

    sf::Clock clock;                           // Time control variable

    sf::RectangleShape timeBar{};              //Adding the Time bar
    float timeBarStartwidth{ 400 };
    float timeBarHeight{ 80 };

    timeBar.setSize( sf::Vector2f(timeBarStartwidth, timeBarHeight) );
    timeBar.setFillColor( sf::Color::Red );
    timeBar.setPosition( (1920 / 2) - timeBarStartwidth / 2, 980 );

    sf::Time gameTimeTotal{};
    float timeRemaining{ 6.0f };
    float timeBarWidthPerSecond{ timeBarStartwidth / timeRemaining };

    bool paused {true};                        // Game pause variable.

    int score{0};                              // Drawing text to display.
    sf::Text messageText;
    sf::Text scoreText;

    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");    // Choosing the font.

    messageText.setFont(font);
    scoreText.setFont(font);

    messageText.setString("Press Enter to Start!"); // Assigning the message to display.
    scoreText.setString("Score = 0");

    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    sf::FloatRect textRect{ messageText.getLocalBounds()};
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    messageText.setPosition( 1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition( 20, 20);

    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);

        branches[i].setOrigin(220, 20);
    }
    
    sf::Texture texturePlayer;      // Preparing the player
    texturePlayer.loadFromFile("graphics/player.png");
    sf::Sprite spritePlayer{};
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    side playerSide {side::LEFT};       // Player starts on the left side

    sf::Texture textureRIP{};           // Loading the gravestone asset
    textureRIP.loadFromFile("graphics/rip.png");
    sf::Sprite spriteRIP{};
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    sf::Texture textureAxe{};           // Axe sprite
    textureAxe.loadFromFile("graphics/axe.png");
    sf::Sprite spriteAxe{};
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    const float AXE_POSITION_LEFT{700};  // Aligning the axe with the tree.
    const float AXE_POSITION_RIGHT{1075};

    sf::Texture textureLog{};           // Falling log texture
    textureLog.loadFromFile("graphics/log.png");
    sf::Sprite spriteLog{};
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    bool logActive{ false };
    float logspeedX{ 1000 };
    float logspeedY{ -1500 };

    bool acceptInput{ false };          // Control player input

    while (window.isOpen())
    {
        /* Handle players input */

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyReleased && !paused)
            {
                acceptInput = true;

                spriteAxe.setPosition(2000, spriteAxe.getPosition().y); // Hide the axe
            }
            
        }
        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            paused = false;

            score = 0;
            timeRemaining = 6;                      // Reset time and score

            for (auto i = 1; i < NUM_BRANCHES; i++) // Starting in the second positions, make all the branches disappear
            {
                branchPositions[i] = side::NONE;
            }

            spriteRIP.setPosition(675, 2000);       // Making sure gravestone is hidden

            spritePlayer.setPosition(580, 720);     // Move player into position
            
        }

        if (acceptInput)        // Enable player input
        {
            /* code */

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                playerSide = side::RIGHT;           // Making sure the player is on the right side
                score ++;
                timeRemaining += (2 / score) + .15; // Increase time remaining

                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);

                updateBranches(score);              //update the branches

                spriteLog.setPosition(810, 720);    // Set falling log to the left
                logspeedX = -5000;
                logActive = true;

                acceptInput = false;
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                playerSide = side::LEFT;           // Keep the player to the left
                score ++;
                timeRemaining += (2 / score) + .15; // then increase time remaining

                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);

                updateBranches(score);              //update the branches

                spriteLog.setPosition(810, 720);    // yeeeeet the falling log
                logspeedX = 5000;
                logActive = true;

                acceptInput = false;
            }

        }
        
        
        /* Update the scene */

        if (!paused)
        {
        
            sf::Time deltaClock { clock.restart() };// Measure time

            timeRemaining -= deltaClock.asSeconds();//Subtract from amount of time remaining

            timeBar.setSize( sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight) );  // Resize the timebar

            if (timeRemaining <= 0.0f)              // if timeout
            {
                paused = true;                      // Pause the game

                messageText.setString("Out of time!!!");// Change message shown

                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin( textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f );

                messageText.setPosition( 1920 / 2.0f, 1080 / 2.0f );    //Reposition the text

            }
            
            
            if (!beeActive)                         // If bee is moving
            {
                srand( (int) time(0) );
                beeSpeed = (rand() % 200) + 200;    // How fast is the bee?

                srand( (int) time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);// And how high is the bee?

                beeActive = true;
            }
            else
            {
                spriteBee.setPosition
                (
                    spriteBee.getPosition().x - (beeSpeed * deltaClock.asSeconds()), spriteBee.getPosition().y
                );  // move the bee

                if (spriteBee.getPosition().x < -100) // if bee has reached the left screen edge
                {
                    beeActive = false;                // Animate new bee next frame
                } 
            }
            
            for (auto i = 0; i < 3; ++i)
            {
                if (!cloudActive[i])                     // For Cloud 1
                {
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(0, 200);
                    cloudSpeed[i] = dist(gen);
                    cloudActive[i] = true;
                    spriteCloud[i].setPosition(-200, i * 250);
                }
                else
                {
                    // move the cloud
                    spriteCloud[i].move((cloudSpeed[i] * deltaClock.asSeconds()), 0);

                    if (spriteCloud[i].getPosition().x > 1920) // if the cloud has reached the right screen edge
                    {
                        cloudActive[i] = false;                // Animate it as a new cloud next frame
                    } 
                }
            }

            std::stringstream ss;                       // Updating the score text
            ss << "Score = " << score;
            scoreText.setString( ss.str() );

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if ( branchPositions[i] == side::LEFT )
                {
                    branches[i].setPosition(610, height);
                    branches[i].setRotation(180);
                }

                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition( 1330, height );
                    branches[i].setRotation(0);
                }

                else
                {
                    branches[i].setPosition(3000, height );
                }
                
            }

            if (logActive)          // Handle the falling log
            {
                spriteLog.setPosition
                (
                    spriteLog.getPosition().x + (logspeedX * deltaClock.asSeconds()), spriteLog.getPosition().y + (logspeedY * deltaClock.asSeconds())
                );

                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) // Has the log reached the right hand edge
                {
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
                
            }

            if (branchPositions[5] == playerSide)   // if branch squishees player
            {
                paused = true;
                acceptInput = false;

                spriteRIP.setPosition(525, 760);    // Draw gravestone and yeet player
                spritePlayer.setPosition(2000, 660);

                messageText.setString("SQUISHED!!");
                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

        } // End if paused

        /* Draw the scene */

        window.clear();                         // Clear everything from the last frame

        window.draw(spriteBackground);          // Draw game scene here

        for(auto& cloud : spriteCloud) window.draw(cloud);
        for (auto i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(spriteTree);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);
        window.draw(spriteBee);

        window.draw(scoreText);
        window.draw(timeBar);

        if (paused)
        {
            window.draw(messageText);           // Draw the message
        }
        

        window.display();                       // Display scene
    }
    

    return 0;
}

void updateBranches(int seed)
{
    for (auto j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j-1];
    }
    
    srand((int) time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    
    case 1:
        branchPositions[1] = side::RIGHT;
        break;
    
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}
