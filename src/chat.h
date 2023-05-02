

#include "AuthData.h"
#include "Message.h"
#include <string>
#include <vector>
#include <exception>
#include <memory>  // std::shared_ptr
#include <iostream>

#pragma once

const size_t MAXCOUNTUSERS = 10;

struct UserLoginEx : public std::exception
{
    const char* what() const noexcept override { return "Логин уже использован, введите другой вариант"; }
};

class Chat {

public:
    void startChat();
    void saveChat();
    void showLoginMenu();
    void showUserMenu();
    bool work() const { return work_; }
    const std::shared_ptr <AuthData> getcurrentUser() const { return currentUser_; }

private:

    void userRegistration();
    void userLogin();
    void addMessage();
    void showChat() const;
    void showAllUsers() const;
    const std::shared_ptr <AuthData> getUserLog(const std::string& _login) const;
    //shared_ptr <AuthData> getUserName(const string& _name) const;

    std::vector <AuthData> userArr_;   
    std::vector <Message> messageArr_;
    std::string users_file_ {"users.txt"};
    std::string messages_file_ {"message.txt"};
    std::shared_ptr <AuthData> currentUser_ { nullptr};
    bool work_ = false;

};