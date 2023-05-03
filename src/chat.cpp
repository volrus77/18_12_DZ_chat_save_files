#include "chat.h"
#include <filesystem>
namespace fs = std::filesystem;

void Chat::startChat()
{
	userArr_.reserve(MAXCOUNTUSERS);
	// чтение пользователей из файла если существует
	std::fstream users_stream = std::fstream(users_file_, std::ios::in | std::ios::out);
	if (!users_stream)
	{
		std::cout << "Error in opening the file" << std::endl;
	}	
	if (users_stream)
	{		
		users_stream.seekg(0, std::ios_base::beg);
		while(!users_stream.eof()) 
		 {
		 	AuthData user;
		 	//while (users_stream >> user)
			if(users_stream >> user)
		 	userArr_.push_back(user);
		 }	

		users_stream.close();		
	}			

    messageArr_.reserve(MAXCOUNTUSERS);
	// чтение сообщений из файла если существует
	std::fstream messages_stream = std::fstream(messages_file_, std::ios::in | std::ios::out);
	if (!messages_stream)
	{
		std::cout << "Error in opening the file" << std::endl;
	}	
	if (messages_stream)
	{
		messages_stream.seekg(0, std::ios_base::beg);
		while(!messages_stream.eof())
		{
			Message msg;
			if(messages_stream >> msg)
			messageArr_.push_back(msg);
		}	
		messages_stream.close();	
	}
	
	work_ = true;

}

void Chat::saveChat()
{
	// сохраняем пользователей в файл
	std::fstream users_stream = std::fstream(users_file_, std::ios::in | std::ios::out);
	if (users_stream) {

		for(auto &user : userArr_)
		{
			users_stream << user << std::endl;
		}
	}
	else
	{
		std::fstream users_stream = std::fstream(users_file_, std::ios::in | std::ios::out | 
		std::ios::trunc);
		if (users_stream) {

		// разрешения на файл только для владельца, при создании файла
		fs::permissions(users_file_, fs::perms::owner_all, fs::perm_options::replace);
 
		for(auto &user : userArr_)
		{
			users_stream << user << std::endl;
		}
		}
	}
	users_stream.close();

	// сохраняем сообщения в файл
	std::fstream messages_stream = std::fstream(messages_file_, std::ios::in | std::ios::out);
	if (messages_stream) {

		for(auto &msg : messageArr_)
		{
			messages_stream << msg << std::endl;
		}
	}
	else {
		// Для создания файла используем параметр ios::trunc
		std::fstream messages_stream = std::fstream(messages_file_, std::ios::in | std::ios::out |
		 std::ios::trunc);
		 if (messages_stream) {

		// разрешения на файл только для владельца, при создании файла
		fs::permissions(messages_file_, fs::perms::owner_all, fs::perm_options::replace);

		for(auto &msg : messageArr_)
		{
			messages_stream << msg << std::endl;
		}
	}
	}
	messages_stream.close();
}

const std::shared_ptr <AuthData> Chat::getUserLog(const std::string& login) const
{
	for(const auto &user : userArr_)
{
if (user.getLogin() == login )
{
	return std::make_shared<AuthData>(user);
}
}
return nullptr;
}

void Chat::userRegistration()
{
	std::cout << "Registation" << std::endl;
	std::string login, pass, name;

	std::cout << "Login:  " << std::endl;
	std::cin >> login;
	std::cout << "Password:   " << std::endl;
	std::cin >> pass;
	std::cout << "Name: " << std::endl;
	std::cin >> name;

	if (getUserLog(login) != nullptr || login == "all")
	{
		throw UserLoginEx();
	}

	AuthData user = AuthData(login, sha1(&pass[0], pass.length()),
		name);
	userArr_.push_back(user);
	currentUser_= std::make_shared <AuthData>(user);
}

void Chat::userLogin()
{
	{
		std::string login, pass;
		char option;

		do
		{
			std::cout << "Login menu" << std::endl;
			std::cout << "Login: " << std::endl;
			std::cin >> login;
			std::cout << "Password: " << std::endl;
			std::cin >> pass;

			currentUser_= getUserLog(login);
		
			uint* digest = sha1(&pass[0], pass.length());

			bool cmpHashes = !memcmp(
				currentUser_->getPassHash(),
				digest,
				SHA1HASHLENGTHBYTES);
			delete[] digest;

			if (currentUser_ == nullptr || !cmpHashes)
			{
				currentUser_ = nullptr;
				std::cout << "0 - exit, or any key " << std::endl;
				std::cin >> option;

				if (option == '0')
					break;
			}

		} while (!currentUser_);
	}
}

