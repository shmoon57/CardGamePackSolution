#include <random>
#include <numeric>
#include <map>
#include <set>
#include <sstream>

// remove() 함수 사용
#include <algorithm>

#include "GameManager.h"
#include "Design.h"

#define USERNUM 3




struct Card {
    char suit;  // 's', 'd', 'h', 'c'
    int value;  // 2-10, 11 (J), 12 (Q), 13 (K), 14 (A)

    Card(char s, int v) : suit(s), value(v) {}
};
std::map<char, int> value_map = {
    {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'1', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}
};

std::vector<Card> parseCards(const std::vector<std::string>& card_strs) {
    std::vector<Card> cards;
    for (const auto& card_str : card_strs) {
        char suit = card_str[0];  // suit는 card_str[1]에 있음
        int value = value_map[card_str[1]];  // value는 card_str[0]에 있음
        cards.emplace_back(suit, value);
    }
    return cards;
}

// 정렬된 unique_values 벡터가 스트레이트인지 확인하는 함수
bool isStraight(const std::vector<int>& unique_values, std::vector<int>& straight_cards) {
    straight_cards.clear(); // 스트레이트 카드를 저장하기 위해 초기화

    // unique_values를 오름차순으로 정렬
    std::vector<int> sorted_values = unique_values;
    std::sort(sorted_values.begin(), sorted_values.end());

    int consecutive_count = 1; // 연속된 카드의 개수를 세는 변수
    for (size_t i = 1; i < sorted_values.size(); ++i) {
        // 현재 값과 이전 값이 1 차이가 나면 연속된 값이므로 consecutive_count 증가
        if (sorted_values[i] - sorted_values[i - 1] == 1) {
            consecutive_count++;
        }
        else {
            consecutive_count = 1; // 연속이 끊겼으므로 다시 1로 초기화
        }

        // 연속된 카드가 5장이 되면 스트레이트이므로 해당 범위를 straight_cards에 저장
        if (consecutive_count == 5) {
            for (int j = i - 4; j <= i; ++j) {
                straight_cards.push_back(sorted_values[j]);
            }
            return true; // 스트레이트가 확인되었으므로 true 반환
        }
    }

    return false; // 스트레이트가 없으므로 false 반환
}

bool compareByValue(const Card& a, const Card& b) {
    return a.value > b.value;
}

