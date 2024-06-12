#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;

class Design
{

public:
	void cardDesign(string card, int x, int y);
	void gotoxy(int x, int y);
	void drawHorizontalLine(int x, int y, int length);
	void drawVerticalLine(int x, int y, int length);
	void drawSquare(int x, int y, int width, int height);
	void printMyCard(vector<string> myCard);
	void printMyInfo(vector<string> myInfo);
};

class HoldemDesign :public Design
{


public:
	void printCommunityCard(vector<string> myCard, int turnNum);

};