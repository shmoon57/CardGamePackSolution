#pragma once

// time 함수 사용
#include <iostream>
#include <ctime>
#include <cstdlib>
// random_suffle 함수 사용
#include <algorithm>
#include <vector>
#include <string>
#include "Card.h"

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

	// 카드 셔플 후 플레이어 1,2,3 각각 해당 받을 카드 벡터 선언
	vector<string> player1Cards;
	vector<string> player2Cards;
	vector<string> player3Cards;

private:
	int m_gameRound = 1;
	int m_zeroCnt = 0;
	vector<string> players = { "나", "user1" , "user2" };
	vector<string> playerOrder;

};

#endif