pair<string, vector<Card>> determineHand(const std::vector<Card>& all_cards) {
    // Sort cards by value
    std::vector<Card> cards = all_cards;
    std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
        return a.value < b.value;
        });

    // Helper structures
    std::map<int, int> value_count;
    std::map<char, int> suit_count;
    std::map<char, std::vector<int>> suit_values;

    for (const auto& card : cards) {
        value_count[card.value]++;
        suit_count[card.suit]++;
        suit_values[card.suit].push_back(card.value);
    }

    // Check for straight flush and royal flush
    for (const auto& suit_values_pair : suit_values) {
        char suit = suit_values_pair.first;
        const std::vector<int>& values = suit_values_pair.second;
        if (values.size() >= 5) {
            std::vector<int> sorted_values = values;
            std::sort(sorted_values.begin(), sorted_values.end());
            std::vector<int> straight_cards;
            if (isStraight(sorted_values, straight_cards)) {
                std::vector<Card> straight_flush_cards;
                for (const auto& card : cards) {
                    if (std::find(straight_cards.begin(), straight_cards.end(), card.value) != straight_cards.end() && card.suit == suit) {
                        straight_flush_cards.push_back(card);
                    }
                }
                if (straight_cards.back() == 14 && straight_cards[straight_cards.size() - 5] == 10) {
                    return { "Royal Flush", straight_flush_cards };
                }
                return { "Straight Flush", straight_flush_cards };
            }
        }
    }
    
    // Check for four of a kind
    for (const auto& value_count_pair : value_count) {
        int value = value_count_pair.first;
        int count = value_count_pair.second;
        if (count == 4) {
            std::vector<Card> four_of_a_kind_cards;
            for (const auto& card : cards) {
                if (card.value == value) {
                    four_of_a_kind_cards.push_back(card);
                }
            }
            return { "Four of a Kind", four_of_a_kind_cards };
        }
    }
    
    // Check for full house
    int three_value = 0, pair_value = 0;
    for (const auto& value_count_pair : value_count) {
        int value = value_count_pair.first;
        int count = value_count_pair.second;
        if (count == 3) {
            three_value = value;
        }
        else if (count == 2 && value > pair_value) {
            pair_value = value;
        }
    }
    if (three_value && pair_value) {
        std::vector<Card> full_house_cards;
        for (const auto& card : cards) {
            if (card.value == three_value || card.value == pair_value) {
                full_house_cards.push_back(card);
            }
        }
        return { "Full House", full_house_cards };
    }
    
    // Check for flush
    for (const auto& suit_count_pair : suit_count) {
        char suit = suit_count_pair.first;
        int count = suit_count_pair.second;
        if (count >= 5) {
            std::vector<Card> flush_cards;
            for (const auto& card : cards) {
                if (card.suit == suit) {
                    flush_cards.push_back(card);
                }
            }
            return { "Flush", std::vector<Card>(flush_cards.begin(), flush_cards.begin() + 5) };
        }
    }
    
    // Check for straight
    std::vector<int> unique_values;
    for (const auto& card : cards) {
        if (unique_values.empty() || unique_values.back() != card.value) {
            unique_values.push_back(card.value);
        }
    }
    std::vector<int> straight_cards;
    if (isStraight(unique_values, straight_cards)) {
        std::vector<Card> straight_hand_cards;
        for (const auto& card : cards) {
            if (std::find(straight_cards.begin(), straight_cards.end(), card.value) != straight_cards.end()) {
                straight_hand_cards.push_back(card);
            }
        }
        return { "Straight", straight_hand_cards };
    }
    
    // Check for three of a kind
    for (const auto& value_count_pair : value_count) {
        int value = value_count_pair.first;
        int count = value_count_pair.second;
        if (count == 3) {
            std::vector<Card> three_of_a_kind_cards;
            for (const auto& card : cards) {
                if (card.value == value) {
                    three_of_a_kind_cards.push_back(card);
                }
            }
            return { "Three of a Kind", three_of_a_kind_cards };
        }
    }

    // Check for two pair and one pair
    std::vector<int> pairs;
    for (const auto& value_count_pair : value_count) {
        int value = value_count_pair.first;
        int count = value_count_pair.second;
        if (count == 2) {
            pairs.push_back(value);
        }
    }

    if (pairs.size() >= 2) {
        std::vector<Card> two_pair_cards;
        for (const auto& card : cards) {
            if (card.value == pairs[0] || card.value == pairs[1]) {
                two_pair_cards.push_back(card);
            }
        }
        return { "Two Pair", two_pair_cards };
    }

    if (pairs.size() == 1) {
        std::vector<Card> one_pair_cards;
        for (const auto& card : cards) {
            if (card.value == pairs[0]) {
                one_pair_cards.push_back(card);
            }
        }
        return { "One Pair", one_pair_cards };
    }

    // High Card
    vector<Card> sortedCards = cards;
    std::sort(sortedCards.begin(), sortedCards.end(), compareByValue);
    vector <Card> highestCard = { sortedCards[0] };
    return { "High Card",  highestCard };
}

std::string handToString(const std::pair<std::string, std::vector<Card>>& hand) {
    std::stringstream ss;
    ss << hand.first << ": ";
    for (const auto& card : hand.second) {
        std::string value_str;
        switch (card.value) {
        case 11: value_str = "J"; break;
        case 12: value_str = "Q"; break;
        case 13: value_str = "K"; break;
        case 14: value_str = "A"; break;
        default: value_str = std::to_string(card.value); break;
        }
        ss << card.suit << value_str << " ";
    }
    return ss.str();
}

