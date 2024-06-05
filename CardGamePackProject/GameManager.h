#pragma once
#include <vector>
#include <string>

#include "Card.h"

using namespace std;

class GameManager
{
public:
	GameManager() : m_gameType(0), m_gamePrice(0) {}
	int getGamePrice();
	void setGamePrice(int price);
	
	// °¡»ó ÇÔ¼ö ¼±¾ð
	virtual string selectWinner() = 0;
	virtual void dealCard() = 0;

protected:
	int m_gameType; // 1 : µµµÏÀâ±â, 2 : È¦´ý
	int m_gamePrice;
	vector <string> m_cardVector;

};

class Holdem : public GameManager
{
public:
	Holdem() : m_gameCnt(0), m_activePlayerNum(0), m_sumBettingPoint(0) {}
	string selectWinner() override;
	void dealCard() override;

private:
	int m_gameCnt;
	int m_activePlayerNum;
	int m_sumBettingPoint;
	vector <string> m_totalUserCard;
};

class OldMaid : public GameManager
{
public:
	OldMaid() : m_zeroCnt(0) {}
	string selectWinner() override;
	void dealCard() override;

private:
	int m_zeroCnt;
};