#include <ctime>

#include "Card.h"

void Card::suffleCards()
{
	// 랜덤 시드 초기화
	std::srand(std::time(0));

	// 피셔-예이츠 셔플 알고리즘
	for (size_t i = m_cardVector.size() - 1; i > 0; --i) {
		size_t j = std::rand() % (i + 1);
		std::swap(m_cardVector[i], m_cardVector[j]);
	}
}

void Card::showMyCard()
{
	for (string card : m_cardVector)
	{
		cout << card << " ";
	}
	cout << endl;
}

vector <string> Card::makeDeck(int type)
{
	// "sA", "s2", "s3", "s4", ... , "sQ", "sK"
	// 카드의 숫자
	vector<string> ranks = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
	// 카드의 슈트
	vector<string> suits = { "s", "d", "h", "c" }; // 스페이드, 다이아, 하트, 클로버

	switch (type)
	{
	case 1: // 도둑잡기
	{
		// 카드 덱 생성
		for (const auto& suit : suits) {
			for (const auto& rank : ranks) {
				m_cardVector.push_back(suit + rank);
			}
		}
		m_cardVector.push_back("JK");
		break;
	}

	case 2: //홀덤
	{
		// 카드 덱 생성
		for (const auto& suit : suits) {
			for (const auto& rank : ranks) {
				m_cardVector.push_back(suit + rank);
			}
		}
		break;
	}
	default:
		break;
	}
}