int Holdem::checkFinalWinner()
{
    std::map<string, int> rankResult = { { "High Card", 1 }, {"One Pair", 2}, {"Two Pair", 3},
    {"Three of a Kind", 4}, {"Straight", 5}, {"Flush", 6}, {"Full House", 7}, {"Four of a Kind", 8}, {"Straight Flush", 9}, {"Royal Flush", 10} };

    // 합산 결과를 만드는 과정
    for (string userResult : m_totalResult)
    {
        std::vector<std::string> tokens1;
        std::string token1;
        std::istringstream tokenStream1(userResult);
        while (std::getline(tokenStream1, token1, ':')) {
            tokens1.push_back(token1);
        }
        m_userRankResultVector.push_back(tokens1[0]);

        // 공백기준으로 랭크 기반의 카드 벡터 만들기 ex) { "cA", "dA", "c8", "d8", "s8" }
        std::vector<std::string> tokens2;
        std::string token2;
        std::istringstream tokenStream2(tokens1[1]); 

        while (std::getline(tokenStream2, token2, ' ')) {
            if (!token2.empty()) {  // 빈 문자열이 아닌 경우에만 추가
                tokens2.push_back(token2);
            }
        }
        m_userRankResultCardVector.push_back(tokens2);
    }

    // tokens1[0] 은 패의 랭크가 무엇인지? ex) 원페어, 투페어, 트리플
    // tokens1[1] 은 해당 랭크의 카드 구성이 무엇인지? ex) "s9 d9", "d10 s10 d8 c8", "d10 s10 c10"

    std::map<char, int> rankCardBase = { {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'1', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14} };

    vector <int> userRankVector;
    for (int i = 0; i < USERNUM; i++)
    {
        userRankVector.push_back(rankResult[m_userRankResultVector[i]]);
    }

    int userRankMax = 0;
    int userMaxIdx = 0;
    for (int i = 0; i < USERNUM; i++)
    {
        if (userRankVector[i] > userRankMax)
        {
            userRankMax = userRankVector[i];
            userMaxIdx = i;
        }

        // 랭크가 같은 경우 숫자가 더 큰 사람이 우승
        else if (userRankVector[i] == userRankMax)
        {
            //High Card : 1개 값 비교
            //One Pair : 1개 값 비교
            //Two Pair : 2개 값 비교
            //Triple : 1개 값 비교
            //Straight : 1개 값 비교
            //Flush : 1개 값 비교
            //Full House : 2개 값 비교
            //Four Card : 1개 값 비교
            //Straight Flush : 1개 값 비교
            //Royal Straight Flush : 무승부
            vector <int> copyVector1;
            vector <int> copyVector2;
            int maxValueLast;
            int maxValueC;
            if (userRankVector[i] == 1 or userRankVector[i] == 2 or userRankVector[i] == 4 or userRankVector[i] == 5 or userRankVector[i] == 6 or userRankVector[i] == 8 or userRankVector[i] == 9)
            {
                for (string element1 : m_userRankResultCardVector[userMaxIdx])
                {
                    // 이전 까지의 최고 랭크 벡터의 가장 큰 숫자 값
                    copyVector1.push_back(rankCardBase[element1[1]]);
                    auto maxValue1 = max_element(copyVector1.begin(), copyVector1.end());
                    maxValueLast = *maxValue1;
                }

                for (string element2 : m_userRankResultCardVector[i])
                {
                    // 확인할 최고 랭크 벡터의 가장 큰 숫자 값
                    copyVector2.push_back(rankCardBase[element2[1]]);
                    auto maxValue2 = max_element(copyVector2.begin(), copyVector2.end());
                    maxValueC = *maxValue2;
                }

                if (maxValueLast < maxValueC)
                {
                    userRankMax = userRankVector[i];
                    userMaxIdx = i;
                }
            }

            else if (userRankVector[i] == 10)
            {
                //무승부
                return -1;
            }

            else
            {
                //2개 값 비교
                for (string element1 : m_userRankResultCardVector[userMaxIdx])
                {
                    // 이전 까지의 최고 랭크 벡터의 가장 큰 숫자 값
                    copyVector1.push_back(rankCardBase[element1[1]]);
                    auto maxValue1 = max_element(copyVector1.begin(), copyVector1.end());
                    maxValueLast = *maxValue1;
                }

                for (string element2 : m_userRankResultCardVector[i])
                {
                    // 확인할 최고 랭크 벡터의 가장 큰 숫자 값
                    copyVector2.push_back(rankCardBase[element2[1]]);
                    auto maxValue2 = max_element(copyVector2.begin(), copyVector2.end());
                    maxValueC = *maxValue2;
                }

                if (maxValueLast < maxValueC)
                {
                    userRankMax = userRankVector[i];
                    userMaxIdx = i;
                }

                else if (maxValueLast == maxValueC)
                {
                    int maxValueLast;
                    int maxValueC;
                    copyVector1.erase(std::remove(copyVector1.begin(), copyVector1.end(), maxValueLast), copyVector1.end());
                    copyVector2.erase(std::remove(copyVector2.begin(), copyVector2.end(), maxValueLast), copyVector2.end());
                    for (string element1 : m_userRankResultCardVector[userMaxIdx])
                    {
                        // 이전 까지의 최고 랭크 벡터의 가장 큰 숫자 값
                        copyVector1.push_back(rankCardBase[element1[1]]);
                        auto maxValue1 = max_element(copyVector1.begin(), copyVector1.end());
                        maxValueLast = *maxValue1;
                    }

                    for (string element2 : m_userRankResultCardVector[i])
                    {
                        // 확인할 최고 랭크 벡터의 가장 큰 숫자 값
                        copyVector2.push_back(rankCardBase[element2[1]]);
                        auto maxValue2 = max_element(copyVector2.begin(), copyVector2.end());
                        maxValueC = *maxValue2;
                    }

                    if (maxValueLast < maxValueC)
                    {
                        userRankMax = userRankVector[i];
                        userMaxIdx = i;
                    }

                    else if (maxValueLast == maxValueC)
                    {
                        return -1;
                    }
                }
            }
        }
    }
    return userMaxIdx;
}

int GameManager::getGamePrice() { return m_gamePrice; }

