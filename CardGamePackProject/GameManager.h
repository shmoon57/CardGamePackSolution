#pragma once

// time 함수 사용
#include <iostream>
#include <ctime>
#include <cstdlib>
// random_suffle 함수 사용
#include <algorithm>
#include <vector>
#include <string>
<<<<<<< HEAD
#include "Card.h"
=======

#include "CardDeck.h"
>>>>>>> 8c6d4269cc11034bd71b4b2c76a3ecd9c11702c3

//#include "Card.h"

using namespace std;

#ifndef __GameManager__
#define __GameManager__


class GameManager
{
public:
	GameManager() {}
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

<<<<<<< HEAD
=======
class Holdem : public GameManager
{
public:
	Holdem() { m_gameType = 2; m_gamePrice = 0; m_gameCnt = 0;  m_activePlayerNum = 0; m_sumBettingPoint = 0; }
	string selectWinner() override;
	void dealCard() override;
	void play();

private:
	int m_gameCnt;
	int m_activePlayerNum;
	int m_sumBettingPoint;
	vector <string> m_fieldCard;
	vector <vector <string>> m_totalUserCard;
};
>>>>>>> 8c6d4269cc11034bd71b4b2c76a3ecd9c11702c3

class OldMaid : public GameManager
{

public:
<<<<<<< HEAD

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

	// 카드 셔플 후 플레이어 1,2,3 각각 해당 받을 카드 벡터 선언
	vector<string> player1Cards;
	vector<string> player2Cards;
	vector<string> player3Cards;
=======
	void dealCard() override;
>>>>>>> 8c6d4269cc11034bd71b4b2c76a3ecd9c11702c3

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
	// 카드 셔플 후 플레이어 1,2,3 각각 해당 받을 카드 벡터 선언
	vector<string> player1Cards;
	vector<string> player2Cards;
	vector<string> player3Cards;
private:
	int m_gameRound = 1;
	int m_zeroCnt = 0;
	vector<string> players = { "나", "user1" , "user2" };
	vector<string> playerOrder;
<<<<<<< HEAD

};

#endif
=======
};
>>>>>>> 8c6d4269cc11034bd71b4b2c76a3ecd9c11702c3
