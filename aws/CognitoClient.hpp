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
    void sendAccessTokenToServer(const std::string& accessToken);
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
    std::shared_ptr<Aws::CognitoIdentityProvider::CognitoIdentityProviderClient> amazonCognitoClient;
    Aws::SDKOptions options;
    const std::string region = Aws::Region::US_WEST_2;
    const unsigned short port = 27015;
    const std::string clientId = "108ttadt9o737in20emgl7fjpm";
};
