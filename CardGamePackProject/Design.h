#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
#include"GameManager.h"
using namespace std;
class Design
{
public:
	void gotoxy(int x, int y);
	void printMyCard(vector<string>& myCard);
	void printMyInfo(vector<string>& myInfo);
protected:
	void cardDesign(string& card, int x, int y);
	void setConsoleTextColor(WORD color);
	void drawHorizontalLine(int x, int y, int length);
	void drawVerticalLine(int x, int y, int length);
	void drawSquare(int x, int y, int width, int height);
};
class HoldemDesign : public Design
{
public:
	void printCommunityCard(vector<string>& myCard, int turnNum);
	void showHoldemResult(string Nickname, vector<string>& Rank, vector<vector<string>>& finalHand);
};
class OldMaidDesign : public Design
{
public:
	void printJoker();
	void printCardNum(string nickname, vector<int>& cardNum);
	void oldMaidPrintMyCard(vector<string>& myCard);
	void printSelectRoutine(vector<string>& playerTurnInfo);
	void getGamePointInfo();
private:
	vector<string> getJ();
	vector<string> getO();
	vector<string> getK();
	vector<string> getE();
	vector<string> getR();
};