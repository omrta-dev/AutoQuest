#include "CognitoClient.hpp"

#include <memory>

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <tabulate/table.hpp>

#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>
#include <aws/core/utils/Outcome.h>
#include <aws/cognito-idp/model/AttributeType.h>
#include <aws/cognito-idp/model/SignUpRequest.h>
#include <aws/cognito-idp/model/SignUpResult.h>
#include <aws/cognito-idp/model/ConfirmSignUpRequest.h>
#include <aws/cognito-idp/model/ConfirmSignUpResult.h>
#include <aws/cognito-idp/model/InitiateAuthRequest.h>
#include <aws/cognito-idp/model/InitiateAuthResult.h>
#include <aws/cognito-idp/model/ForgotPasswordRequest.h>
#include <aws/cognito-idp/model/ForgotPasswordResult.h>
#include <aws/cognito-idp/model/ConfirmForgotPasswordRequest.h>
#include <aws/cognito-idp/model/ConfirmForgotPasswordResult.h>
#include <aws/cognito-idp/model/ChangePasswordRequest.h>
#include <aws/cognito-idp/model/ChangePasswordResult.h>
#include <aws/cognito-idp/model/GetUserRequest.h>
#include <aws/cognito-idp/model/GetUserResult.h>
#include <aws/core/client/ClientConfiguration.h>

void CognitoClient::connectClient()
{
    Aws::Utils::Logging::LogLevel logLevel{Aws::Utils::Logging::LogLevel::Error};
    sdkOptions_.loggingOptions.logger_create_fn = [logLevel] {
        return std::make_shared<Aws::Utils::Logging::ConsoleLogSystem>(logLevel);
    };

    Aws::InitAPI(sdkOptions_);

    Aws::Client::ClientConfiguration clientConfiguration;
    clientConfiguration.region = region_;    // region_ must be set for Cognito operations

    amazonCognitoClient_ = Aws::MakeShared<Aws::CognitoIdentityProvider::CognitoIdentityProviderClient>(
            "CognitoIdentityProviderClient", clientConfiguration);

    loggedIn_ = false;
    loggedOutMenu();
}

void CognitoClient::closeClient()
{
    Aws::ShutdownAPI(sdkOptions_);
}

bool CognitoClient::sendAccessTokenToServer(const std::string &accessToken)
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
    if (socket.connect(server, port_) != sf::Socket::Done)
        return false;
    std::cout << "Connected to server " << server << std::endl;

    // Send a message to the server
    if (socket.send(accessToken.c_str(), accessToken.size() + 1) != sf::Socket::Done)
        return false;
    std::cout << "Message sent to the server: \"" << accessToken << "\"" << std::endl;

    // Receive a message from the server
    char in[256];
    std::size_t received;
    if (socket.receive(in, sizeof(in), received) != sf::Socket::Done)
        return false;
    std::cout << "Message received from the server: \"" << in << "\"" << std::endl;
    return true;
}

void CognitoClient::logIn()
{
    std::string username;
    std::cout << "username: ";
    std::cin >> username;

    std::string password;
    std::cout << "password: ";
    std::cin >> password;

    Aws::CognitoIdentityProvider::Model::InitiateAuthRequest initiateAuthRequest;
    initiateAuthRequest.SetClientId(clientId_);
    initiateAuthRequest.SetAuthFlow(Aws::CognitoIdentityProvider::Model::AuthFlowType::USER_PASSWORD_AUTH);

    std::map<std::string, std::string> authParameters{
            {"USERNAME", username},
            {"PASSWORD", password}
    };
    initiateAuthRequest.SetAuthParameters(authParameters);

    Aws::CognitoIdentityProvider::Model::InitiateAuthOutcome initiateAuthOutcome{
            amazonCognitoClient_->InitiateAuth(initiateAuthRequest)};
    if (initiateAuthOutcome.IsSuccess())
    {
        Aws::CognitoIdentityProvider::Model::InitiateAuthResult initiateAuthResult{initiateAuthOutcome.GetResult()};
        if (initiateAuthResult.GetChallengeName() == Aws::CognitoIdentityProvider::Model::ChallengeNameType::NOT_SET)
        {
            // for this code sample, this is what we expect, there should be no further challenges
            // there are more complex sdkOptions_, for example requiring the user to reset the password the first login
            // or using a more secure password transfer mechanism which will be covered in later examples
            const Aws::CognitoIdentityProvider::Model::AuthenticationResultType &authenticationResult = initiateAuthResult.GetAuthenticationResult();
            std::cout << std::endl << "Congratulations, you have successfully signed in!" << std::endl;
            std::cout << "\tToken Type: " << authenticationResult.GetTokenType() << std::endl;
            std::cout << "\tAccess Token: " << authenticationResult.GetAccessToken().substr(0, 20) << " ..."
                      << std::endl;
            std::cout << "\tExpires in " << authenticationResult.GetExpiresIn() << " seconds" << std::endl;
            std::cout << "\tID Token: " << authenticationResult.GetIdToken().substr(0, 20) << " ..." << std::endl;
            std::cout << "\tRefresh Token: " << authenticationResult.GetRefreshToken().substr(0, 20) << " ..."
                      << std::endl;

            loggedIn_ = true;
            tokenType_ = authenticationResult.GetTokenType();
            accessToken_ = authenticationResult.GetAccessToken();
            idToken_ = authenticationResult.GetIdToken();
            refreshToken_ = authenticationResult.GetRefreshToken();

            if (!sendAccessTokenToServer(accessToken_))
            {
                std::cout << "Unable to connect to server" << std::endl;
            }
        }
    } else
    {
        const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = initiateAuthOutcome.GetError();
        std::cout << "Error logging in: " << error.GetMessage() << std::endl << std::endl;
    }
}

