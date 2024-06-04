#include "User.h"

void User::showUserInfo()
{


}

string User::getID()
{


}

string User::getPassword()
{


}

string User::getNickname()
{

}

int User::getGamePoint()
{

}

void User::setID()
{

}

void User::setPassword()
{

}

void User::setNickname()
{

}

void User::setGamePoint()
{

}
 
void UserDataManager::writeUser(vector<string> userInfo) //회원정보 저장
{

}

void UserDataManager::openUser()
{

}

void UserDataManager::signUp() //회원가입
{
	vector<string> user;
	string ID, password, checkingpassword, nickname;
	ifstream read_file("User.txt");

	cout << "---------------------회원가입---------------------\n";

	if (read_file.is_open() == false)
	{
		//파일 열리지 않을 때 어떡하나
	}

	while (true)
	{
		cout << "아이디			: \n";
		cin >> ID;
		cout << "비밀번호			: \n";
		cin >> password;
		cout << "비밀번호 확인	: \n";
		cin >> checkingpassword;
		cout << "닉네임			: \n";
		cin >> nickname;
		
		if (password != checkingpassword)
		{
			cout << "비밀번호가 일치하지 않습니다.\n";
		}
		else {
			break;
		}
	}
	
	user.push_back(ID);
	user.push_back(password);
	user.push_back(nickname);
	user.push_back("1000000"); // 회원가입시 기본 제공 포인트
	writeUser(user);

	cout << "회원가입이 완료되었습니다. \n";
	read_file.close();
}


vector<string> UserDataManager::login() //로그인
{
	vector<string> user;
	ifstream read_file("User.txt");

	if (read_file.is_open() == false)
	{
		//파일 열리지 않을 때 어떡하나
	}
	
	while (true)
	{
		cout << "아이디			: \n";
		cout << "비밀번호			: \n";
	}

}