void GameManager::setGamePrice(int price) { m_gamePrice = price; }

// Holdem 클래스 함수 정의
string Holdem::selectWinner() {
    // 홀덤 게임의 승자 선택 로직 구현
    for (int i = 0; i < USERNUM; i++)
    {
        for (string fieldCard : m_fieldCard)
        {
            m_totalUserCard[i].push_back(fieldCard);
        }
    }

    for (int j = 0; j < USERNUM; j++)
    {
        //vector <string> testV = { "s2", "dA", "d10", "d9", "d6", "dK", "c2" };
        //vector<Card> cards = parseCards(testV);
        vector<Card> cards = parseCards(m_totalUserCard[j]);
        pair<string, vector<Card>> result = determineHand(cards);
        string strResult = handToString(result);
        m_totalResult.push_back(strResult);
    }

    int winnerIdx = checkFinalWinner();

    if (winnerIdx == -1)
    {
        return "0";
    }
    // user 인스턴스 기반으로 닉네임 가져오기
    // 닉네임 포함한 string 벡터의 우승자 idx 접근하기

    HoldemDesign holdemDesign;
    holdemDesign.gotoxy(1, 32);
    cout << std::to_string(winnerIdx) + "번째 플레이어 우승" << endl;
    return std::to_string(winnerIdx);
}

void Holdem::dealCard()
{
    // 홀덤 게임의 카드 배분 로직 구현
    //카드 덱 생성
    CardDeck card(2); //홀덤 option == 2

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

    m_fieldCard = fieldCard;
    m_totalUserCard = usersCard;
}

bool Holdem::betting(int turnNum)
{
    int bettingOption;
    cout << endl << "배팅 선택" << endl;
    cout << "0 : Call | 1 : Raise | 2 : Fold -> ";
    cin >> bettingOption;

    vector <int> bettingResultVector;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 10);
    if (dis(gen) < 6)
    {
        bettingResultVector.push_back(0);
    }

    else
    {
        bettingResultVector.push_back(1);
    }

    if (bettingOption == 0)
    {
        bettingResultVector.push_back(0);
    }

    else if (bettingOption == 1)
    {
        bettingResultVector.push_back(1);
    }

    else if (bettingOption == 2)
    {
        bettingResultVector.push_back(2);
    }

    for (int bettingResult : bettingResultVector)
    {
        if (bettingResult == 0)
        {
            m_sumBettingPoint += m_gamePrice;
        }

        else if (bettingResult == 1)
        {
            m_gamePrice = m_gamePrice * 2;
            m_sumBettingPoint += m_gamePrice;
        }

        else if (bettingResult == 2)
        {
            cout << "Fold 하셨습니다. 모두의 패를 공개합니다.";
            return true;
        }
    }
    return false;
}

void Holdem::play(User &user)
{
    Design design;
    vector <string> myInfo;
    myInfo.push_back(user.getNickname());
    myInfo.push_back(user.getGamePoint());
    design.printMyInfo(myInfo);

    int turnNum = 0;
    HoldemDesign holdemDesign;
    // 유저 본인 카드 받기
    holdemDesign.printMyCard(m_totalUserCard[0]);
    bool isFold = false;
    while (true)
    {
        // n번째턴 : 0/5 -> 3/5 -> 4/5 -> 5/5
        holdemDesign.printCommunityCard(m_fieldCard, turnNum);
        // 배팅 : 프리, 퍼스트, 세컨드, 라스트
        isFold = betting(turnNum);

        //Fold 한 경우
        if (isFold)
        {
            system("cls");
            holdemDesign.showHoldemResult(user.getNickname(), m_userRankResultVector, m_userRankResultCardVector);
            break;
        }

        //포인트 셋 하기
        int point = stoi(user.getGamePoint()) - m_gamePrice;
        m_bettingPoint += point;
        user.setGamePoint(to_string(point));
        myInfo.pop_back();
        myInfo.push_back(user.getGamePoint());
        design.printMyInfo(myInfo);

        // Fold 하지 않은 유저만 winner로 보여주기
        if (turnNum == 3)
        {
            string winnerIdx = selectWinner();
            //무승부인 경우
            if (winnerIdx == "-1")
            {
                system("cls");
                holdemDesign.showHoldemResult(user.getNickname(), m_userRankResultVector, m_userRankResultCardVector);
                cout << "무승부입니다! 베팅한 포인트를 돌려드립니다." << endl;
                user.setGamePoint(to_string(m_bettingPoint));
                myInfo.pop_back();
                myInfo.push_back(user.getGamePoint());
                design.printMyInfo(myInfo);
                break;
            }
            
            if (winnerIdx == "0")
            {
                system("cls");
                holdemDesign.showHoldemResult(user.getNickname(), m_userRankResultVector, m_userRankResultCardVector);
                cout << "축하합니다! " << m_sumBettingPoint << " 만큼의 포인트를 얻었습니다." << endl;
                user.setGamePoint(to_string(stoi(user.getGamePoint()) + m_sumBettingPoint));
                myInfo.pop_back();
                myInfo.push_back(user.getGamePoint());
                design.printMyInfo(myInfo);
                break;
            }

            system("cls");
            holdemDesign.showHoldemResult(user.getNickname(), m_userRankResultVector, m_userRankResultCardVector);
            myInfo.pop_back();
            myInfo.push_back(user.getGamePoint());
            design.printMyInfo(myInfo);
            break;
        }

        turnNum++;
    }
}

