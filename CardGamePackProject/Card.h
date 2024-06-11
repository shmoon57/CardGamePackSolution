#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;
class CardDeck
{
public:
	CardDeck(int type) // type 1: µµµÏÀâ±â, 2: È¦´ý
	{
		makeDeck(type);
	}
	void suffleCards();
	vector<string> getCardVector();


private:
	vector<string> m_cardVector;
	void makeDeck(int type);
};