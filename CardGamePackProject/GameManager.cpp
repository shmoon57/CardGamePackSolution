#include <random>
#include <numeric>
#include <map>
#include <set>
#include <sstream>

// remove() �Լ� ���
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
        char suit = card_str[0];  // suit�� card_str[1]�� ����
        int value = value_map[card_str[1]];  // value�� card_str[0]�� ����
        cards.emplace_back(suit, value);
    }
    return cards;
}

// ���ĵ� unique_values ���Ͱ� ��Ʈ����Ʈ���� Ȯ���ϴ� �Լ�
bool isStraight(const std::vector<int>& unique_values, std::vector<int>& straight_cards) {
    straight_cards.clear(); // ��Ʈ����Ʈ ī�带 �����ϱ� ���� �ʱ�ȭ

    // unique_values�� ������������ ����
    std::vector<int> sorted_values = unique_values;
    std::sort(sorted_values.begin(), sorted_values.end());

    int consecutive_count = 1; // ���ӵ� ī���� ������ ���� ����
    for (size_t i = 1; i < sorted_values.size(); ++i) {
        // ���� ���� ���� ���� 1 ���̰� ���� ���ӵ� ���̹Ƿ� consecutive_count ����
        if (sorted_values[i] - sorted_values[i - 1] == 1) {
            consecutive_count++;
        }
        else {
            consecutive_count = 1; // ������ �������Ƿ� �ٽ� 1�� �ʱ�ȭ
        }

        // ���ӵ� ī�尡 5���� �Ǹ� ��Ʈ����Ʈ�̹Ƿ� �ش� ������ straight_cards�� ����
        if (consecutive_count == 5) {
            for (int j = i - 4; j <= i; ++j) {
                straight_cards.push_back(sorted_values[j]);
            }
            return true; // ��Ʈ����Ʈ�� Ȯ�εǾ����Ƿ� true ��ȯ
        }
    }

    return false; // ��Ʈ����Ʈ�� �����Ƿ� false ��ȯ
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

    // �ջ� ����� ����� ����
    for (string userResult : m_totalResult)
    {
        std::vector<std::string> tokens1;
        std::string token1;
        std::istringstream tokenStream1(userResult);
        while (std::getline(tokenStream1, token1, ':')) {
            tokens1.push_back(token1);
        }
        m_userRankResultVector.push_back(tokens1[0]);

        // ����������� ��ũ ����� ī�� ���� ����� ex) { "cA", "dA", "c8", "d8", "s8" }
        std::vector<std::string> tokens2;
        std::string token2;
        std::istringstream tokenStream2(tokens1[1]); 

        while (std::getline(tokenStream2, token2, ' ')) {
            if (!token2.empty()) {  // �� ���ڿ��� �ƴ� ��쿡�� �߰�
                tokens2.push_back(token2);
            }
        }
        m_userRankResultCardVector.push_back(tokens2);
    }

    // tokens1[0] �� ���� ��ũ�� ��������? ex) �����, �����, Ʈ����
    // tokens1[1] �� �ش� ��ũ�� ī�� ������ ��������? ex) "s9 d9", "d10 s10 d8 c8", "d10 s10 c10"

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

        // ��ũ�� ���� ��� ���ڰ� �� ū ����� ���
        else if (userRankVector[i] == userRankMax)
        {
            //High Card : 1�� �� ��
            //One Pair : 1�� �� ��
            //Two Pair : 2�� �� ��
            //Triple : 1�� �� ��
            //Straight : 1�� �� ��
            //Flush : 1�� �� ��
            //Full House : 2�� �� ��
            //Four Card : 1�� �� ��
            //Straight Flush : 1�� �� ��
            //Royal Straight Flush : ���º�
            vector <int> copyVector1;
            vector <int> copyVector2;
            int maxValueLast;
            int maxValueC;
            if (userRankVector[i] == 1 or userRankVector[i] == 2 or userRankVector[i] == 4 or userRankVector[i] == 5 or userRankVector[i] == 6 or userRankVector[i] == 8 or userRankVector[i] == 9)
            {
                for (string element1 : m_userRankResultCardVector[userMaxIdx])
                {
                    // ���� ������ �ְ� ��ũ ������ ���� ū ���� ��
                    copyVector1.push_back(rankCardBase[element1[1]]);
                    auto maxValue1 = max_element(copyVector1.begin(), copyVector1.end());
                    maxValueLast = *maxValue1;
                }

                for (string element2 : m_userRankResultCardVector[i])
                {
                    // Ȯ���� �ְ� ��ũ ������ ���� ū ���� ��
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
                //���º�
                return -1;
            }

            else
            {
                //2�� �� ��
                for (string element1 : m_userRankResultCardVector[userMaxIdx])
                {
                    // ���� ������ �ְ� ��ũ ������ ���� ū ���� ��
                    copyVector1.push_back(rankCardBase[element1[1]]);
                    auto maxValue1 = max_element(copyVector1.begin(), copyVector1.end());
                    maxValueLast = *maxValue1;
                }

                for (string element2 : m_userRankResultCardVector[i])
                {
                    // Ȯ���� �ְ� ��ũ ������ ���� ū ���� ��
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
                        // ���� ������ �ְ� ��ũ ������ ���� ū ���� ��
                        copyVector1.push_back(rankCardBase[element1[1]]);
                        auto maxValue1 = max_element(copyVector1.begin(), copyVector1.end());
                        maxValueLast = *maxValue1;
                    }

                    for (string element2 : m_userRankResultCardVector[i])
                    {
                        // Ȯ���� �ְ� ��ũ ������ ���� ū ���� ��
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

// Holdem Ŭ���� �Լ� ����
string Holdem::selectWinner() {
    // Ȧ�� ������ ���� ���� ���� ����
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
    // user �ν��Ͻ� ������� �г��� ��������
    // �г��� ������ string ������ ����� idx �����ϱ�

    HoldemDesign holdemDesign;
    holdemDesign.gotoxy(1, 32);
    cout << std::to_string(winnerIdx) + "��° �÷��̾� ���" << endl;
    return std::to_string(winnerIdx);
}

void Holdem::dealCard()
{
    // Ȧ�� ������ ī�� ��� ���� ����
    //ī�� �� ����
    CardDeck card(2); //Ȧ�� option == 2

    //ī�� ���� �� ī�� ���� ��������
    card.suffleCards();
    vector<string> cardVector = card.getCardVector();

    ////�����ϰ� �ʵ�ī�� 5�� �������� ���� cardVector���� �� ����
    //���� ���� �ʱ�ȭ
    random_device rd;
    mt19937 g(rd());

    //���� �ε���
    vector<int> indices(cardVector.size());
    iota(indices.begin(), indices.end(), 0);

    //�ε��� ������ ����
    shuffle(indices.begin(), indices.end(), g);

    //������ 5���� �ε��� ���� ����
    vector<int> selected_indices(indices.begin(), indices.begin() + 5);

    //�ʵ忡 ���� 5�� ī�� ���� ����
    vector<string> fieldCard;
    for (int index : selected_indices) {
        fieldCard.push_back(cardVector[index]);
    }

    //���� 5���� ī�带 ������ cardVector���� ����
    sort(selected_indices.rbegin(), selected_indices.rend());
    for (int index : selected_indices) {
        cardVector.erase(cardVector.begin() + index);
    }

    ////������ cardVector���� �����ϰ� 2�徿 ������ usersCard�� �ֱ�
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
    cout << endl << "���� ����" << endl;
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
            cout << "Fold �ϼ̽��ϴ�. ����� �и� �����մϴ�.";
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
    // ���� ���� ī�� �ޱ�
    holdemDesign.printMyCard(m_totalUserCard[0]);
    bool isFold = false;
    while (true)
    {
        // n��°�� : 0/5 -> 3/5 -> 4/5 -> 5/5
        holdemDesign.printCommunityCard(m_fieldCard, turnNum);
        // ���� : ����, �۽�Ʈ, ������, ��Ʈ
        isFold = betting(turnNum);

        //Fold �� ���
        if (isFold)
        {
            system("cls");
            holdemDesign.showHoldemResult(user.getNickname(), m_userRankResultVector, m_userRankResultCardVector);
            break;
        }

        //����Ʈ �� �ϱ�
        int point = stoi(user.getGamePoint()) - m_gamePrice;
        m_bettingPoint += point;
        user.setGamePoint(to_string(point));
        myInfo.pop_back();
        myInfo.push_back(user.getGamePoint());
        design.printMyInfo(myInfo);

        // Fold ���� ���� ������ winner�� �����ֱ�
        if (turnNum == 3)
        {
            string winnerIdx = selectWinner();
            //���º��� ���
            if (winnerIdx == "-1")
            {
                system("cls");
                holdemDesign.showHoldemResult(user.getNickname(), m_userRankResultVector, m_userRankResultCardVector);
                cout << "���º��Դϴ�! ������ ����Ʈ�� �����帳�ϴ�." << endl;
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
                cout << "�����մϴ�! " << m_sumBettingPoint << " ��ŭ�� ����Ʈ�� ������ϴ�." << endl;
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

// OldMaid Ŭ���� �Լ� ����
// ������� ������ ī�� ��� ���� ���� 
void OldMaid::dealCard()
{
    CardDeck OldMaidDeck(1);
    OldMaidDeck.suffleCards();

    // ���� ���� �߰� �ؾ��ҵ� 17,18,18
    // 1. �÷��̾�� ī�� �й� : ���õ� ī�� ������ �� �÷��̾ 17��, 18��, 18�� �й�
    int cardsPerPlayer[] = { 17,18,18 };
    m_cardVector = OldMaidDeck.getCardVector();

    // 2. ī�� �й� ��� ���� : �� �÷��̾ ���� ī�带 ������ ���ͳ� �ٸ� ������ ������ ������ ����
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

// �÷��̾� �� ī�� ��Ȳ ǥ��
// ���� ����
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
            // ���� �ߺ� �Ǵ� ���
            if (playerCards[i].substr(1) == playerCards[j].substr(1))
            {
                playerCards[i] = "00";
                playerCards[j] = "00";
                break;
            }
        }
    }
     
    // remove() : ��� �ִ� ��ҵ��� ������ �������� �̵���Ű��, ���ο� �� �ݺ��ڸ� ��ȯ
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
    // ���� �ð��� �õ�� ����Ͽ� ���������� ����
    srand(time(0));

    // players ���͸� playerOrder ���Ϳ� ����
    playerOrder = players;

    // playerOrder ���͸� �������� ����
    random_shuffle(playerOrder.begin(), playerOrder.end());
}

void OldMaid::printSelectRoutine()
{
    // ��� ���
    cout << "�÷��̾� ����:\n";
    for (int i = 0; i < playerOrder.size(); ++i)
    {
        cout << (i + 1) << "��° : " << playerOrder[i] << "\n";
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

// RandomPickOrder : main �� ���� i
/*
1. pickNum++ -> ���� ǥ��
2 - 1)ī�尡 ������ self/auto pick ����
2 - 2)ī�� ���ٴ� �޽��� ���
3. �÷��̾� ī�� �� Ȯ���ϸ� ī�� �� 0�̸� ++
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
    cout << "player�� ī�� ������ 0 �Դϴ�. ���� player�� ī�带 �̽��ϴ�.\n";
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

  // �Ѷ��徿 ����ǹǷ� �ʱ�ȭ �ʿ�
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
  // player1�� ī�带 ���� Ÿ�� �ε���
  int targetIdx;
  string targetPlayer;

  // targetIdx ���� loop
  while (true)
  {
    // Ÿ�� �ε��� ���ϱ� : �÷��̾� 3���� ���
    if (playerVectorSize == 3)
    {
      // ���� ������ �����̸�
      if (RandomPickOrderIdx == 2)
      {
        targetIdx = 0;
      }

      // ���� ������ ������ �ƴϸ�
      else
      {
        targetIdx = RandomPickOrderIdx + 1;
      }
    }

    // Ÿ�� �ε��� ���ϱ� : �÷��̾� 2���� ���
    else if (playerVectorSize == 2)
    {

      // ������ �ι�° �������� ���
      if (RandomPickOrderIdx == 1)
      {
        if (playerOrder[targetIdx] != "player1")
        {
          // autoPick ������; playerOrder[RandomPickOrder])
          autoPickCard(RandomPickOrderIdx, playerVectorSize, playerOrder[RandomPickOrderIdx]);

        }
        targetIdx = 0;
      }

      else
      {
        if (playerOrder[targetIdx] != "player1")
        {
          // selfPick ������; playerOrder[RandomPickOrder])
          autoPickCard(RandomPickOrderIdx, playerVectorSize, playerOrder[RandomPickOrderIdx]);

        }

        // ������ ù��° �����ų� ������ �������� ���
        //targetIdx = RandomPickOrder + 1;
        targetIdx = 1;
      }

    }

    // autoPickCard �� ��� : 1 3 2 ���� player1�� selfPick�� ī����� ��� �� ������  Player3�� �̾ƾ� �ϴµ�  selfPick���� �̾ƹ���
    if ((playerVectorSize == 2 && RandomPickOrderIdx == 1 && (playerOrder[targetIdx] != "player1"))
      || (playerVectorSize == 2 && RandomPickOrderIdx == 1 && (playerOrder[targetIdx] != "player1")))

    {
      return;
    }


    targetPlayer = playerOrder[targetIdx];

    // ���� ���� Ÿ�� ī�� ���� ���� ���
    if (getPlayerCards(playerOrder[targetIdx]).size() == 0)
    {
      cout << targetPlayer << "�� ī�尡 �����ϴ�. �ٸ� �÷��̾ �����մϴ�.\n";
      // �a���� ������ ���� ���Ϳ��� ��Ҹ� ����
      playerOrder.erase(playerOrder.begin() + targetIdx);
      continue;
    }

    break;
  }

  // Ÿ�� ī�� ����
  vector <string>& targetCards = getPlayerCards(playerOrder[targetIdx]);

  if (!targetCards.empty())
  {
    bool correctInput = false;
    // �Է� ���� ó��
    while (!correctInput)
    {
      int cardNumber;
      cout << "�� ��° ī�带 �����ϰڽ��ϱ�? ( 1���� " << targetCards.size() << "���� ) : ";
      cin >> cardNumber;

      int cardIndex = cardNumber - 1;

      if (cardIndex < 0 || cardIndex >= targetCards.size())
      {
        cout << "�Է��� �߸��Ǿ����ϴ�. �ٽ� �õ����ּ���.\n\n";

      }

      else
      {
        targetPlayer = playerOrder[targetIdx];
        string pickedCard = targetCards[cardIndex];

        // getPlayerCards �Ű����� const ����
        getPlayerCards("player1").push_back(pickedCard);

        targetCards.erase(targetCards.begin() + cardIndex);

        cout << "player1���� " << targetPlayer << "�� " << pickedCard << " ī�带 �̾ҽ��ϴ�.\n\n";

        correctInput = true;
        break;

      }

    }
  }

  // �ߺ��� ī�� ���� �� �� ��� ó��
  //removeDuplicates(getPlayer1Cards());
  disCard();

  //cout << "���� �� ��� \n";
 // printPlayerDeck();

  //showZeroCnt();

}

//autoPickCard(RandomPickOrder, playerOrder.size(), playerOrder[RandomPickOrder])
void OldMaid::autoPickCard(int RandomPickOrderIdx, int playerVectorSize, string& currentPlayer)
{
  // Player 2,3�� ī�带 ���� Ÿ�� �ε���
  int targetIdx;
  string targetPlayer;

  // targetIdx ���� loop
  while (true)
  {

    // Ÿ�� �ε��� ���ϱ� : �÷��̾� 3���� ���
    if (playerVectorSize == 3)
    {
      // ���� ������ �����̸�
      if (RandomPickOrderIdx == 2)
      {
        targetIdx = 0;
      }

      // ���� ������ ������ �ƴϸ�
      else
      {
        targetIdx = RandomPickOrderIdx + 1;
      }
    }

    // Ÿ�� �ε��� ���ϱ� : �÷��̾� 2���� ���
    else if (playerVectorSize == 2)
    {

      if (RandomPickOrderIdx == 1)
      {
        if (playerOrder[targetIdx] == "player1")
        {
          // selfPick ������;
          selfPickCard(RandomPickOrderIdx, playerVectorSize);
          break;
        }
        targetIdx = 0;
      }

      else
      {
        if (playerOrder[targetIdx] == "player1")
        {
          // selfPick ������
          selfPickCard(RandomPickOrderIdx, playerVectorSize);
          break;
        }
        // targetIdx = RandomPickOrder + 1;
        targetIdx = 1;
      }

    }

    // selfPickCard �� ��� : 3 1 2 ���� player3�� autoPick�� ī����� ��� �� ������  Player1�� �̾ƾ� �ϴµ�  autoPick���� �̾ƹ���
    if ((playerVectorSize == 2 && RandomPickOrderIdx == 1 && (playerOrder[targetIdx] == "player1"))
      || (playerVectorSize == 2 && RandomPickOrderIdx == 1 && (playerOrder[targetIdx] == "player1")))

    {
      return;
    }

    targetPlayer = playerOrder[targetIdx];

    if (getPlayerCards(playerOrder[targetIdx]).size() == 0)
    {
      cout << targetPlayer << "�� ī�尡 �����ϴ�. �ٸ� �÷��̾ �����մϴ�.\n";
      // ���� ����
      playerOrder.erase(playerOrder.begin() + targetIdx);
      continue;
    }

    break;
  }

  // Ÿ�� ī�� ����
  vector <string>& targetCards = getPlayerCards(playerOrder[targetIdx]);

  // ������ ī�� ����
  // CardNum ���� :  1 ~ targetCards ũ��
  int CardNum = rand() % targetCards.size() + 1;
  int cardIndex = CardNum - 1;

  targetPlayer = playerOrder[targetIdx];
  string pickedCard = targetCards[cardIndex];

  // currentPlayer�� ������ ī�带 �߰��ϰ�, targetCards���� ����
  getPlayerCards(currentPlayer).push_back(pickedCard);

  targetCards.erase(targetCards.begin() + cardIndex);

  // ���� ���� ���� ����
  cout << currentPlayer << "���� " << targetPlayer << "�� " << pickedCard << " ī�带 �̾ҽ��ϴ�.\n\n";

  // �ߺ��� ī�� ����
  disCard();

  //cout << "���� �� ��� \n";
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

  // ������ ����� �� �����ִ� �÷��̾�� �� ���� ���� ī�带 ���� �÷��̾ ������ ������ �ű��
  if (!player1Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player1"));
  }
  if (!player2Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player2"));
  }
  if (!player3Cards.empty()) {
    playerRankings.push_back(make_pair(m_pickNum, "player3"));
  }

  // ������ ���
  string result = "���� ����! ������ ������ �����ϴ�:\n";
  for (int i = 0; i < playerRankings.size(); ++i) {
    result += to_string(i + 1) + "��: " + playerRankings[i].second + "\n";
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
  
  cout << "�������\n";
  cout << "���� ����! \n\n";

  // 2 - 1) ī�� �й�
  cout << "(1) ī�带 �й��մϴ�.\n";
  dealCard();

  // 3�� ������
  this_thread::sleep_for(chrono::seconds(3));
  printJkInMyDeck();
  oldMaidDesign.printCardNum(user.getNickname(), printDeckNum);
  oldMaidDesign.oldMaidPrintMyCard(player1Cards);
  //cout << "ī�� �й� ��� \n";
  //printPlayerDeck();

  for (int i = 0; i < player1Cards.size(); i++)
  {
    if (player1Cards[i] == "joker")
    {
      oldMaidDesign.printJoker();
    }
  }

  // 2 - 2) �ߺ� ī�带 ������
  cout << "(2) �ߺ��� ����ī�带 �����ϴ�.\n";
  cout << " - ī�带 �����ּ���.\n\n";
  disCard();

  this_thread::sleep_for(chrono::seconds(2));

  // ���� ȭ�鿡 �ʱ�ȭ �ȵ� -> How to?
  //oldMaidDesign.printCardNum(user.getNickname(), printDeckNum);
  oldMaidDesign.oldMaidPrintMyCard(player1Cards);

  this_thread::sleep_for(chrono::seconds(2));

  //cout << "�ߺ��� ī�带 ���� �����? \n";
  // printPlayerDeck();

  // 2 - 3) ī�� ���� ���� ���ϱ�
 
  while (true) {

    system("cls");
    cout << "�������\n\n";
    cout << (getPickNum() / 3) + 1 << "�����Դϴ�\n\n";

    cout << "(1) ī�� ���� ������ ���մϴ�.\n";
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
    cout << " - ������ ���������ϴ�! \n\n";

    for (int i = 0; i < 3; i++)
    {
      // ����� ����
      if (isWin())
      {
        cout << "����ڰ� ���� �Ǿ����ϴ�\n";
        break;
      }
      // Player�� �ε����� ��
      cout << "(" << i + 2 << ") " << i+1 << "��° �÷��̾� �����Դϴ�\n";
      pickCard(i);
      this_thread::sleep_for(chrono::seconds(3));
    }

    if (getZeroCnt() == 2)
    {
      break;
    }
  } 

  // 3. ��� ���� 
  system("cls");
  
  cout << "�������\n\n";
  cout << "����� �����մϴ�\n\n";
  this_thread::sleep_for(chrono::seconds(3));

  // ���� ���� �� ������ ���
  cout << selectWinner();

  cout << "\n���� ������ ���Դϴ�....\n";
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
      cout << " - ��Ŀ�Դϴ�! �������� �������ּ���!\n\n";
    }

    else
    {
      cout << " - ���� ī�忡 ��Ŀ�� �����ϴ�.\n\n";
    }

}

