
#include <ctime>

#include "CardDeck.h"
#include "User.h"
#include "GameManager.h"

int main()
{
	UserDataManager manager;
	User user;
	
	//회원가입, 로그인 선택
	while (true)
	{
		system("cls");
		cout << "♠CARD GAME♠\n";
		string option = "0";
		cout << "1. 회원가입 2. 로그인 3. 프로그램 종료 \n";
		cin >> option;
		if (option == "1")
		{
			manager.signUp();
		}
		else if (option == "2")
		{
			user.login();
			break;
		}
		else if (option == "3")
		{
			cout << "프로그램을 종료합니다.\n";
			break;
		}else
		{
			cout << "잘못된 입력입니다.\n";
		}
	}





}