// OldMaid 클래스 함수 정의
// 도둑잡기 게임의 카드 배분 로직 구현 
void OldMaid::dealCard()
{
    CardDeck OldMaidDeck(1);
    OldMaidDeck.suffleCards();

    // 여기 랜덤 추가 해야할듯 17,18,18
    // 1. 플레이어에게 카드 분배 : 셔플된 카드 덱에서 각 플레이어가 17장, 18장, 18장 분배
    int cardsPerPlayer[] = { 17,18,18 };
    m_cardVector = OldMaidDeck.getCardVector();

    // 2. 카드 분배 결과 저장 : 각 플레이어가 받은 카드를 별도의 벡터나 다른 적절한 데이터 구조에 저장
    int cardIndex = 0;

    for (int card = 0; card < cardsPerPlayer[0]; card++)
    {
        player1Cards.push_back(m_cardVector[cardIndex]);
        cardIndex++;
    }

    for (int card = 0; card < cardsPerPlayer[1]; card++)
    {
        player2Cards.push_back(m_cardVector[cardIndex]);
        cardIndex++;
    }

    for (int card = 0; card < cardsPerPlayer[2]; card++)
    {
        player3Cards.push_back(m_cardVector[cardIndex]);
        cardIndex++;
    }

    printDeckNum = { (int)player1Cards.size(), (int)player2Cards.size(), (int)player3Cards.size() };

}

// 플레이어 별 카드 상황 표현
// 지울 예정
void OldMaid::printPlayerDeck()
{
	  cout << "Player 1 Cards: ";
	  for (const string& card : getPlayer1Cards()) {
		cout << card << " ";
	  }
	  cout << endl;

	  cout << "Player 2 Cards: ";
	  for (const string& card : getPlayer2Cards()) {
		cout << card << " ";
	  }
	  cout << endl;

	  cout << "Player 3 Cards: ";
	  for (const string& card : getPlayer3Cards()) {
		  cout << card << " ";
	  }
	  cout << endl << endl;
}

void OldMaid::removeDuplicates(vector<string>& playerCards)
{
    for (int i = 0; i < playerCards.size(); i++)
    {
        for (int j = i + 1; j < playerCards.size(); j++)
        {
            // 숫자 중복 되는 경우
            if (playerCards[i].substr(1) == playerCards[j].substr(1))
            {
                playerCards[i] = "00";
                playerCards[j] = "00";
                break;
            }
        }
    }
     
    // remove() : 비어 있는 요소들을 벡터의 뒤쪽으로 이동시키고, 새로운 끝 반복자를 반환
    playerCards.erase(remove(playerCards.begin(), playerCards.end(), "00"), playerCards.end());
}

void OldMaid::disCard()
{
    removeDuplicates(player1Cards);
    removeDuplicates(player2Cards);
    removeDuplicates(player3Cards);

    printDeckNum = { (int)player1Cards.size(), (int)player2Cards.size(), (int)player3Cards.size() };
}

void OldMaid::selectRoutine()
{
    // 현재 시간을 시드로 사용하여 무작위성을 증가
    srand(time(0));

    // players 벡터를 playerOrder 벡터에 복사
    playerOrder = players;

    // playerOrder 벡터를 무작위로 섞기
    random_shuffle(playerOrder.begin(), playerOrder.end());
}

void OldMaid::printSelectRoutine()
{
    // 결과 출력
    cout << "플레이어 순서:\n";
    for (int i = 0; i < playerOrder.size(); ++i)
    {
        cout << (i + 1) << "번째 : " << playerOrder[i] << "\n";
    };
}

vector<string>& OldMaid::getPlayerCards(const string& player)
{
    if (player == players[0])
    {
        return player1Cards;
    }

    if (player == players[1])
    {
        return player2Cards;
    }

    else
    {
        return player3Cards;
    }
}

void OldMaid::showZeroCnt()
{
    cout << endl << "------------showWinner-----------" << endl;
    cout << "Player1 card size : " << this->player1Cards.size() << endl;
    cout << "Player2 card size : " << this->player2Cards.size() << endl;
    cout << "Player3 card size : " << this->player3Cards.size() << endl << endl;

}

