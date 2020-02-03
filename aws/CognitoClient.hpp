#pragma once

#include <iostream>
#include <aws/core/Region.h>
#include <aws/core/Aws.h>
#include <aws/cognito-idp/CognitoIdentityProviderClient.h>

class CognitoClient
{
public:
    void connectClient();
    void closeClient();
    bool sendAccessTokenToServer(const std::string& accessToken);
    void logIn();
    void logOut();
    void signUp();
    void confirmRegistration();
    void forgotPassword();
    void changePassword();
    void displayInfo();
    void loggedInMenu();
    void loggedOutMenu();
private:
    std::shared_ptr<Aws::CognitoIdentityProvider::CognitoIdentityProviderClient> amazonCognitoClient_;
    Aws::SDKOptions sdkOptions_;
    const std::string region_ = Aws::Region::US_WEST_2;
    const unsigned short port_ = 27015;
    const std::string clientId_ = "108ttadt9o737in20emgl7fjpm";
    bool loggedIn_;
    std::string tokenType_;
    std::string accessToken_;
    std::string idToken_;
    std::string refreshToken_;
};
