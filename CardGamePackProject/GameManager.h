#pragma once
// time �Լ� ���
#include <iostream>
#include <ctime>
#include <cstdlib>
// random_suffle , remove �Լ� ���
#include <algorithm>
#include <vector>
#include <string>

#include "User.h"
#include "CardDeck.h"

#include <thread> // for Delay

using namespace std;

class GameManager
{
public:
	GameManager() { m_gameType = 0; m_gamePrice = 0; }
	int getGamePrice();
	void setGamePrice(int price);

	// ���� �Լ� ����
	virtual string selectWinner() = 0;
	virtual void dealCard() = 0;

protected:
	int m_gameType; // 1 : �������, 2 : Ȧ��
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

	void play(User& user);
	void dealCard() override;
	// �ߺ� ���� �Լ�
	void removeDuplicates(vector<string>& playerCards);
	void disCard();
	// ���� ���� �� ������ �����ִ� �Լ�
	void selectRoutine();
	void printSelectRoutine();
	// ���� ī�带 ��� �Լ� pickCard()����
	vector<string>& getPlayerCards(const string& player);
	void pickCard(int RandomPickOrder);
	void autoPickCard(int RandomPickOrder, int playerVectorSize, string& currentPlayer);
	void selfPickCard(int RandomPickOrder, int playerVectorSize);
	string selectWinner() override;
	int getPickNum();
	void showZeroCnt();
	void printPlayerDeck();
	int getZeroCnt();
	bool isWin();

	vector <int> printDeckNum;
	vector<string>& getPlayer1Cards() { return player1Cards; }
	vector<string>& getPlayer2Cards() { return player2Cards; }
	vector<string>& getPlayer3Cards() { return player3Cards; }

	// ī�� ���� �� �÷��̾� 1,2,3 ���� �ش� ���� ī�� ���� ����
	vector<string> player1Cards;
	vector<string> player2Cards;
	vector<string> player3Cards;

	void printJkInMyDeck();

private:
	int m_pickNum = 1;
	int m_zeroCnt = 0;
	vector<string> players;
	//= { "player1", "player2" , "player3" };
	vector<string> playerOrder;
	vector<string> finishedPlayers;

};