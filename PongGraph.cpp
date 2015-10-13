/////////////////////////////
//         Headers         //
/////////////////////////////
#include <SFML/Graphics.hpp>
#include "Vector.hpp"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>

//Constraints
Vector newVelo1;
Vector newVelo2;
bool start = true;
const float pi = 3.14159f;
const int gameWidth = 800;
const int gameHeight = 600;
const float gW = 800.f;
const float gH = 600.f;
float MAXBOUNCE = 0.01f;
Vector leftSpeed(0.f, 400.f);
Vector rightSpeed(0.f, 400.f);
Vector ballSpeed1(250.f, 100.f);
Vector ballSpeed2(-250.f, 100.f);
stringstream ss1;
stringstream ss2;

using namespace std;
int main()
{
    sf::Vector2f padSize(25, 100);
    float ballRad = 10.f;
    Vector comp;

    //Create the window
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "Pong Game",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);


    //Create the left paddle
    sf::RectangleShape leftPad;
    leftPad.setSize(padSize - sf::Vector2f(3, 3));
    leftPad.setOutlineThickness(3);
    leftPad.setOutlineColor(sf::Color::Black);
    leftPad.setFillColor(sf::Color(100, 100, 200));
    leftPad.setOrigin(padSize / 2.f);


    //Create the right paddle
    sf::RectangleShape rightPad;
    rightPad.setSize(padSize - sf::Vector2f(3, 3));
    rightPad.setOutlineThickness(3);
    rightPad.setOutlineColor(sf::Color::Black);
    rightPad.setFillColor(sf::Color(100, 100, 200));
    rightPad.setOrigin(padSize / 2.f);
    Vector vPar1;
    Vector vPerp1;
    Vector nvPar1;

    //Create the first ball
    sf::CircleShape ball;
    ball.setRadius(ballRad);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(sf::Color::Black);
    ball.setFillColor(sf::Color(200, 100, 100));
    ball.setOrigin(ballRad / 2, ballRad / 2);
    Vector vPar2;
    Vector vPerp2;
    Vector nvPar2;


	//Create the second ball
    sf::CircleShape ball2;
    ball2.setRadius(ballRad);
    ball2.setOutlineThickness(3);
    ball2.setOutlineColor(sf::Color::Black);
    ball2.setFillColor(sf::Color(200, 100, 100));
    ball2.setOrigin(ballRad / 2, ballRad / 2);

    //Get font file
    sf::Font font;
    if(!font.loadFromFile("sansation.ttf"))
        return EXIT_FAILURE;

    //Initialize the pause message
    sf::Text pauseMsg;
    pauseMsg.setFont(font);
    pauseMsg.setCharacterSize(40);
    pauseMsg.setPosition(120.f, 150.f);
    pauseMsg.setColor(sf::Color::White);
    pauseMsg.setString("Welcome to pong!\nPress space to start the game");

    //Initialize the scoreboards
    //Left Scoreboard
    sf::Text leftScore;
    leftScore.setFont(font);
    leftScore.setCharacterSize(20);
    leftScore.setPosition(gW/4.f, gH - 50);
    leftScore.setColor(sf::Color::White);
    int score1 = 0;
    ss1 << score1;
    string score1str = ss1.str();
    leftScore.setString(score1str);

    //Right Scoreboard
    sf::Text rightScore;
    rightScore.setFont(font);
    rightScore.setCharacterSize(20);
    rightScore.setPosition(gW - gW/4.f, gH - 50);
    rightScore.setColor(sf::Color::White);
    int score2 = 0;
    ss2 << score2;
    string score2str = ss2.str();
    rightScore.setString(score2str);

    //Properties of paddles
    //sf::Clock AIClock; // Clock for AI
    //const float paddleSpeed = 400.f;
	//const float ballSpeed =400.f;

	//Game clock
	sf::Clock gameClock;
	bool isPlaying = false;

	while(window.isOpen())
	{
		//Handles the events
		sf::Event event;
		while (window.pollEvent(event))
		{
			//Window closed or escape key pressed = exit
			if((event.type == sf::Event::Closed) ||
			   ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			   {
					window.close();
					break;
				}

				//Game plays when space is pressed
				if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
				{
					if(!isPlaying)
					{
						//Start the game
						//Can also be used to start a new round
						isPlaying = true;
						gameClock.restart();

						//Set the position of both paddles and balls
						leftPad.setPosition(10 + padSize.x / 2, gameHeight/2);
						rightPad.setPosition(gameWidth - 10  - padSize.x, gameHeight/2);
						ball.setPosition(gameWidth / 2 - 200, gameHeight /2);
						ball2.setPosition(gameWidth / 2 + 200, gameHeight /2);
					}
				}
		}

		if (isPlaying)
		{
			float deltaTime = gameClock.restart().asSeconds();

			//Left Paddle Movement
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (leftPad.getPosition().y - padSize.y / 2 > 5.f))
			{
				//leftPad.move(0.f, -paddleSpeed * deltaTime);
				leftPad.setPosition(leftPad.getPosition().x, leftPad.getPosition().y - (leftSpeed.y * deltaTime));
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (leftPad.getPosition().y + padSize.y / 2 < gameHeight - 5.f))
			{
				//leftPad.move(0.f, paddleSpeed * deltaTime);
				leftPad.setPosition(leftPad.getPosition().x, leftPad.getPosition().y + (leftSpeed.y * deltaTime));
			}

			//Right Paddle Movement
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (rightPad.getPosition().y - padSize.y / 2 > 5.f))
			{
				//rightPad.move(0.f, -paddleSpeed * deltaTime);
				rightPad.setPosition(rightPad.getPosition().x, rightPad.getPosition().y - (rightSpeed.y * deltaTime));
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (rightPad.getPosition().y + padSize.y / 2 < gameHeight - 5.f))
			{
				rightPad.setPosition(rightPad.getPosition().x, rightPad.getPosition().y + (rightSpeed.y * deltaTime));
			}

			// Create new vectors based from ball position
			Vector currentBallPos1(ball.getPosition().x, ball.getPosition().y);
			Vector currentBallPos2(ball2.getPosition().x, ball2.getPosition().y);
            newVelo1 = comp.scalarMult(ballSpeed1, deltaTime);
            newVelo2 = comp.scalarMult(ballSpeed2, deltaTime);
			ball.setPosition(currentBallPos1.x + newVelo1.x, currentBallPos1.y + newVelo1.y);
			ball2.setPosition(currentBallPos2.x + newVelo2.x, currentBallPos2.y + newVelo2.y);

			//Collision check
			//Left Screen
			if((ball.getPosition().x - ballRad <0.f) || (ball2.getPosition().x - ballRad < 0.f))
			{
			    Vector sp(250.f, 100.f);
			    Vector sp2(-250.f, 100.f);
				gameClock.restart();
				//Set the position of both paddles and balls
                leftPad.setPosition(10 + padSize.x / 2, gameHeight/2);
                rightPad.setPosition(gameWidth - 10  - padSize.x, gameHeight/2);
                ball.setPosition(gameWidth / 2 - 50, gameHeight /2);
                ball2.setPosition(gameWidth / 2 + 50, gameHeight /2);
                ballSpeed1 = sp;
                ballSpeed2 = sp2;
                score2++;
                ss2 << score2;
                string score2str = ss2.str();
                rightScore.setString(score2str.substr(score2str.length()-1, score2str.length()));
			}
			//Right Screen
			if((ball.getPosition().x + ballRad > gameWidth) || (ball2.getPosition().x + ballRad > gameWidth))
			{
			    Vector sp(250.f, 100.f);
			    Vector sp2(-250.f, 100.f);
				gameClock.restart();
				//Set the position of both paddles and balls
                leftPad.setPosition(10 + padSize.x / 2, gameHeight/2);
                rightPad.setPosition(gameWidth - 10  - padSize.x, gameHeight/2);
                ball.setPosition(gameWidth / 2 - 50, gameHeight /2);
                ball2.setPosition(gameWidth / 2 + 50, gameHeight /2);
                ballSpeed1 = sp;
                ballSpeed2 = sp2;
                score1++;
                ss1 << score1;
                string score1str = ss1.str();
                leftScore.setString(score1str.substr(score1str.length()-1, score1str.length()));
			}
			//Top Screen
			if((ball.getPosition().y - ballRad <0.f) || (ball2.getPosition().y - ballRad < 0.f))
			{
				Vector normal(0, 1);
				if(ball.getPosition().y - ballRad <0.f)
                {
                    Vector current(ball.getPosition().x, ball.getPosition().y - 5.f);
                    ballSpeed1 = comp.vectorReflection(ballSpeed1, normal);
                }
                if(ball2.getPosition().y - ballRad <0.f)
                {
                    Vector current(ball2.getPosition().x, ball2.getPosition().y - 5.f);
                    ballSpeed2 = comp.vectorReflection(ballSpeed2, normal);
                }
			}
            //Bottom Screen
			if((ball.getPosition().y + ballRad > gameHeight) || (ball2.getPosition().y + ballRad > gameHeight))
			{
			    Vector normal(0, 1);
				if(ball.getPosition().y + ballRad > gameHeight)
                {
                    Vector current(ball.getPosition().x, ball.getPosition().y - 5.f);
                    ballSpeed1 = comp.vectorReflection(ballSpeed1, normal);

                }
                if(ball2.getPosition().y + ballRad > gameHeight)
                {
                    Vector current(ball2.getPosition().x, ball2.getPosition().y - 5.f);
                    ballSpeed2 = comp.vectorReflection(ballSpeed2, normal);
                }
			}

			//Ball - Paddle Collision
			//Left Paddle

			//Ball 1
			bool ball1left = (ball.getPosition().x - ballRad < leftPad.getPosition().x + padSize.x / 2 &&
                ball.getPosition().x - ballRad > leftPad.getPosition().x &&
                ball.getPosition().y + ballRad >= leftPad.getPosition().y - padSize.y / 2 &&
                ball.getPosition().y - ballRad <= leftPad.getPosition().y + padSize.y / 2);

            //Ball2
            bool ball2left = (ball2.getPosition().x - ballRad < leftPad.getPosition().x + padSize.x / 2 &&
                ball2.getPosition().x - ballRad > leftPad.getPosition().x &&
                ball2.getPosition().y + ballRad >= leftPad.getPosition().y - padSize.y / 2 &&
                ball2.getPosition().y - ballRad <= leftPad.getPosition().y + padSize.y / 2);

			 if ((ball1left == true) || (ball2left == true))
                {
                    Vector norm(1, 0);
                    if(ball1left == true)
                    {
                        Vector newSpeed = comp.vectorReflection(ballSpeed1, norm);
                        float xsquared = std::pow((ball.getPosition().x - (leftPad.getPosition().x + padSize.x/2.f)), 2.f);
                        float ysquared = std::pow((ball.getPosition().y - (leftPad.getPosition().y + padSize.y/2.f)), 2.f);
                        float dist = std::sqrt(xsquared + ysquared);
                        Vector speedCollide = comp.scalarMult(leftSpeed, dist);
                        Vector add = comp.scalarMult(speedCollide, MAXBOUNCE);
                        ballSpeed1 = comp.vectorAdd(newSpeed, add);
                    }
                    if(ball2left == true)
                    {
                        Vector newSpeed = comp.vectorReflection(ballSpeed2, norm);
                        float xsquared = std::pow((ball2.getPosition().x - (leftPad.getPosition().x + padSize.x/2.f)), 2.f);
                        float ysquared = std::pow((ball2.getPosition().y - (leftPad.getPosition().y + padSize.y/2.f)), 2.f);
                        float dist = std::sqrt(xsquared + ysquared);
                        Vector speedCollide = comp.scalarMult(leftSpeed, dist);
                        Vector add = comp.scalarMult(speedCollide, MAXBOUNCE);
                        ballSpeed2 = comp.vectorAdd(newSpeed, add);
                    }
                }
            //Right Paddle

            //Ball 1
            bool ball1right = (ball.getPosition().x + ballRad > rightPad.getPosition().x - padSize.x / 2 &&
                ball.getPosition().x + ballRad < rightPad.getPosition().x &&
                ball.getPosition().y + ballRad >= rightPad.getPosition().y - padSize.y / 2 &&
                ball.getPosition().y - ballRad <= rightPad.getPosition().y + padSize.y / 2);
            //Ball 2
            bool ball2right = (ball2.getPosition().x + ballRad > rightPad.getPosition().x - padSize.x / 2 &&
                ball2.getPosition().x + ballRad < rightPad.getPosition().x &&
                ball2.getPosition().y + ballRad >= rightPad.getPosition().y - padSize.y / 2 &&
                ball2.getPosition().y - ballRad <= rightPad.getPosition().y + padSize.y / 2);
                if ((ball1right == true) || (ball2right == true))
                {
                    Vector norm(-1,0);
                    if(ball1right == true)
                    {
                        Vector newSpeed = comp.vectorReflection(ballSpeed1, norm);
                        float xsquared = std::pow((ball.getPosition().x - (rightPad.getPosition().x + padSize.x/2.f)), 2.f);
                        float ysquared = std::pow((ball.getPosition().y - (rightPad.getPosition().y + padSize.y/2.f)), 2.f);
                        float dist = std::sqrt(xsquared + ysquared);
                        Vector speedCollide = comp.scalarMult(rightSpeed, dist);
                        Vector add = comp.scalarMult(speedCollide, MAXBOUNCE);
                        ballSpeed1 = comp.vectorAdd(newSpeed, add);
                    }
                    if(ball2right == true)
                    {
                        Vector newSpeed = comp.vectorReflection(ballSpeed2, norm);
                        float xsquared = std::pow((ball2.getPosition().x - (rightPad.getPosition().x + padSize.x/2.f)), 2.f);
                        float ysquared = std::pow((ball2.getPosition().y - (rightPad.getPosition().y + padSize.y/2.f)), 2.f);
                        float dist = std::sqrt(xsquared + ysquared);
                        Vector speedCollide = comp.scalarMult(rightSpeed, dist);
                        Vector add = comp.scalarMult(speedCollide, MAXBOUNCE);
                        ballSpeed2 = comp.vectorAdd(newSpeed, add);
                    }
                }

            //Ball to Ball Collision
              bool xc = (ball.getPosition().x < ball2.getPosition().x);
              bool yc = (ball.getPosition().x < ball2.getPosition().x);
              float ball1ball2distx = std::pow((ball.getPosition().x - ball2.getPosition().x), 2.f);
              float ball1ball2disty = std::pow((ball.getPosition().y - ball2.getPosition().y), 2.f);
              float dist = std::sqrt(ball1ball2distx + ball1ball2disty);
              if (dist <= 2.f * ballRad)
               {
                   float overlap = 2*ballRad - dist;
                    Vector disp(ball.getPosition().x - ball2.getPosition().x, ball.getPosition().y - ball2.getPosition().y);
                    Vector norm = comp.vectorNormalize(disp);
                    Vector ball2push = comp.scalarMult(norm, overlap/2);
                    Vector ball1push = comp.scalarMult(ball2push, -1);

                    ball.setPosition(ball.getPosition().x + ball2push.x, ball.getPosition().y + ball2push.y);
                    ball2.setPosition(ball2.getPosition().x + ball1push.x, ball2.getPosition().y + ball1push.y);

                    Vector axisCollision(ball1ball2distx, ball1ball2disty);
                    vPar1 = comp.vectorProjection(ballSpeed1, axisCollision);
                    vPar2 = comp.vectorProjection(ballSpeed2, axisCollision);
                    vPerp1 = comp.vectorSubtract(ballSpeed1, vPar1);
                    vPerp2 = comp.vectorSubtract(ballSpeed2, vPar2);
                    nvPar1 = vPar2;
                    nvPar2 = vPar1;
                    ballSpeed1 = comp.vectorAdd(vPerp1, nvPar1);
                    ballSpeed2 = comp.vectorAdd(vPerp2, nvPar2);
               }
            if(score1 == 5)
            {
                isPlaying = false;
                pauseMsg.setString("Left Paddle wins! \n Press Space to start another round \n or press Esc to exit");
                score1 = 0;
                score2 = 0;
                ss1 << score1;
                ss2 << score2;
                string score1str = ss1.str();
                string score2str = ss2.str();
                leftScore.setString(score1str.substr(score1str.length()-1, score1str.length()));
                rightScore.setString(score2str.substr(score2str.length()-1, score2str.length()));

            }
            if(score2 == 5)
            {
                isPlaying = false;
                pauseMsg.setString("Right Paddle wins! \n Press Space to start another round \n or press Esc to exit");
                score1 = 0;
                score2 = 0;
                ss1 << score1;
                ss2 << score2;
                string score1str = ss1.str();
                string score2str = ss2.str();
                leftScore.setString(score1str.substr(score1str.length()-1, score1str.length()));
                rightScore.setString(score2str.substr(score2str.length()-1, score2str.length()));
            }

		}

		window.clear(sf::Color(50, 200, 50));

        if (isPlaying)
        {
            // Draw the paddles and the ball
            window.draw(leftPad);
            window.draw(rightPad);
            window.draw(ball);
            window.draw(ball2);
            window.draw(leftScore);
            window.draw(rightScore);
        }
        else
        {
            // Draw the pause message
            window.draw(pauseMsg);
        }

        // Display things on screen
        window.display();
	}
	return EXIT_SUCCESS;
}

