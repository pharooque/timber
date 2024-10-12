#include <sstream>
#include <SFML/Graphics.hpp>
#include <random>
#include <array>

const int NUM_BRANCHES = 6;
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

enum class side {LEFT, RIGHT, NONE};

// Game state variables
struct Timber
{
    std::array<sf::Sprite, NUM_BRANCHES> branches;
    std::array<side, NUM_BRANCHES> branchPositions;
    sf::Sprite player, rip, axe, log;
    side playerSide = side::LEFT;  // Player starts on the left
    bool logActive = false;
    float logspeedX = 1000;
    float logspeedY = -1500;
    bool paused = true;  // Game pause variable.
    int score = 0;
    float timeRemaining = 6.0f;
    bool acceptInput = false;
};

void updateBranches(Timber& game, std::mt19937& gen);
void setupSprites(Timber& game, const sf::Texture& textureBranch, const sf::Texture& texturePlayer,
                  const sf::Texture& textureRip, const sf::Texture& textureAxe, const sf::Texture& textureLog);

int main()
{
    // Render game window
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Timber", sf::Style::Fullscreen);

    // Load textures once at the start
    std::array<sf::Texture, 9> textures;
    const char* textureFiles[]{"graphics/background.png", "graphics/tree.png", "graphics/bee.png",
                               "graphics/cloud.png", "graphics/branch.png", "graphics/player.png",
                               "graphics/rip.png", "graphics/axe.png", "graphics/log.png"};

    for (int i = 0; i < textures.size(); ++i)
    {
        if (!textures[i].loadFromFile(textureFiles[i]))
        {
            // Handle error
            return 1;
        }
    }
    
    // Set up sprites
    sf::Sprite background(textures[0]), tree(textures[1]), bee(textures[2]);
    std::array<sf::Sprite, 3> clouds;
    for (auto& cloud : clouds) cloud.setTexture(textures[3]);

    tree.setPosition(810, 0);
    bee.setPosition(0, 800);

    // Positioning the clouds from the left at three different height levels
    clouds[0].setPosition(0, 0);
    clouds[1].setPosition(0, 250);
    clouds[2].setPosition(0, 500);

    // Setup game state
    Timber game;
    setupSprites(game, textures[4], textures[5], textures[6], textures[7], textures[8]);

    // Time bar setup
    const float timeBarStartWidth = 400;
    const float timeBarHeight = 80;
    sf::RectangleShape timeBar({timeBarStartWidth, timeBarHeight});
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition((SCREEN_WIDTH / 2) - timeBarStartWidth / 2, 980);

    // Setup text
    sf::Font font;
    if (!font.loadFromFile("fonts/KOMIKAP_.ttf"))
    {
        // Handle error
        return 1;
    }

    // Drawing text
    sf::Text messageText, scoreText;
    messageText.setFont(font);
    scoreText.setFont(font);
    messageText.setString("Press Enter to Start!");  // Assigning the message to display
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
    scoreText.setPosition( 20, 20);

    // setup random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> beeHeightDist(500, 1000);
    std::uniform_real_distribution<> cloudHeightDist(-150, 300);
    std::uniform_real_distribution<> beeSpeedDist(200, 400);
    std::uniform_real_distribution<> cloudSpeedDist(0, 200);
    
    // Initialize sprite speed
    float beeSpeed = beeSpeedDist(gen);
    std::array<float, 3> cloudSpeed;
    for (auto& speed : cloudSpeed) speed = cloudSpeedDist(gen);

    sf::Clock clock;

    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }

            if (event.type == sf::Event::KeyReleased && !game.paused)
            {
                game.acceptInput = true;
                game.axe.setPosition(2000, game.axe.getPosition().y);
            }
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            game.paused = false;
            game.score = 0;
            game.timeRemaining = 6;
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                game.branchPositions[i] = side::NONE;
            }    

            // Move player into position and hide gravestone
            game.rip.setPosition(675, 2000);
            game.player.setPosition(580, 720);
        }

        // Handle player input
        if (game.acceptInput)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                game.playerSide = side::RIGHT;
                game.score++;
                game.timeRemaining += (2.0f / game.score) + 0.15f;  // Increase time based on score
                game.axe.setPosition(1075, game.axe.getPosition().y);
                game.player.setPosition(1200, 720);
                updateBranches(game, gen);  // Update the branches
                game.log.setPosition(810, 720);
                game.logspeedX = -5000;
                game.logActive = true;
                game.acceptInput = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                game.playerSide = side::LEFT;
                game.score++;
                game.timeRemaining += (2.0f / game.score) + 0.15f;
                game.axe.setPosition(700, game.axe.getPosition().y);
                game.player.setPosition(580, 720);
                updateBranches(game, gen);

                game.log.setPosition(810, 720);
                game.logspeedX = 5000;
                game.logActive = true;
                game.acceptInput = false;
            }
        }
        
        if (!game.paused)
        {
            // Update timer
            sf::Time deltaClock = clock.restart();
            game.timeRemaining -= deltaClock.asSeconds();
            
            float timeBarWidth = (game.timeRemaining / 6.0f) * timeBarStartWidth;
            timeBar.setSize({timeBarWidth, timeBarHeight});

            // Update timeout
            if (game.timeRemaining <= 0.0f)
            {
                game.paused = true;
                messageText.setString("Out of time!!!");  // Display timeout message
                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f );
                messageText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_WIDTH / 2.0f);
            }
            
            // Update bee
            if (bee.getPosition().x < -100)
            {
                beeSpeed = beeSpeedDist(gen);
                bee.setPosition(2000, beeHeightDist(gen));
            }
            else
            {
                bee.move(-beeSpeed * deltaClock.asSeconds(), 0);
            }
            
            // Update clouds
            for (int i = 0; i < 3; ++i)
            {
                if (clouds[i].getPosition().x > SCREEN_HEIGHT)
                {
                    cloudSpeed[i] = cloudSpeedDist(gen);
                    clouds[i].setPosition(-200, cloudHeightDist(gen));
                }
                else
                {
                    clouds[i].move((cloudSpeed[i] * deltaClock.asSeconds()), 0);
                }
            }

            // Update the score text
            std::stringstream ss;
            ss << "Score = " << game.score;
            scoreText.setString(ss.str());

            // Update branches
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (game.branchPositions[i] == side::LEFT)
                {
                    game.branches[i].setPosition(610, height);
                    game.branches[i].setRotation(180);
                }
                else if (game.branchPositions[i] == side::RIGHT)
                {
                    game.branches[i].setPosition(1330, height);
                    game.branches[i].setRotation(0);
                }
                else
                {
                    game.branches[i].setPosition(3000, height);
                }
            }

            // Update yeeted log
            if (game.logActive)
            {
                game.log.move(game.logspeedX * deltaClock.asSeconds(), game.logspeedY * deltaClock.asSeconds());
                if (game.log.getPosition().x < -100 || game.log.getPosition().x > 2000)
                {
                    game.logActive = false;
                    game.log.setPosition(810, 720);
                }  
            }

            // Check if player gets squished
            if (game.branchPositions[5] == game.playerSide)
            {
                game.paused = true;
                game.acceptInput = false;

                // Replace player with gravestone
                game.rip.setPosition(525, 760);
                game.player.setPosition(2000, 660);
                messageText.setString("SQUISHED!!");
                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
            }

        } // End if game.paused

        // Draw game scene
        window.clear();
        window.draw(background);

        for(const auto& cloud : clouds) window.draw(cloud);
        for (const auto& branch : game.branches) window.draw(branch);
        window.draw(tree);
        window.draw(game.player);
        window.draw(game.axe);
        window.draw(game.log);
        window.draw(game.rip);
        window.draw(bee);

        window.draw(scoreText);
        window.draw(timeBar);
        if (game.paused) window.draw(messageText);

        // Display scene
        window.display();
    }
    
    return 0;
}

void updateBranches(Timber& game, std::mt19937& gen)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        game.branchPositions[j] = game.branchPositions[j-1];
    }
    std::uniform_int_distribution<> dist(0, 2);
    int r = dist(gen);
    game.branchPositions[0] = (r == 0) ? side::LEFT : (r == 1) ? side::RIGHT : side::NONE;
}

void setupSprites(Timber& game, const sf::Texture& textureBranch, const sf::Texture& texturePlayer,
                  const sf::Texture& textureRip, const sf::Texture& textureAxe, const sf::Texture& textureLog)
{
    for (auto& branch : game.branches)
    {
        branch.setTexture(textureBranch);
        branch.setPosition(-2000, -2000);
        branch.setOrigin(220, 20);
    }
    game.player.setTexture(texturePlayer);
    game.player.setPosition(580, 720);
    game.rip.setTexture(textureRip);
    game.rip.setPosition(600, 860);
    game.axe.setTexture(textureAxe);
    game.axe.setPosition(700, 830);
    game.log.setTexture(textureLog);
    game.log.setPosition(810, 720);
}
