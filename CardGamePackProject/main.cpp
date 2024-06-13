#include "GameManager.h"
#include "CardDeck.h"
#include <algorithm>


using std::cout;
using std::endl;

void clearScreen();

int main()
{

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
	// clearScreen();  
	cout << endl<< game1.getzeroCnt() << endl;
	// 2. 게임 시작 창
	cout << "도둑잡기 \n";
	cout << "게임 시작 ! \n\n";

	// 2 - 1) 카드 분배
	cout << "(1) 카드를 분배합니다.\n";
	game1.dealCard();

	cout << "카드 분배 결과 \n";
	cout << "Player 1 Cards: ";
	for (const string& card : game1.getPlayer1Cards()) {
		cout << card << " ";
	}
	cout << endl;

	cout << "Player 2 Cards: ";
	for (const string& card : game1.getPlayer2Cards()) {
		cout << card << " ";
	}
	cout << endl;

	cout << "Player 3 Cards: ";
	for (const string& card : game1.getPlayer3Cards()) {
		cout << card << " ";
	}
	cout << endl << endl;

	// if JK 벡터 존재
	// cout << "조커입니다! 평정심을 유지해주세요\n"
	// else 현재 카드에 조커는 없습니다 ;

	// 2 - 2) 중복 카드를 버리기
	cout << "(2) 중복된 숫자카드를 버립니다.\n";
	cout << "카드를 버려주세요.\n";
	game1.disCard();

	cout << "Player 1 Cards: ";
	for (const string& card : game1.getPlayer1Cards()) {
		cout << card << " ";
	}
	cout << endl;

	cout << "Player 2 Cards: ";
	for (const string& card : game1.getPlayer2Cards()) {
		cout << card << " ";
	}
	cout << endl;

	cout << "Player 3 Cards: ";
	for (const string& card : game1.getPlayer3Cards()) {
		cout << card << " ";
	}
	cout << endl;

	// 2 - 3) 카드 뽑을 순서 정하기
	game1.selectRoutine();
	game1.printSelectRoutine();

	//clearScreen();
	//game1.pickCard();
	
	// 3. 턴 별 게임 진행
	while (true)
	{
		cout << endl;
		cout << "도둑잡기 \n";
		cout << game1.getGameRound() << "번째 턴입니다" << endl;

		// 우승자 확인
		string winner = game1.selectWinner();

		if (!winner.empty())
		{
			cout << winner << endl;
			break;
		}
	}

	return 0;
}
	
	

void clearScreen()
{
	system("cls");
}


