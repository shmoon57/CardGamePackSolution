#include "GameManager.h"

int GameManager::getGamePrice() { return m_gamePrice; }

void GameManager::setGamePrice(int price) { m_gamePrice = price; }

// Holdem 클래스 함수 정의
string Holdem::selectWinner() {
    // 홀덤 게임의 승자 선택 로직 구현
    return "Holdem Winner";
}

void Holdem::dealCard()
{
    // 홀덤 게임의 카드 배분 로직 구현

}

void Holdem::shuffleCard() {
    // 홀덤 게임의 카드 섞기 로직 구현
}

// OldMaid 클래스 함수 정의
string OldMaid::selectWinner() {
    // 도둑잡기 게임의 승자 선택 로직 구현
    return "OldMaid Winner";
}

void OldMaid::dealCard() {
    // 도둑잡기 게임의 카드 배분 로직 구현
}

void OldMaid::shuffleCard() {
    // 도둑잡기 게임의 카드 섞기 로직 구현
}