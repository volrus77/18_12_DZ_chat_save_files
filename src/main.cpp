
#include "chat.h"

int main()
{
	Chat chat;
	chat.startChat();

	while (chat.work())
	{
		// вход в чат конкретного пользователя (currentUsesr != nullptr)
		chat.showLoginMenu();  // меню: вход, регистрация = вход, выход

		while (chat.getcurrentUser())  // пока currentUsesr != nullptr
		{
			// меню действий пользователя в чате
			chat.showUserMenu();
		}
	}
	chat.saveChat();
	return 0;
}