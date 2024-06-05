#include <random>
#include <numeric>

#include "GameManager.h"
#include "Card.h"

#define USERNUM 3

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
    //카드 덱 생성
    Card card(2); //홀덤 option == 2

    //카드 섞기 및 카드 벡터 가져오기
    card.suffleCards();
    vector<string> cardVector = card.getCardVector();
    
    ////랜덤하게 필드카드 5장 가져오고 기존 cardVector에서 값 빼기
    //랜덤 엔진 초기화
    random_device rd;
    mt19937 g(rd());

    //벡터 인덱스
    vector<int> indices(cardVector.size());
    iota(indices.begin(), indices.end(), 0);

    //인덱스 무작위 섞기
    shuffle(indices.begin(), indices.end(), g);

    //무작위 5개의 인덱스 벡터 생성
    vector<int> selected_indices(indices.begin(), indices.begin() + 5);

    //필드에 있을 5개 카드 벡터 생성
    vector<string> fieldCard;
    for (int index : selected_indices) {
        fieldCard.push_back(cardVector[index]);
    }

    //뽑은 5개의 카드를 기존의 cardVector에서 제거
    sort(selected_indices.rbegin(), selected_indices.rend());
    for (int index : selected_indices) {
        cardVector.erase(cardVector.begin() + index);
    }

    ////나머지 cardVector에서 랜덤하게 2장씩 가져와 usersCard에 넣기
    vector<vector <string>> usersCard(USERNUM);

    for (int i = 0; i < USERNUM; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            usersCard[i].push_back(cardVector.back());
            cardVector.pop_back();
        }
    }
}

// OldMaid 클래스 함수 정의
string OldMaid::selectWinner() {
    // 도둑잡기 게임의 승자 선택 로직 구현
    return "OldMaid Winner";
}

void OldMaid::dealCard() {
    // 도둑잡기 게임의 카드 배분 로직 구현
}