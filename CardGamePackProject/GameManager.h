#pragma once
// time 함수 사용
#include <iostream>
#include <ctime>
#include <cstdlib>
// random_suffle , remove 함수 사용
#include <algorithm>
#include <vector>
#include <string>

#include "User.h"
#include "CardDeck.h"

using namespace std;

class GameManager
{
public:
	GameManager() { m_gameType = 0; m_gamePrice = 0; }
	int getGamePrice();
	void setGamePrice(int price);

	// 가상 함수 선언
	virtual string selectWinner() = 0;
	virtual void dealCard() = 0;

protected:
	int m_gameType; // 1 : 도둑잡기, 2 : 홀덤
	int m_gamePrice;
	vector <string> m_cardVector;

};


class Holdem : public GameManager
{
public:
	Holdem() { m_gameType = 2; m_gamePrice = 5000; m_bettingPoint = 0; m_gameCnt = 0;  m_activePlayerNum = 0; m_sumBettingPoint = 0; }
	string selectWinner() override;
	void dealCard() override;
	void play(User &user);
	int checkFinalWinner();

private:
	int m_bettingPoint;
	bool betting(int turnNum);
	int m_gameCnt;
	int m_activePlayerNum;
	int m_sumBettingPoint;
	vector <vector <string>> m_userRankResultCardVector;
	vector <string> m_userRankResultVector;
	vector <string> m_fieldCard;
	vector <vector <string>> m_totalUserCard;
	vector <string> m_totalResult;
};

class OldMaid : public GameManager
{

public:

	void dealCard() override;
	// 중복 제거 함수
	void removeDuplicates(vector<string>& playerCards);
	void disCard();
	// 게임 시작 전 순서를 정해주는 함수
	void selectRoutine();
	void printSelectRoutine();
	// 상대방 카드를 얻는 함수 pickCard()위함
	vector<string>& getPlayerCards(string& player);
	void pickCard();
	string determineWinner();
	string selectWinner() override;
	int getGameRound();
	void showWinner();
	// 개수 출력 벡터 만들기(int)
	void clearScreen();

	// 지울 예정
	int getzeroCnt();
	vector<string>& getPlayer1Cards() { return player1Cards; }
	vector<string>& getPlayer2Cards() { return player2Cards; }
	vector<string>& getPlayer3Cards() { return player3Cards; }


	// 카드 셔플 후 플레이어 1,2,3 각각 해당 받을 카드 벡터 선언
	vector<string> player1Cards;
	vector<string> player2Cards;
	vector<string> player3Cards;


private:

	int m_gameRound = 1;
	int m_zeroCnt = 0;
	vector<string> players = { "player1", "player2" , "player3" };
	vector<string> playerOrder;

};