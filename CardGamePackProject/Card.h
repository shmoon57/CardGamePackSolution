#pragma once
#include <vector>
#include <string>

using namespace std;
//test
class Card
{
public:
	Card(int type) // type 1: µµµÏÀâ±â, 2: È¦´ý
	{
		m_cardVector = makeDeck(type);
	}

	void suffleCards();

private:
	vector<string> m_cardVector;
	vector<string> makeDeck(int type);
};