void CognitoClient::logOut()
{
    loggedIn_ = false;
    tokenType_.clear();
    accessToken_.clear();
    idToken_.clear();
    refreshToken_.clear();
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
    signUpRequest.SetClientId(clientId_);
    signUpRequest.SetUsername(username);
    signUpRequest.SetPassword(password);

    // note that sdkOptions_, like the e-mail address requirement, are stored in an attributes vector
    // not exposed through the request like required fields.
    Aws::Vector<Aws::CognitoIdentityProvider::Model::AttributeType> attributes;
    Aws::CognitoIdentityProvider::Model::AttributeType emailAttribute;
    emailAttribute.SetName("email");
    emailAttribute.SetValue(email);
    attributes.push_back(emailAttribute);
    signUpRequest.SetUserAttributes(attributes);

    Aws::CognitoIdentityProvider::Model::SignUpOutcome signUpOutcome{amazonCognitoClient_->SignUp(signUpRequest)};
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
        const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = signUpOutcome.GetError();
        std::cout << "Error signing up: " << error.GetMessage() << std::endl << std::endl;
    }
}

void CognitoClient::confirmRegistration()
{
    std::cout << "Please type the user name of account you'd like to confirm: ";
    std::string userName;
    std::cin >> userName;

    std::cout << "Please type the confirmation code for your account: ";
    std::string confirmationCode;
    std::cin >> confirmationCode;

    Aws::CognitoIdentityProvider::Model::ConfirmSignUpRequest confirmSignupRequest;
    confirmSignupRequest.SetClientId(clientId_);
    confirmSignupRequest.SetUsername(userName);
    confirmSignupRequest.SetConfirmationCode(confirmationCode);

    Aws::CognitoIdentityProvider::Model::ConfirmSignUpOutcome confirmSignupOutcome{
            amazonCognitoClient_->ConfirmSignUp(confirmSignupRequest)};
    if (confirmSignupOutcome.IsSuccess())
    {
        std::cout << "Confirmation succeeded! You can now log in from the menu." << std::endl << std::endl;
    } else
    {
        const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = confirmSignupOutcome.GetError();
        std::cout << "Error confirming signup: " << error.GetMessage() << std::endl << std::endl;
    }
}

