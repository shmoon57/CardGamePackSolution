#include "User.h"

void User::showUserInfo()
{


}

string User::getID()
{
	return "0";
}

string User::getPassword()
{
	return "0";
}

string User::getNickname()
{
	return "0";
}

int User::getGamePoint()
{
	return 0;
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
	vector<vector<string>> data = openUser();

	int userNum = data.size();
	int option = 1; // 1: 회원가입 2: 기존회원
	string targetID; // 수정하는 user의 ID

	//기존 회원인지 아닌지 확인
	//아이디 비교를 통해 확인
	//기존 회원이라면 회원의 ID 저장 & 변경된 회원 정보(point)도 저장
	for (int i = 0; i < userNum; i++)
	{
		if (data[i][0] == userInfo[0])
		{
			option = 2;
			targetID = data[i][0];
			data[i][3] = userInfo[3];
		}
	}

	//회원가입인 경우 data에 새로운 회원 추가
	data.push_back(userInfo);

	//쓰기 파일 실행

	ofstream file("User.txt");

	//파일에 회원 정보 저장
	for (int i = 0; i < userNum; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			file << data[i][j] << " ";
		}
		file << "\n";
	}

	file.close();
}

vector<vector<string>> UserDataManager::openUser()
{
	//파일이 열렸는지 확인
	ifstream file("User.txt");
	if (!file.is_open()) {

	}

	//회원의 정보를 저장할 벡터
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
	
	file.close();

	return data;
}

void UserDataManager::signUp() //회원가입
{
	vector<string> user;
	string ID, password, checkingpassword, nickname;
	//ifstream read_file("User.txt");

	cout << "---------------------회원가입---------------------\n";

	while (true)
	{
		cout << "아이디			: ";
		cin >> ID;
		cout << "비밀번호			: ";
		cin >> password;
		cout << "비밀번호 확인	: ";
		cin >> checkingpassword;
		cout << "닉네임			: ";
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
	//read_file.close();
}


vector<string> UserDataManager::login() //로그인
{
	ifstream read_file("User.txt");

	if (read_file.is_open() == false)
	{
		//파일 열리지 않을 때 어떡하나
	}
	
	while (true)
	{
		//로그인 할 유저의 정보
		vector<string> user;
		string ID, password;
		cout << "아이디			: \n";
		cin >> ID;
		cout << "비밀번호			: \n";
		cin >> password;

	}

}