// RandomPickOrder : main 문 에서 i
/*
1. pickNum++ -> 라운드 표현
2 - 1)카드가 있으면 self/auto pick 결정
2 - 2)카드 없다는 메시지 출력
3. 플레이어 카드 수 확인하며 카드 수 0이면 ++
*/

void OldMaid::pickCard(int RandomPickOrderIdx)
{
  m_pickNum++;

  // playerOrder = {player3, player1, player2}
  string currentPlayerName = playerOrder[RandomPickOrderIdx].substr(6, 1);
  int sizeofPlayerCard = -1;

  if (currentPlayerName == "1")
  {
    sizeofPlayerCard = player1Cards.size();
  }
  else if (currentPlayerName == "2")
  {
    sizeofPlayerCard = player2Cards.size();
  }
  else if (currentPlayerName == "3")
  {
    sizeofPlayerCard = player3Cards.size();
  }

  if (sizeofPlayerCard == 0)
  {
    cout << "player의 카드 개수는 0 입니다. 다음 player가 카드를 뽑습니다.\n";
  }

  else
  {
    if (playerOrder[RandomPickOrderIdx] == players[0])
    {
      selfPickCard(RandomPickOrderIdx, playerOrder.size());
    }
    else
    {
      autoPickCard(RandomPickOrderIdx, playerOrder.size(), playerOrder[RandomPickOrderIdx]);
    }

  }

  // 한라운드씩 실행되므로 초기화 필요
  m_zeroCnt = 0;

  if (player1Cards.size() == 0)
  {
    m_zeroCnt++;
  }
  if (player2Cards.size() == 0)
  {
    m_zeroCnt++;
  }
  if (player3Cards.size() == 0)
  {
    m_zeroCnt++;
  }

}
//selfPickCard(RandomPickOrder, playerOrder.size())
void OldMaid::selfPickCard(int RandomPickOrderIdx, int playerVectorSize)
{
  // player1이 카드를 뽑을 타겟 인덱스
  int targetIdx;
  string targetPlayer;

  // targetIdx 결정 loop
  while (true)
  {
    // 타겟 인덱스 정하기 : 플레이어 3명인 경우
    if (playerVectorSize == 3)
    {
      // 내가 마지막 순서이면
      if (RandomPickOrderIdx == 2)
      {
        targetIdx = 0;
      }

      // 내가 마지막 순서가 아니면
      else
      {
        targetIdx = RandomPickOrderIdx + 1;
      }
    }

    // 타겟 인덱스 정하기 : 플레이어 2명인 경우
    else if (playerVectorSize == 2)
    {

      // 기존에 두번째 순서였던 경우
      if (RandomPickOrderIdx == 1)
      {
        if (playerOrder[targetIdx] != "player1")
        {
          // autoPick 가야함; playerOrder[RandomPickOrder])
          autoPickCard(RandomPickOrderIdx, playerVectorSize, playerOrder[RandomPickOrderIdx]);

        }
        targetIdx = 0;
      }

      else
      {
        if (playerOrder[targetIdx] != "player1")
        {
          // selfPick 가야함; playerOrder[RandomPickOrder])
          autoPickCard(RandomPickOrderIdx, playerVectorSize, playerOrder[RandomPickOrderIdx]);

        }

        // 기존에 첫번째 순서거나 마지막 순서였던 경우
        //targetIdx = RandomPickOrder + 1;
        targetIdx = 1;
      }

    }

    // autoPickCard 인 경우 : 1 3 2 순서 player1이 selfPick때 카드수가 없어서 못 뽑으면  Player3이 뽑아야 하는데  selfPick으로 뽑아버림
    if ((playerVectorSize == 2 && RandomPickOrderIdx == 1 && (playerOrder[targetIdx] != "player1"))
      || (playerVectorSize == 2 && RandomPickOrderIdx == 1 && (playerOrder[targetIdx] != "player1")))

    {
      return;
    }


    targetPlayer = playerOrder[targetIdx];

    // 내가 뽑을 타겟 카드 수가 없는 경우
    if (getPlayerCards(playerOrder[targetIdx]).size() == 0)
    {
      cout << targetPlayer << "의 카드가 없습니다. 다른 플레이어를 선택합니다.\n";
      // 픞레이 순서를 담은 벡터에서 요소를 제거
      playerOrder.erase(playerOrder.begin() + targetIdx);
      continue;
    }

    break;
  }

  // 타겟 카드 정의
  vector <string>& targetCards = getPlayerCards(playerOrder[targetIdx]);

  if (!targetCards.empty())
  {
    bool correctInput = false;
    // 입력 예외 처리
    while (!correctInput)
    {
      int cardNumber;
      cout << "몇 번째 카드를 선택하겠습니까? ( 1부터 " << targetCards.size() << "까지 ) : ";
      cin >> cardNumber;

      int cardIndex = cardNumber - 1;

      if (cardIndex < 0 || cardIndex >= targetCards.size())
      {
        cout << "입력이 잘못되었습니다. 다시 시도해주세요.\n\n";

      }

      else
      {
        targetPlayer = playerOrder[targetIdx];
        string pickedCard = targetCards[cardIndex];

        // getPlayerCards 매개변수 const 수정
        getPlayerCards("player1").push_back(pickedCard);

        targetCards.erase(targetCards.begin() + cardIndex);

        cout << "player1님이 " << targetPlayer << "의 " << pickedCard << " 카드를 뽑았습니다.\n\n";

        correctInput = true;
        break;

      }

    }
  }

  // 중복된 카드 제거 및 빈 요소 처리
  //removeDuplicates(getPlayer1Cards());
  disCard();

  //cout << "뽑은 후 결과 \n";
 // printPlayerDeck();

  //showZeroCnt();

}

