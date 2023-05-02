
#include <string>
#include <iostream>
#include <fstream>

#pragma once

struct Message
    {
		public:
        Message() = default;
        Message(const std::string& sender, const std::string& receiver,
            const std::string& text)
            : sender_(sender), receiver_(receiver), text_(text)
        {}

        const std::string& getSender() const { return sender_;}
        const std::string& getReceiver() const { return receiver_;}
        const std::string& getText() const { return text_;}

        friend std::fstream& operator >>(std::fstream& is, Message& obj);  // читаем из файла
	    friend std::ostream& operator <<(std::ostream& os, const Message& obj);  // пишем в файл

private:
        std::string sender_;
        std::string receiver_;
        std::string text_;
    };

   