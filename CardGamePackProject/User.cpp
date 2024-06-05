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
 
void UserDataManager::writeUser(vector<string> userInfo) 
//회원정보 저장
//userInfo = {ID, password, nickname, point}
{
	fstream file("User.txt", ios::in | ios::out);
	if (!file.is_open()) {

	}
	vector<vector<string>> data;
	string line;

	//파일 읽어오기
	while (getline(file, line)) {
		istringstream iss(line);
		string word;
		vector<std::string> row;

		// 줄에서 단어를 분리하여 벡터에 저장
		while (iss >> word) {
			row.push_back(word);
		}

		// 한 줄의 데이터를 이중 벡터에 추가
		data.push_back(row);
	}

	int userNum = data.size();
	int option = 1; // 1: 회원가입 2: 기존회원
	string targetID; // 수정하는 user의 ID

	//기존 회원인지 아닌지 확인
	//아이디 비교를 통해 확인
	//기존 회원이라면 회원의 ID 저장 & 변경된 회원 정보(point)도 저장
	for (int i = 0; i < userNum; i++)
	{
		if (data[userNum][0] == userInfo[0])
		{
			option = 2;
			targetID = data[userNum][0];
			data[userNum][3] = userInfo[3];
		}
	}

	//option 1 : txt파일 마지막 줄에 회원정보 추가
	//option 2 : txt파일에 기존 회원 정보 수정
	if (option == 1)
	{
		file.clear();
		file.seekp(0, ios::end);
		for (int i = 0; i < 4; i++)
		{
			file << userInfo[i] << " " ; // 회원정보 추가
		}
	}
	else if (option == 2)
	{
		file.clear();
		for (int i = 0; i < userNum; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				file << data[i][j] << " ";
			}
			file << "\n";
		}
	}

	file.close();
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

