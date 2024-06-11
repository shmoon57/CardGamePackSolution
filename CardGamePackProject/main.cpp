#include "GameManager.h"
#include "Card.h"

// 화면 초기화 함수
//void clearScreen();
using std::cout;
using std::endl;

#include "Card.h"
#include "User.h"
#include "GameManager.h"


int main()
{
<<<<<<< HEAD
	// 도둑잡기 객체 생성
	OldMaid game1;

	// 1. 게임 시작전 유저 접속 확인 창
	cout << "도둑잡기 \n";
	cout << "다른 유저를 기다리는중....";
	/*
	if (모든 유저 참가)
	{
		cout << "모든 유저가 참가하였습니다 ! "<< endl << "게임을 시작합니다";
	}
	*/

	// 화면 초기화
	clearScreen();  
	
	// 2. 게임 시작 창
	cout << "도둑잡기 \n";
	cout << "게임 시작 ! \n\n";
	
	// 2 - 1) 카드 분배
	cout << "(1) 카드를 분배합니다.\n";
	game1.dealCard();

	// if JK 벡터 존재
	// cout << "조커입니다! 평정심을 유지해주세요\n"
	// else 현재 카드에 조커는 없습니다 ;
	
	// 2 - 2) 중복 카드를 버리기
	cout << "(2) 중복된 숫자카드를 버립니다.\n";
	cout << "카드를 버려주세요.\n";
	game1.disCard();

	// 2 - 3) 카드 뽑을 순서 정하기
	game1.selectRoutine();
	game1.printSelectRoutine();
	clearScreen();

	// 3. 턴 별 게임 진행
	while (true)
	{
		cout << "도둑잡기 \n";
		cout << game1.getGameRound() << "번째 턴입니다" << endl;

		




	}

	



	



	return 0;

}


void clearScreen()
{
	system("cls");
}



/*
* 
	while (true)
	{
		// 우승자 확인
		string winner = game1.selectWinner();

		if (!winner.empty())
		{
			cout << winner << endl;
			break;
		}
	}*/
=======
	User user1;
	User user2;
	User user3;

	int gameOption;

	cout << "CARD GAME" << endl;
	cout << "1, 2, 3" << endl;
	cin >> gameOption;

	cout << user1.getGamePoint();

	Holdem holdem;
	holdem.dealCard();


}
>>>>>>> 8c6d4269cc11034bd71b4b2c76a3ecd9c11702c3
