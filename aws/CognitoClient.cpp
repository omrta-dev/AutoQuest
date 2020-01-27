#include "CognitoClient.hpp"

#include <memory>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>

#include <tabulate/table.hpp>
#include <aws/core/utils/Outcome.h>
#include <aws/cognito-idp/model/SignUpRequest.h>
#include <aws/cognito-idp/model/SignUpResult.h>

void CognitoClient::connectClient()
{
    Aws::Utils::Logging::LogLevel logLevel{Aws::Utils::Logging::LogLevel::Error};
    options.loggingOptions.logger_create_fn = [logLevel] {
        return std::make_shared<Aws::Utils::Logging::ConsoleLogSystem>(logLevel);
    };

    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration clientConfiguration;
    clientConfiguration.region = region;    // region must be set for Cognito operations

    amazonCognitoClient = Aws::MakeShared<Aws::CognitoIdentityProvider::CognitoIdentityProviderClient>(
            "CognitoIdentityProviderClient", clientConfiguration);

    loggedOutMenu();
}

void CognitoClient::closeClient()
{
    Aws::ShutdownAPI(options);
}

void CognitoClient::sendAccessTokenToServer(const std::string &accessToken)
{
    // Ask for the server address
    sf::IpAddress server;
    do
    {
        std::cout << "Type the address or name of the server to connect to: ";
        std::cin >> server;
    } while (server == sf::IpAddress::None);

    // Create a socket for communicating with the server
    sf::TcpSocket socket;

    // Connect to the server
    if (socket.connect(server, port) != sf::Socket::Done)
        return;
    std::cout << "Connected to server " << server << std::endl;

    // Send a message to the server
    const char out[] = "AKIAQBI4XOMAFNBH4R4F";
    if (socket.send(out, sizeof(out)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;

    // Receive a message from the server
    char in[256];
    std::size_t received;
    if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
        return;
    std::cout << "Message received from the server: \"" << in << "\"" << std::endl;
}

void CognitoClient::logIn()
{

}

void CognitoClient::logOut()
{

}

void CognitoClient::signUp()
{
    std::string username;
    std::cout << "Enter desired username: ";
    std::cin >> username;

    std::string password;
    std::string checkPassword;
    while (password.empty())
    {
        std::cout << std::endl << "Passwords require 8 characters minimum, at least one number, " << std::endl
                  << "at least one special character and at least one each of capital and lower case." << std::endl;
        std::cout << "What password would you like to use? ";
        std::cin >> password;

        std::cout << "Please type your password again: ";
        std::cin >> checkPassword;

        if (password != checkPassword)
        {
            std::cout << std::endl << "Passwords don't match, try again." << std::endl;
            password.clear();
            checkPassword.clear();
        }
    }

    std::string email;
    std::cout << "What e-mail address would you like to use to verify your registration? ";
    std::cin >> email;

    std::cout << std::endl << "One moment while I process your registration...";

    Aws::CognitoIdentityProvider::Model::SignUpRequest signUpRequest;
    signUpRequest.SetClientId(clientId);
    signUpRequest.SetUsername(username);
    signUpRequest.SetPassword(password);

    // note that options, like the e-mail address requirement, are stored in an attributes vector
    // not exposed through the request like required fields.
    Aws::Vector<Aws::CognitoIdentityProvider::Model::AttributeType> attributes;
    Aws::CognitoIdentityProvider::Model::AttributeType emailAttribute;
    emailAttribute.SetName("email");
    emailAttribute.SetValue(email);
    attributes.push_back(emailAttribute);
    signUpRequest.SetUserAttributes(attributes);

    Aws::CognitoIdentityProvider::Model::SignUpOutcome signUpOutcome{amazonCognitoClient->SignUp(signUpRequest)};
    if (signUpOutcome.IsSuccess())
    {
        Aws::CognitoIdentityProvider::Model::SignUpResult signUpResult{signUpOutcome.GetResult()};
        std::cout << "Successful signup!" << std::endl;
        std::cout << "Details:" << std::endl;
        std::cout << "\tUser Confirmed: " << signUpResult.GetUserConfirmed() << std::endl;
        std::cout << "\tUser UUID: " << signUpResult.GetUserSub() << std::endl << std::endl;
        std::cout
                << "Please check your e-mail for confirmation code then enter the code from the confirmation menu option."
                << std::endl;
    } else
    {
        Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> error = signUpOutcome.GetError();
        std::cout << "Error signing up: " << error.GetMessage() << std::endl << std::endl;
    }
}

void CognitoClient::confirmRegistration()
{

}

void CognitoClient::forgotPassword()
{

}

void CognitoClient::changePassword()
{

}

void CognitoClient::displayInfo()
{

}

void CognitoClient::loggedInMenu()
{

}

void CognitoClient::loggedOutMenu()
{
    tabulate::Table optionTable;
    optionTable.add_row({"Select an option"})[0].format().font_align(tabulate::FontAlign::center)
            .font_style({tabulate::FontStyle::bold});
    tabulate::Table options;
    options.add_row({"0", "Log In"});
    options.add_row({"1", "Sign Up"});
    options.add_row({"2", "Confirm Registration"});
    options.add_row({"3", "Forgot Password"});
    options.add_row({"4", "Quit"});

    optionTable.add_row({options});
    while (true)
    {
        std::cout << "\n" << optionTable << "\n" << std::endl;

        std::cout << "Your choice: ";

        int selection;
        std::cin >> selection;
        switch (selection)
        {
            case 0:
                logIn();
                break;
            case 1:
                signUp();
                break;
            case 2:
                confirmRegistration();
                break;
            case 3:
                forgotPassword();
                break;
            default:
                return;
        }
    }
}