//autoPickCard(RandomPickOrder, playerOrder.size(), playerOrder[RandomPickOrder])
void OldMaid::autoPickCard(int RandomPickOrderIdx, int playerVectorSize, string& currentPlayer)
{
  // Player 2,3이 카드를 뽑을 타겟 인덱스
  int targetIdx;
  string targetPlayer;

  // targetIdx 결정 loop
  while (true)
  {

    // 타겟 인덱스 정하기 : 플레이어 3명인 경우
    if (playerVectorSize == 3)
    {
      // 내가 마지막 순서이면
      if (RandomPickOrderIdx == 2)
      {
        targetIdx = 0;
      }

      // 내가 마지막 순서가 아니면
      else
      {
        targetIdx = RandomPickOrderIdx + 1;
      }
    }

    // 타겟 인덱스 정하기 : 플레이어 2명인 경우
    else if (playerVectorSize == 2)
    {

      if (RandomPickOrderIdx == 1)
      {
        if (playerOrder[targetIdx] == "player1")
        {
          // selfPick 가야함;
          selfPickCard(RandomPickOrderIdx, playerVectorSize);
          break;
        }
        targetIdx = 0;
      }

      else
      {
        if (playerOrder[targetIdx] == "player1")
        {
          // selfPick 가야함
          selfPickCard(RandomPickOrderIdx, playerVectorSize);
          break;
        }
        // targetIdx = RandomPickOrder + 1;
        targetIdx = 1;
      }

    }

    // selfPickCard 인 경우 : 3 1 2 순서 player3이 autoPick때 카드수가 없어서 못 뽑을떄  Player1이 뽑아야 하는데  autoPick으로 뽑아버림
    if ((playerVectorSize == 2 && RandomPickOrderIdx == 1 && (playerOrder[targetIdx] == "player1"))
      || (playerVectorSize == 2 && RandomPickOrderIdx == 1 && (playerOrder[targetIdx] == "player1")))

    {
      return;
    }

    targetPlayer = playerOrder[targetIdx];

    if (getPlayerCards(playerOrder[targetIdx]).size() == 0)
    {
      cout << targetPlayer << "의 카드가 없습니다. 다른 플레이어를 선택합니다.\n";
      // 여기 부터
      playerOrder.erase(playerOrder.begin() + targetIdx);
      continue;
    }

    break;
  }

  // 타겟 카드 정의
  vector <string>& targetCards = getPlayerCards(playerOrder[targetIdx]);

  // 임의의 카드 선택
  // CardNum 범위 :  1 ~ targetCards 크기
  int CardNum = rand() % targetCards.size() + 1;
  int cardIndex = CardNum - 1;

  targetPlayer = playerOrder[targetIdx];
  string pickedCard = targetCards[cardIndex];

  // currentPlayer가 선택한 카드를 추가하고, targetCards에서 제거
  getPlayerCards(currentPlayer).push_back(pickedCard);

  targetCards.erase(targetCards.begin() + cardIndex);

  // 여기 사이 오류 존재
  cout << currentPlayer << "님이 " << targetPlayer << "의 " << pickedCard << " 카드를 뽑았습니다.\n\n";

  // 중복된 카드 제거
  disCard();

  //cout << "뽑은 후 결과 \n";
  //printPlayerDeck();

  //showZeroCnt();
}

bool OldMaid::isWin()
{
  if (getZeroCnt() >= 2)
  {
    return true;
  }
  else
  {
    return false;
  }
}



