#pragma once
#include <iostream>
#include <memory>

#include <aws/core/Region.h>
#include <aws/cognito-idp/CognitoIdentityProviderClient.h>

class CognitoServer
{
public:
    void startServer();
private:
    void initCoginto();
    bool isUserValid(const std::string& accessToken);

    std::shared_ptr<Aws::CognitoIdentityProvider::CognitoIdentityProviderClient> amazonCognitoClient_;
    const std::string region_ = Aws::Region::US_WEST_2;
    const unsigned short port_ = 27015;
};
