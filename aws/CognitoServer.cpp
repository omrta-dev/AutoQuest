//
// Created by omar on 1/24/20.
//

#include "CognitoServer.hpp"
#include <memory>

#include <aws/core/Aws.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/cognito-idp/model/GetUserRequest.h>
#include <aws/cognito-idp/model/GetUserResult.h>
#include <aws/cognito-idp/model/AdminAddUserToGroupRequest.h>
#include <aws/cognito-idp/CognitoIdentityProviderClient.h>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

void CognitoServer::startServer()
{
    initCoginto();

    sf::TcpListener tcpListener;
    if (tcpListener.listen(port_) != sf::Socket::Done)
    {
        // error
        return;
    }
    std::cout << "Server is listening to port " << port_ << ", waiting for connections... " << std::endl;

    while (true)
    {
        // Wait for a connection
        sf::TcpSocket socket;
        if (tcpListener.accept(socket) != sf::Socket::Done)
        {
            // error
            return;
        }
        std::cout << "Client connected: " << socket.getRemoteAddress() << std::endl;

        // Receive a message back from the client
        char in[1024];
        std::size_t received;
        if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
            return;
        std::cout << "Answer received from the client: \"" << in << "\"" << std::endl;
        std::cout << "Attempting to validate that information with AWS Cognito" << std::endl;

        // Send a message to the connected client
        const std::string msg = isUserValid(in) ? "User is valid!" : "User is not valid!";
        if (socket.send(msg.c_str(), sizeof(msg.c_str()) * msg.size()) != sf::Socket::Done)
            return;
        std::cout << "Message sent to the client: \"" << msg << "\"" << std::endl;

    }
}

void CognitoServer::initCoginto()
{
    Aws::SDKOptions options;
    Aws::Utils::Logging::LogLevel logLevel{Aws::Utils::Logging::LogLevel::Error};
    options.loggingOptions.logger_create_fn = [logLevel] {
        return std::make_shared<Aws::Utils::Logging::ConsoleLogSystem>(logLevel);
    };
    Aws::InitAPI(options);
    Aws::Client::ClientConfiguration clientConfiguration;
    clientConfiguration.region = region_;

    amazonCognitoClient_ = Aws::MakeShared<Aws::CognitoIdentityProvider::CognitoIdentityProviderClient>(
            "CognitoIdentityProviderClient", clientConfiguration);
}

void CognitoServer::addUserToGroup(const std::string username)
{
    Aws::CognitoIdentityProvider::Model::AdminAddUserToGroupRequest addUserToGroupRequest;
    addUserToGroupRequest.SetGroupName("PlayerGroup");
    addUserToGroupRequest.SetUsername(username);
    addUserToGroupRequest.SetUserPoolId(poolId_);

    Aws::CognitoIdentityProvider::Model::AdminAddUserToGroupOutcome adminAddUserToGroupOutcome{
            amazonCognitoClient_->AdminAddUserToGroup(addUserToGroupRequest)};
    if (adminAddUserToGroupOutcome.IsSuccess())
    {
        std::cout << "All good!" << std::endl;
    } else
    {
        const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = adminAddUserToGroupOutcome.GetError();
        std::cout << "Error adding user " << username << " to group: " << error.GetMessage() << std::endl;
    }
}

bool CognitoServer::isUserValid(const std::string &accessToken)
{
    Aws::CognitoIdentityProvider::Model::GetUserRequest getUserRequest;
    getUserRequest.SetAccessToken(accessToken);
    Aws::CognitoIdentityProvider::Model::GetUserOutcome getUserOutcome{amazonCognitoClient_->GetUser(getUserRequest)};
    if (getUserOutcome.IsSuccess())
    {
        Aws::CognitoIdentityProvider::Model::GetUserResult getUserResult{getUserOutcome.GetResult()};
        std::string username = getUserResult.GetUsername();
        std::cout << "User logged in : " << username << std::endl;

        for (const auto &attribute : getUserResult.GetUserAttributes())
        {
            std::cout << attribute.GetName() << " : " << attribute.GetValue() << std::endl;
        }
        std::cout << "Adding user " << username << " to group PlayerGroup" << std::endl;
        addUserToGroup(username);
        return true;
    } else
    {
        const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = getUserOutcome.GetError();
        std::cout << "Error getting user: " << error.GetMessage() << std::endl;
        return false;
    }
}
