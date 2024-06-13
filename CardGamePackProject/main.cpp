
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
		}
		else
		{
			cout << "잘못된 입력입니다.\n";
		}
	}
	
	cout << user.getGamePoint() << endl;

	while (true)
	{
		string option;
		cout << "게임을 선택하세요." << endl;
		cin >> option;
		if (option == "1")
		{
			system("cls");
			//도둑잡기
			cout << "도둑잡기를 시작합니다." << endl;
		}
		else if (option == "2")
		{
			system("cls");
			//홀덤
			cout << "홀덤을 시작합니다." << endl;
			Holdem holdem;

			cout << "카드를 섞어 배분합니다." << endl;
			holdem.dealCard();

			cout << "홀덤을 시작합니다." << endl;
			holdem.play();
		}

		else if (option == "3")
		{
			cout << "프로그램을 종료합니다." << endl;
			break;
		}
		else
		{
			cout << "잘못된 입력입니다." << endl;
		}
  }
}