#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

class UserDataManager :public User
{
public:
	void writeUser(vector<string> userInfo);
	void openUser();
	void signUp();
	vector<string> login();
};