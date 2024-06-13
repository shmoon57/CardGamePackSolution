#include <random>
#include <numeric>
#include <map>
#include <set>
#include <sstream>

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

    // user 인스턴스 기반으로 닉네임 가져오기
    // 닉네임 포함한 string 벡터의 우승자 idx 접근하기
    
    cout << std::to_string(winnerIdx) + "번째 플레이어 우승" << endl;
    return "0";
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

void Holdem::betting(int turnNum)
{
	int bettingOption;
	cout << endl << "배팅 선택" << endl;
	cout << "0 : Call | 1 : Raise | 2 : Fold -> ";
	cin >> bettingOption;
}

void Holdem::play()
{
	int turnNum = 0;
	HoldemDesign holdemDesign;
    // 유저 본인 카드 받기
	holdemDesign.printMyCard(m_totalUserCard[0]);
	while (true)
	{
		// n번째턴 : 0/5 -> 3/5 -> 4/5 -> 5/5
		holdemDesign.printCommunityCard(m_fieldCard, turnNum);
		// 배팅 : 프리, 퍼스트, 세컨드, 라스트
		betting(turnNum);

        // Fold 하지 않은 유저만 winner로 보여주기
		if (turnNum == 3)
		{
            selectWinner();
			break;
		}

		turnNum++;
	}
}

// OldMaid 클래스 함수 정의
// 도둑잡기 게임의 카드 배분 로직 구현
void OldMaid::dealCard()
{
	CardDeck card(1);

	card.suffleCards();
	// 여기 랜덤 추가 해야할듯 14,14,15
	// 1. 플레이어에게 카드 분배 : 셔플된 카드 덱에서 각 플레이어가 14장, 14장, 15장 분배
	const int cardsPerPlayer[] = { 14,14,15 };
	// 2. 카드 분배 결과 저장 : 각 플레이어가 받은 카드를 별도의 벡터나 다른 적절한 데이터 구조에 저장
	int cardIndex = 0;
	for (int card = 0; card < cardsPerPlayer[0]; card++)
	{
		player1Cards.push_back(m_cardVector[cardIndex]);
		cardIndex++;
	}
	for (int card = 0; card < cardsPerPlayer[0]; card++)
	{
		player2Cards.push_back(m_cardVector[cardIndex]);
		cardIndex++;
	}
	for (int card = 0; card < cardsPerPlayer[0]; card++)
	{
		player3Cards.push_back(m_cardVector[cardIndex]);
		cardIndex++;
	}
}
void OldMaid::removeDuplicates(std::vector<std::string>& playerCards)
{
	// 중복된 요소를 찾아 제거하는 과정
	for (int i = 0; i < playerCards.size(); i++)
	{
		for (int j = i + 1; j < playerCards.size();)
		{
			// 현재 요소와 다음 요소를 비교하여 중복 여부 확인
			if (playerCards[i] == playerCards[j])
			{
				// 중복된 경우 다음 요소를 현재 요소의 위치에 덮어씌움
				playerCards.erase(playerCards.begin() + j);
			}
			else
			{
				// 중복되지 않은 경우 다음 요소로 이동
				j++;
			}
		}
	}
	// 현재 플레이어의 카드가 비어 있는지 확인하고, 비어 있다면 m_zeroCnt 증가
	if (playerCards.empty())
	{
		m_zeroCnt++;
	}
}
void OldMaid::disCard()
{
	removeDuplicates(player1Cards);
	removeDuplicates(player2Cards);
	removeDuplicates(player3Cards);
}
void OldMaid::selectRoutine()
{
	cout << "제비뽑기로 순서를 정합니다!\n";
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
	}
}
vector<string>& OldMaid::getPlayerCards(string& player)
{
	if (player == "나")
	{
		return player1Cards;
	}
	if (player == "user1")
	{
		return player2Cards;
	}
	else
	{
		return player3Cards;
	}
}
// 1턴 기준 상대방 카드를 임의로 뽑기
void OldMaid::pickCard()
{
	// 플레이어들이 순서대로 카드를 뽑기 시작합니다.
	for (int i = 0; i < playerOrder.size(); i++)
	{
		// 현재 순서의 플레이어
		string& currentPlayer = playerOrder[i];
		// 다음 순서의 플레이어
		string& nextPlayer = playerOrder[(i + 1) % playerOrder.size()];
		cout << currentPlayer << "의 차례입니다.\n";
		// 다음 순서의 플레이어 카드 목록을 참조로 가져옵니다.
		vector<string>& nextPlayerCards = getPlayerCards(nextPlayer);
		// 다음 순서의 플레이어가 더 이상 카드를 가지고 있지 않은 경우
		if (nextPlayerCards.empty())
		{
			cout << nextPlayer << "는 더 이상 카드를 가지고 있지 않습니다.\n";
			continue;
		}
		// 랜덤 선택된 카드 가져오기
		int cardIndex = rand() % nextPlayerCards.size();
		string pickedCard = nextPlayerCards[cardIndex];
		// 현재 순서의 플레이어 카드 목록을 참조로 가져옵니다.
		vector<string>& currentPlayerCards = getPlayerCards(currentPlayer);
		// 현재 플레이어의 카드 목록에 선택된 카드를 추가합니다.
		currentPlayerCards.push_back(pickedCard);
		// 다음 플레이어의 카드 목록에서 선택된 카드를 제거합니다.
		nextPlayerCards.erase(nextPlayerCards.begin() + cardIndex);
		cout << currentPlayer << "가 " << nextPlayer << "의 카드를 뽑았습니다: " << pickedCard << "\n";
		// 중복된 카드를 제거
		removeDuplicates(currentPlayerCards);
		// pickCard() 기준으로 게임라운드 변수 증가
		m_gameRound++;
	}
}
string OldMaid::determineWinner() {
	// 우승자를 저장할 변수 초기화
	string winner = "";
	// 플레이어들의 카드를 확인하여 남은 카드가 없는 플레이어를 찾음
	for (string& player : playerOrder)
	{
		vector<string>& playerCards = getPlayerCards(player);
		if (playerCards.empty()) {
			winner = player;
			break; // 남은 카드가 없는 플레이어를 찾았으므로 루프 종료
		}
	}
	// 우승자가 없는 경우 "Game is still ongoing." 반환
	if (winner.empty()) {
		return "Game is still ongoing.";
	}
	// 우승자가 있는 경우 해당 플레이어를 우승자로 선정하여 반환
	return winner + " is the Old Maid!";
}
string OldMaid::selectWinner()
{
	// 만약 m_zeroCnt가 2인 경우 (즉, 카드가 없는 플레이어가 2명 이상인 경우)
	if (m_zeroCnt == 2)
	{
		// 승자를 결정하고 반환
		return determineWinner();
	}
	// m_zeroCnt가 2가 아닌 경우, pickCard() 함수 호출
	pickCard();
	// 게임이 아직 종료되지 않았으므로 빈 문자열 반환
	return "";
}
int OldMaid::getGameRound()
{
	return m_gameRound;
}