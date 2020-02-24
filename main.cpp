#include <iostream>
#include "aws/CognitoServer.hpp"
#include "aws/CognitoClient.hpp"

#include "game/Game.hpp"

void showMenu()
{
}

void processInput()
{
    CognitoServer cognitoServer;
    CognitoClient cognitoClient;

    int option;
    std::cin >> option;
    switch (option)
    {
        case 0:
            cognitoServer.startServer();
            break;
        case 1:
            cognitoClient.connectClient();
            break;
        default:
            break;
    }
}

int main()
{
    Game game;
    game.startGame();
}