void CognitoClient::forgotPassword()
{
    std::string username;
    std::cout << "I forgot the password for username: ";
    std::cin >> username;

    Aws::CognitoIdentityProvider::Model::ForgotPasswordRequest forgotPasswordRequest;
    forgotPasswordRequest.SetClientId(clientId_);
    forgotPasswordRequest.SetUsername(username);

    Aws::CognitoIdentityProvider::Model::ForgotPasswordOutcome forgotPasswordOutcome{
            amazonCognitoClient_->ForgotPassword(forgotPasswordRequest)};
    if (forgotPasswordOutcome.IsSuccess())
    {
        std::cout << "Password reset accepted. Check your email for a reset code and input it: ";
        std::string confirmationCode;
        std::cin >> confirmationCode;

        std::cout << "New password: ";
        std::string password;
        std::cin >> password;

        Aws::CognitoIdentityProvider::Model::ConfirmForgotPasswordRequest confirmForgotPasswordRequest;
        confirmForgotPasswordRequest.SetClientId(clientId_);
        confirmForgotPasswordRequest.SetUsername(username);
        confirmForgotPasswordRequest.SetConfirmationCode(confirmationCode);
        confirmForgotPasswordRequest.SetPassword(password);

        Aws::CognitoIdentityProvider::Model::ConfirmForgotPasswordOutcome confirmForgotPasswordOutcome{
                amazonCognitoClient_->ConfirmForgotPassword(confirmForgotPasswordRequest)};
        if (confirmForgotPasswordOutcome.IsSuccess())
        {
            std::cout << "Confirmation succeeded! You can now log in from the menu." << std::endl << std::endl;
        } else
        {
            const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = confirmForgotPasswordOutcome.GetError();
            std::cout << "Error confirming password reset: " << error.GetMessage() << std::endl << std::endl;
        }
    } else
    {
        const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = forgotPasswordOutcome.GetError();
        std::cout << "Forgot password request error: " << error.GetMessage() << std::endl << std::endl;
    }
}

void CognitoClient::changePassword()
{
    if (loggedIn_)
    {
        std::cout << "Please log in before trying to change your password." << std::endl;
        return;
    }

    std::cout << "Please type the current password: ";
    std::string currentPassword;
    std::cin >> currentPassword;

    std::cout << "Please type the new password: ";
    std::string newPassword;
    std::cin >> newPassword;

    Aws::CognitoIdentityProvider::Model::ChangePasswordRequest changePasswordRequest;
    changePasswordRequest.SetAccessToken(accessToken_);
    changePasswordRequest.SetPreviousPassword(currentPassword);
    changePasswordRequest.SetProposedPassword(newPassword);

    Aws::CognitoIdentityProvider::Model::ChangePasswordOutcome changePasswordOutcome{
            amazonCognitoClient_->ChangePassword(changePasswordRequest)};
    if (changePasswordOutcome.IsSuccess())
    {
        std::cout << "Password reset successful!" << std::endl << std::endl;
    } else
    {
        const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = changePasswordOutcome.GetError();
        std::cout << "Error changing password: " << error.GetMessage() << std::endl << std::endl;
    }
}

void CognitoClient::displayInfo()
{
    if (!loggedIn_)
    {
        std::cout << "Please log in before trying to view your user information." << std::endl;
        return;
    }

    Aws::CognitoIdentityProvider::Model::GetUserRequest getUserRequest;
    getUserRequest.SetAccessToken(accessToken_);

    Aws::CognitoIdentityProvider::Model::GetUserOutcome getUserOutcome{amazonCognitoClient_->GetUser(getUserRequest)};
    if (getUserOutcome.IsSuccess())
    {
        Aws::CognitoIdentityProvider::Model::GetUserResult getUserResult{getUserOutcome.GetResult()};
        std::cout << std::endl << "User name : " << getUserResult.GetUsername() << std::endl;

        for (const auto &attribute : getUserResult.GetUserAttributes())
        {
            std::cout << attribute.GetName() << " : " << attribute.GetValue() << std::endl;
        }
    } else
    {
        const Aws::Client::AWSError<Aws::CognitoIdentityProvider::CognitoIdentityProviderErrors> &error = getUserOutcome.GetError();
        std::cout << "Error getting user: " << error.GetMessage() << std::endl << std::endl;
    }
}

void CognitoClient::loggedInMenu()
{
    tabulate::Table optionTable;
    optionTable.add_row({"Select an option"})[0].format().font_align(tabulate::FontAlign::center)
            .font_style({tabulate::FontStyle::bold});
    tabulate::Table options;
    options.add_row({"0", "DisplayInfo"});
    options.add_row({"1", "Change Password"});
    options.add_row({"2", "Log Out"});
    options.add_row({"3", "Quit"});

    optionTable.add_row({options});
    std::cout << "\n" << optionTable << "\n" << std::endl;

    std::cout << "Your choice: ";

    int selection;
    std::cin >> selection;
    switch (selection)
    {
        case 0:
            displayInfo();
            break;
        case 1:
            changePassword();
            break;
        case 2:
            logOut();
            break;
        case 3:
        default:
            return;
    }
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