void Chat::showLoginMenu()
{
	currentUser_ = nullptr;
	char option;
	do
	{
		std::cout << " 1 - Login " << std::endl;
		std::cout << " 2 - Registration " << std::endl;
		std::cout << " 0 - exit " << std::endl;
		std::cin >> option;

		switch (option)
		{
		case '1':
			userLogin();
			break;
		case '2':
			try

			{
				userRegistration();
			}
			catch (const std::exception& ex)
			{
				std::cout << ex.what() << std::endl;
			}
			break;
		case '0':
			work_ = false;
			break;

		default:
			std::cout << "Enter 1, or 2, or 0" << std::endl;
		}
	} while (!currentUser_ && work_);
}

void Chat::showUserMenu()
{
	char option;
	std::cout << "Hello, " << currentUser_->getLogin() << std::endl;
	while (currentUser_)
	{
		std::cout << "1 - Show chat, 2 - Add Message, 3 - showAllUser, 0 - exit"
			<< std::endl;
		std::cin >> option;


		switch (option)
		{
		case '1':
			showChat();
			break;
		case '2':
			addMessage();
			break;
		case '3':
			showAllUsers();
			break;
		case '0':
			currentUser_ = nullptr;
			break;

		default:
			std::cout << "Enter 1, or 2, or 3, or 0" << std::endl;
		}
	}

}

void Chat::addMessage()
{
	std::string login, text;
	std::cout << "Enter login, or all" << std::endl;
	std::cin >> login;
	std::cout << "Enter message" << std::endl;
	std::cin.ignore();
	std::getline(std::cin, text);

	if (!((login == "all") || getUserLog(login)))
	{
		std::cout << "error: " << login << std::endl;
		return;
	}

	messageArr_.emplace_back(currentUser_->getLogin(), login, text);
}

void Chat::showAllUsers() const
{
	std::cout << "users: " << std::endl;

	for (const auto& user : userArr_)
	{
		std::cout << user.getLogin();
		//cout << endl;
		if (currentUser_->getLogin() == user.getLogin())
		{
			std::cout << "(me)";
			//cout << endl;
		}
		std::cout << std::endl;
	}
}

void Chat::showChat() const
{
	//std::cout << "Chat: " << std::endl;
	std::cout << "Messages to me: " << std::endl;
for ( const auto& msg : messageArr_)
{
	if(msg.getReceiver() == currentUser_->getLogin() || msg.getReceiver() == "all")
	{
	std::cout << "from: " << msg.getSender() << " to: " 
	<< msg.getReceiver() << std::endl;
	std::cout << msg.getText() << std::endl;
	}
}

std::cout << std::endl;
std::cout << "Messages from me: " << std::endl;
for ( const auto& msg : messageArr_)
{
	if(msg.getSender() == currentUser_->getLogin())
	{
	std::cout << "from: " << msg.getSender() 
	<< " to: " << msg.getReceiver() << std::endl;
	std::cout << msg.getText() << std::endl;
	}
}
}

 std::fstream& operator >>(std::fstream& is, Message& msg)
{
	is >> msg.sender_;
	is >> msg.receiver_;
	is.ignore();
	std::getline(is, msg.text_, '\n');
	return is;
}
std::ostream& operator <<(std::ostream& os, const Message& msg)
{
	os << msg.sender_;
	os << ' ';
	os << msg.receiver_;
	os << ' ';
	os << msg.text_;
	//os << '\n';  // и без этого происходит переход на новую строку
	return os;
}

std::fstream& operator >>(std::fstream& is, AuthData& user)
{
	is >> user.login_;   
    for(size_t i = 0; i < SHA1HASHLENGTHUINTS; i++ )
    {
	is >> *(user.pass_sha1_hash_ + i);
    }
	is >> user.name_;
	return is;
}
std::ostream& operator <<(std::ostream& os, const AuthData& user)
{
	os << user.login_;
    os << ' ';
    for(size_t i = 0; i < SHA1HASHLENGTHUINTS; i++ )
    {
	os << *(user.pass_sha1_hash_ + i);
	os << ' ';
    }
	os << user.name_;
	
	//os << '\n'; // и без этого происходит переход на новую строку
	return os;
}
 