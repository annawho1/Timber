

#include<sstream>
#include<random>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
using namespace sf;

//Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];  //an array of sprite objects that can hold 6 sprite isntances
enum class side { LEFT, RIGHT, NONE };  //used to describe the position of individual branches as well as the player
side branchPositions[NUM_BRANCHES]; // an array of side types with a size of 6; each of these will be either LEFT,RIGHT,NONE

class Clouds : public Transformable {
    Texture m_texture;
    Sprite m_cloud;
    float m_x_position;
    float m_y_position;
    bool m_active = false;
    float m_speed = 0.0f;
public:
    Clouds(float x_pos, float y_pos) : m_x_position{ x_pos }, m_y_position{ y_pos }
    {
        m_texture.loadFromFile("graphics/cloud.png");
        m_cloud.setTexture(m_texture);
        m_cloud.setPosition(m_x_position, m_y_position);
    }
    float getSpeed()
    {
        return m_speed;
    }
    void setSpeed(float x)
    {
        m_speed = x;
    }
    bool getActive()
    {
        return m_active;
    }
    void setActive()
    {
        m_active = true;
    }
    void setInactive()
    {
        m_active = false;
    }
    Sprite getSprite()
    {
        return m_cloud;
    }
    void updatePosition(float x)
    {
        m_x_position += m_speed * x;
        m_cloud.setPosition(m_x_position, m_y_position);
    }
    void updateInitialPosition(float x, float y)
    {
        m_x_position = x;
        m_y_position = y;
        m_cloud.setPosition(m_x_position, m_y_position);
    }
    float getXPosition()
    {
        return m_x_position;
    }
};
class Tree : public Transformable {
    Texture m_texture;
    Sprite m_treeSprite;
    float m_x_position = 810;
    float m_y_position = 0;
    float m_x_scale = 1;
    float m_y_scale = 1;
public:
    Tree()
    {
        m_texture.loadFromFile("graphics/tree.png");
        m_treeSprite.setTexture(m_texture);
        m_treeSprite.setPosition(m_x_position, m_y_position);
    }
    Tree(float x_pos, float y_pos, float x_scale, float y_scale) : m_x_position{ x_pos }, m_y_position{ y_pos }, m_x_scale{ x_scale }, m_y_scale{ y_scale }
    {
        m_texture.loadFromFile("graphics/tree.png");
        m_treeSprite.setTexture(m_texture);
        m_treeSprite.setPosition(m_x_position, m_y_position);
        m_treeSprite.setScale(m_x_scale, m_y_scale);
    }
    Sprite getSprite() {
        return m_treeSprite;
    }
};


