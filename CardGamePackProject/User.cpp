#include "User.h"

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
	if (option == 1)
	{
		data.push_back(userInfo);
		userNum++;
	}

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
	vector<vector<string>> userdata = openUser();
	vector<string> user;
	int userNum = userdata.size();
	string ID, password, checkingpassword, nickname;
	bool signUpOK;

	system("cls");
	cout << "---------------------회원가입---------------------\n";

	while (true)
	{
		cout << "아이디 : ";
		cin >> ID;

		// 기존에 아이디가 존재하는 경우
		bool ExistingUser = false;
		for (int i = 0; i < userNum; i++)
		{
			if (userdata[i][0] == ID) {
				ExistingUser = true;
			}
		}
		if (ExistingUser)
		{
			continue;
		}

		// 신규 유저인 경우
		cout << "닉네임 : ";
		cin >> nickname;
		cout << "비밀번호 : ";
		password = inputPassword();

		// 비밀번호 확인
		while (true)
		{
			cout << "비밀번호 확인 : ";
			checkingpassword = inputPassword();
			if (password != checkingpassword)
			{
				cout << "비밀번호가 일치하지 않습니다.\n";
				continue;
			}
			else
			{
				signUpOK = true;
				break;
			}
		}
		if (signUpOK)
		{
			break;
		}
	}

	user.push_back(ID);
	user.push_back(password);
	user.push_back(nickname);
	user.push_back("5000000"); // 회원가입시 기본 제공 포인트
	writeUser(user);

	cout << "회원가입이 완료되었습니다. \n";
	//read_file.close();
}

string UserDataManager::inputPassword()
{
	const char BACKSPACE = 8;
	const char RETURN = 13;

	string password;
	char ch;

	// 비밀번호 입력 받기
	while (true) {
		ch = _getch();

		if (ch == RETURN) {
			std::cout << std::endl;
			break;
		}
		else if (ch == BACKSPACE) {
			if (!password.empty()) {
				std::cout << "\b \b"; // 백스페이스 처리
				password.pop_back();
			}
		}
		else {
			std::cout << '*'; // '*'로 대체
			password.push_back(ch);
		}
	}

	return password;

}

void User::login() //로그인 return : Nickname, point
{
	vector<vector<string>> data = openUser();
	bool isUser = false;
	int userNum = data.size();
	int userIndex = -1; // 로그인하는 user의 index
	string ID, password;

	system("cls");
	cout << "---------------------로그인---------------------\n";

	//ID가 존재하는 지 확인
	while (isUser == false)
	{
		cout << "아이디 : ";
		cin >> ID;
		// data를 살피며 id가 존재하는지 확인
		for (int i = 0; i < userNum; i++)
		{
			if (ID == data[i][0])
			{
				isUser = true;
				userIndex = i;
				break;
			}
		}

		if (isUser == false)
		{
			cout << "존재하지 않는 아이디입니다. \n";
			cout << "다시 입력해주세요. \n";
		}
	}

	//비밀번호 확인
	while (true)
	{
		cout << "비밀번호 : ";
		password = inputPassword();

		if (password == data[userIndex][1])
		{
			cout << "로그인에 성공했습니다. \n";
			break;
		}
		else
		{
			cout << "다시 입력해주세요 : \n";
		}
	}

	//User에 정보 저장
	setID(data[userIndex][0]);
	setPassword(data[userIndex][1]);
	setNickname(data[userIndex][2]);
	setGamePoint(data[userIndex][3]);
}

string User::getID()
{
	return this->m_ID;
}

string User::getPassword()
{
	return this->m_password;
}

string User::getNickname()
{
	return this->m_nickname;
}

string User::getGamePoint()
{
	return this->m_gamePoint;
}

void User::setID(string ID)
{
	this->m_ID = ID;
}

void User::setPassword(string PW)
{
	this->m_password = PW;
}

void User::setNickname(string Nickname)
{
	this->m_nickname = Nickname;
}

void User::setGamePoint(string gamePoint)
{
	this->m_gamePoint = gamePoint;
}

