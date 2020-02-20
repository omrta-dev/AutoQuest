#include <iostream>
#include <tabulate/table.hpp>
#include "aws/CognitoServer.hpp"
#include "aws/CognitoClient.hpp"

#include "game/Game.hpp"

void showMenu()
{
    tabulate::Table optionTable;
    optionTable.add_row({"Select an option"})[0].format().font_align(tabulate::FontAlign::center)
            .font_style({tabulate::FontStyle::bold});
    tabulate::Table options;
    options.add_row({"0", "Start Server"});
    options.add_row({"1", "Send Data"});
    optionTable.add_row({options});

    std::cout << "\n" << optionTable << "\n" << std::endl;
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
    //showMenu();
    //processInput();
    //return 0;
}