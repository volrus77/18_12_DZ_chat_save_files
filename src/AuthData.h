

#include "sha1.h"
#include <string>
#include <cstring>  // memcpy
#include <iostream>
#include <fstream>

#pragma once


struct AuthData {
    public:

 enum CellStatus {
        free,
        engaged,
        deleted
    };

        AuthData() :
            login_(""),
            //pass_sha1_hash_(0),
            pass_sha1_hash_{new uint[SHA1HASHLENGTHUINTS]},
            name_(""),
            status_(CellStatus::free)
             {}
             
        /*~AuthData() {
            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
        }*/
        // копирует логин, забирает внутрь хеш
        AuthData(std::string login, uint* sh1, std::string name) {
            login_ = login;
            pass_sha1_hash_ = sh1;
            name_ = name;
            status_ = CellStatus::engaged;
        }
        // копирует всё
        AuthData& operator = (const AuthData& other) {
            login_ = other.login_;

            if (pass_sha1_hash_ != 0)
                delete[] pass_sha1_hash_;
            pass_sha1_hash_ = new uint[SHA1HASHLENGTHUINTS];
            memcpy(pass_sha1_hash_, other.pass_sha1_hash_, SHA1HASHLENGTHBYTES);
            name_ = other.name_;
            status_ = other.status_;

            return *this;
        }

        const std::string& getLogin() const {return login_;}
        const uint* getPassHash() const {return pass_sha1_hash_;}
        friend std::fstream& operator >>(std::fstream& is, AuthData& obj);  // читаем из файла
	    friend std::ostream& operator <<(std::ostream& os, const AuthData& obj);  // пишем в файл

    private:
        std::string login_;
        uint* pass_sha1_hash_;
        std::string name_;

        CellStatus status_;
    };

   