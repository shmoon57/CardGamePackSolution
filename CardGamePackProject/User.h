#pragma once

#include <string>

using namespace std;

class User {

protected:
	string m_ID;
	string m_password;
	string m_nickname;
	int m_gamePoint;

public:
	void showUserInfo();
	string getID();
	string getPassword();
	string getNickname();
	int getGamePoint();

	void setID();
	void setPassword();
	void setNickname();
	void setGamePoint();
};