
#include "Card.h"
#include "User.h"
#include "GameManager.h"


int main()
{
	User user1;
	User user2;
	User user3;

	int gameOption;

	cout << "CARD GAME" << endl;
	cout << "1, 2, 3" << endl;
	cin >> gameOption;

	cout << user1.getGamePoint();

	Holdem holdem;
	holdem.dealCard();


}