int main()
{
    //Create a video mode object
    VideoMode vm(1360, 760);
    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
    View view(sf::FloatRect(0, 0, 1920, 1080));
    //window.setFramerateLimit(60);
    window.setView(view);
    //Create a texture to hold the graphic on the GPU
    Texture textureBackground;
    //Load a graphic into a texture
    textureBackground.loadFromFile("graphics/background.png");
    Sprite spriteBackground;
    //Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);
    //Set the sprite background to cover the screen
    spriteBackground.setPosition(0, 0);
    //Make tree sprites
    Tree mainTree;
    Tree spriteTree1(1300, -150, 0.5, 1);
    Tree spriteTree2(1600, -240, 0.5, 1);
    Tree spriteTree3(200, 0, 0.7, 1.3);

    //Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    //Is the bee currently moving?
    bool beeActive = false;
    //How fast can the bee fly
    float beeSpeed = 0.0f;
    //make 4 cloud sprites 

    Clouds cloud1(0, 50);
    Clouds cloud2(0, 100);
    Clouds cloud3(0, 200);
    Clouds cloud4(0, 300);

    //Variables to control time itself
    Clock clock;
    //Time bar
    RectangleShape timeBar;         //SFML Class perfect for drawing rectangles
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
    //Score bar
    RectangleShape scoreBar;
    scoreBar.setSize(Vector2f(540, 110));
    scoreBar.setFillColor(Color::Black);
    scoreBar.setPosition(20, 20);

    //Track whether the game is running
    bool paused = true;
    //Draw some text
    int score = 0;
    Text messageText;
    Text scoreText;
    //Choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    messageText.setFont(font);
    scoreText.setFont(font);
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    //Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    //Position the text
    FloatRect textRect = messageText.getLocalBounds(); //initialize textRect with the coordinates of the rectangle that wraps messageText
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);
    //Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }
    //Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    //The player starts on the left
    side playerSide = side::LEFT;
    //Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 800);
    //Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);
    //Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 700;
    //Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);
    //Othe useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
    //Control player input
    bool acceptInput = false;

    //Prepare the sounds
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    //Random values for cloud speed/ height
    std::random_device myRand;
    unsigned seed = myRand();
    std::default_random_engine myRandEngine(seed);
    std::uniform_int_distribution<int> myUnifDist(-100, 300);
    std::uniform_int_distribution<int> myUnifDistSpeed(0, 200);
    std::uniform_int_distribution<int> myUnifDistBeeSpeed(200, 400);
    std::uniform_int_distribution<int> myUnifDistBeeHeight(500, 900);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                //Listen for key presses again
                acceptInput = true;
                //hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
            score = 0;
            timeRemaining = 6;
            //Make all the branches disappear starting in the second position
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }
            //Make sure gravestone is hidden
            spriteRIP.setPosition(675, 2000);
            //Move the player into position
            spritePlayer.setPosition(580, 720);
            acceptInput = true;
        }

        //Wrap the player controls to make sure we are accepting input

        if (acceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                playerSide = side::RIGHT;
                score++;
                //Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);
                //Update branches
                updateBranches(score);
                //Set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerSide = side::LEFT;
                score++;
                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);
                updateBranches(score);
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;
                acceptInput = false;
                chop.play();
            }

        }
        //Update the scene

        if (!paused)
        {
            //Measure time
            Time dt = clock.restart();
            //Subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
            if (timeRemaining <= 0.0f) {
                paused = true;
                messageText.setString("Out of time!!");
                //Reposition text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                outOfTime.play();
            }
            //Setup the bee
            if (!beeActive)
            {
                //How fast is the bee
                beeSpeed = myUnifDistBeeSpeed(myRandEngine);    // get a random number between 200 and 400 and assign to beeSpeed
                //How high is the bee
                float height = myUnifDistBeeHeight(myRandEngine);//get a random number between 500 and 900 and assign to height
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
                //Move the bee
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
                //Has the bee reached the left-hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                }
            }
            //Manage the clouds


            if (!cloud1.getActive())
            {
                float s = myUnifDistSpeed(myRandEngine);
                cloud1.setSpeed(s);
                float height = myUnifDist(myRandEngine);
                cloud1.updateInitialPosition(-200, height);
                cloud1.setActive();
            }
            else
            {
                cloud1.updatePosition(dt.asSeconds());
                if (cloud1.getXPosition() > 1920)
                {
                    cloud1.setInactive();
                }
            }
            if (!cloud2.getActive())
            {
                float s = myUnifDistSpeed(myRandEngine);
                cloud2.setSpeed(s);
                float height = myUnifDist(myRandEngine);
                cloud2.updateInitialPosition(-200, height);
                cloud2.setActive();
            }
            else
            {
                cloud2.updatePosition(dt.asSeconds());
                if (cloud2.getXPosition() > 1920)
                {
                    cloud2.setInactive();
                }
            }
            if (!cloud3.getActive())
            {
                float s = myUnifDistSpeed(myRandEngine);
                cloud3.setSpeed(s);
                float height = myUnifDist(myRandEngine);
                cloud3.updateInitialPosition(-200, height);
                cloud3.setActive();
            }
            else
            {
                cloud3.updatePosition(dt.asSeconds());
                if (cloud3.getXPosition() > 1920)
                {
                    cloud3.setInactive();
                }
            }
            if (!cloud4.getActive())
            {
                float s = myUnifDistSpeed(myRandEngine);
                cloud4.setSpeed(s);
                float height = myUnifDist(myRandEngine);
                cloud4.updateInitialPosition(-200, height);
                cloud4.setActive();
            }
            else
            {
                cloud4.updatePosition(dt.asSeconds());
                if (cloud4.getXPosition() > 1920)
                {
                    cloud4.setInactive();
                }
            }

            //Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150; //first branch will have a height of 0, second 150...sixth 750
                if (branchPositions[i] == side::LEFT)
                {
                    //Move branch to left side
                    branches[i].setPosition(610, height);
                    //Flip the sprite round the other way
                    branches[i].setOrigin(220, 40);
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);
                    branches[i].setOrigin(220, 40);
                    branches[i].setRotation(0);
                }
                else
                {
                    //Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }
            //Handle a flying log
            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
                //Has the log reached the right hand edge?
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().y > 2000)
                {
                    //Set it up ready to be a whole new log next frame
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
                //Update the score text
                std::stringstream ss;
                ss << "Score = " << score;   //concatenate variables into a string stream; whatever the value of score is, is concatenated; if score changes, ss will adapt each frame
                scoreText.setString(ss.str());  //sets the String contained in ss to scoreText

            }
            //has the player been squished by a branch?
            if (branchPositions[5] == playerSide)
            {
                paused = true;
                acceptInput = false;

                //Draw gravestone
                spriteRIP.setPosition(525, 760);
                //hide player
                spritePlayer.setPosition(2000, 660);
                //Change text of message
                messageText.setString("SQUISHED!!");
                //Center it on the screen
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                death.play();
            }
        }
        window.clear();

        //Draw our game scene here
        window.draw(spriteBackground);

        window.draw(cloud1.getSprite());
        window.draw(cloud2.getSprite());
        window.draw(cloud3.getSprite());
        window.draw(cloud4.getSprite());

        window.draw(mainTree.getSprite());
        window.draw(spriteTree1.getSprite());
        window.draw(spriteTree2.getSprite());
        window.draw(spriteTree3.getSprite());

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);
        window.draw(spriteBee);
        window.draw(scoreBar);
        window.draw(scoreText);
        window.draw(timeBar);
        if (paused)
        {
            window.draw(messageText);
        }

        //Show everything we just drew
        window.display();
    }
    return 0;
}

void updateBranches(int seed) {
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }
    //Spawn a new branch at position 0
    std::default_random_engine myRandEngine(seed);
    std::uniform_int_distribution<int> myUnifDistBranch(0, 4);
    int r = myUnifDistBranch(myRandEngine);   //generate a random number between 0 and 4
    switch (r) {
    case 0: branchPositions[0] = side::LEFT; break;
    case 1: branchPositions[0] = side::RIGHT; break;
    default: branchPositions[0] = side::NONE; break;
    }
}