string OldMaid::selectWinner()
{
  vector<pair<int, string>> playerRankings;

  if (player1Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player1"));
  }
  if (player2Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player2"));
  }
  if (player3Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player3"));
  }

  // 게임이 종료된 후 남아있는 플레이어들 중 가장 많은 카드를 가진 플레이어를 마지막 순위로 매기기
  if (!player1Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player1"));
  }
  if (!player2Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player2"));
  }
  if (!player3Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player3"));
  }

  // 순위를 출력
  string result = "게임 종료! 순위는 다음과 같습니다:\n";
  for (int i = 0; i < playerRankings.size(); ++i) {
    result += to_string(i + 1) + "등: " + playerRankings[i].second + "\n";
  }
  return result;
}

void OldMaid::play(User& user)
{
  Design design;
  //vector<string> myInfo;
  //myInfo.push_back(user.getNickname());
  //myInfo.push_back(user.getGamePoint());
  //design.printMyInfo(myInfo);
  players.push_back(user.getNickname());
  players.push_back("player2");
  players.push_back("player3");

  OldMaidDesign oldMaidDesign;
  
  cout << "도둑잡기\n";
  cout << "게임 시작! \n\n";

  // 2 - 1) 카드 분배
  cout << "(1) 카드를 분배합니다.\n";
  dealCard();

  // 3초 딜레이
  this_thread::sleep_for(chrono::seconds(3));
  printJkInMyDeck();
  oldMaidDesign.printCardNum(user.getNickname(), printDeckNum);
  oldMaidDesign.oldMaidPrintMyCard(player1Cards);
  //cout << "카드 분배 결과 \n";
  //printPlayerDeck();

  for (int i = 0; i < player1Cards.size(); i++)
  {
    if (player1Cards[i] == "joker")
    {
      oldMaidDesign.printJoker();
    }
  }

  // 2 - 2) 중복 카드를 버리기
  cout << "(2) 중복된 숫자카드를 버립니다.\n";
  cout << " - 카드를 버려주세요.\n\n";
  disCard();

  this_thread::sleep_for(chrono::seconds(2));

  // 여기 화면에 초기화 안됨 -> How to?
  //oldMaidDesign.printCardNum(user.getNickname(), printDeckNum);
  oldMaidDesign.oldMaidPrintMyCard(player1Cards);

  this_thread::sleep_for(chrono::seconds(2));

  //cout << "중복된 카드를 버린 결과는? \n";
  // printPlayerDeck();

  // 2 - 3) 카드 뽑을 순서 정하기
 
  while (true) {

    system("cls");
    cout << "도둑잡기\n\n";
    cout << (getPickNum() / 3) + 1 << "라운드입니다\n\n";

    cout << "(1) 카드 뽑을 순서를 정합니다.\n";
    this_thread::sleep_for(chrono::seconds(1));
    for (int i = 0; i < player1Cards.size(); i++)
    {
      if (player1Cards[i] == "joker")
      {
        oldMaidDesign.printJoker();
      }
    }
    oldMaidDesign.printCardNum(user.getNickname(), printDeckNum);
    oldMaidDesign.oldMaidPrintMyCard(player1Cards);

    selectRoutine();
    oldMaidDesign.printSelectRoutine(playerOrder);
    cout << " - 순서가 정해졌습니다! \n\n";

    for (int i = 0; i < 3; i++)
    {
      // 우승자 선정
      if (isWin())
      {
        cout << "우승자가 선정 되었습니다\n";
        break;
      }
      // Player의 인덱스가 들어감
      cout << "(" << i + 2 << ") " << i+1 << "번째 플레이어 차례입니다\n";
      pickCard(i);
      this_thread::sleep_for(chrono::seconds(3));
    }

    if (getZeroCnt() == 2)
    {
      break;
    }
  } 

  // 3. 결과 공개 
  system("cls");
  
  cout << "도둑잡기\n\n";
  cout << "결과를 공개합니다\n\n";
  this_thread::sleep_for(chrono::seconds(3));

  // 게임 종료 후 순위를 출력
  cout << selectWinner();

  cout << "\n방을 나가는 중입니다....\n";
  this_thread::sleep_for(chrono::seconds(5));

} 

int OldMaid::getPickNum()
{
    return m_pickNum;
}

int OldMaid::getZeroCnt()
{
    return m_zeroCnt;
}

void OldMaid::printJkInMyDeck()
{
  bool jokerFound = false;

  for (int i = 0; i < player1Cards.size(); i++)
  {
    if (player1Cards[i] == "joker")
    {

      jokerFound = true;
      break;
    }
  }
    
    if (jokerFound)
    {
      cout << " - 조커입니다! 평정심을 유지해주세요!\n\n";
    }

    else
    {
      cout << " - 현재 카드에 조커는 없습니다.\n\n";
    }

}

