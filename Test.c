#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>	
int map[30][50], ind = 0;
int ways[500][2];	
int findMap[30][50];
void findPath(int sizeX, int sizeY, int xPos, int yPos){
	int ra1 = 0;
	while(ra1 < 100){
		for(int i = 1; i < sizeX - 1; i++){
			for(int k = 1; k < sizeY - 1; k++){
				if(findMap[k][i] != - 1){
					if(findMap[k - 1][i] == -1 &&
						map[k - 1][i] != 2)
							findMap[k - 1][i] = findMap[k][i] + 1;
						if(findMap[k + 1][i] == -1 && 
						map[k + 1][i] != 2)
							findMap[k + 1][i] = findMap[k][i] + 1;	
						if(findMap[k][i - 1] == -1 &&
						map[k][i - 1] != 2)
							findMap[k][i - 1] = findMap[k][i] + 1;	
						if(findMap[k][i + 1] == -1 &&
						map[k][i + 1] != 2)
							findMap[k][i + 1] = findMap[k][i] + 1;
					}
				}
		} 
		ra1++;
	}	
	ra1 = 0;
	int x1,y1,nx1,ny1;
	x1 = 15;
	y1 = 15;
	for(int i = 0; i < sizeX; i++){
			for(int k = 0; k < sizeY; k++){
				if(findMap[k][i] == - 1){
					findMap[k][i] = 99;
				}
			}
		}	
	int min = 99;
	while(y1 != yPos || x1 != xPos){
		if(y1 + 1 < sizeY){
			min = findMap[y1 + 1][x1];
			nx1 = x1;
			ny1 = y1 + 1;
		}
		if(x1 + 1 < sizeX){
			if(findMap[y1][x1 + 1] < min){
				min = findMap[y1][x1+1];
				nx1 = x1+1;
				ny1 = y1;
			}
		}
		if(x1 > 0){
			if(findMap[y1][x1 - 1] < min){
				min = findMap[y1][x1 - 1];
				nx1 = x1 - 1;
				ny1 = y1;
			}
		}
		if(y1 > 0){
			if(findMap[y1 - 1][x1] < min){
				min = findMap[y1 - 1][x1];
				nx1 = x1;
				ny1 = y1 - 1;
			}
		}
		x1 = nx1;
		y1 = ny1;
		if(ind < 200) {
			ways[ind][0] = ny1;
			ways[ind][1] = nx1;
		}
		else{ 
			ind = 0;
			break;
		}
		ind++;
	}
}
void drawMap(int sizeX, int sizeY){
	for(int i = 0; i < sizeX; i++){
			for(int k = 0; k < sizeY; k++){
				if(map[k][i] == 2)
					mvprintw(k,i,"=");
				if(map[k][i] == 0)
					mvprintw(k,i," ");
				findMap[k][i] = -1;	
			}
		}
		for(int i = 0; i < ind - 1; i++){
			//if(map[ways[i][0]][ways[i][1]] != 2)
				mvprintw(ways[i][0],ways[i][1],"*");
		}
		ind = 0;

}
void init(int sizeX, int sizeY){
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){
			map[k][i] = 2;
			findMap[k][i] = -1;
		}
	}
}
void genLab(int xPos, int yPos, int sizeX, int sizeY){
	int originX,originY;
	int rand4;
	originX = rand() % (sizeX - 3) + 1;
	originY = rand() % (sizeY - 3) + 1;
	map[originY][originX] = 0;
	for(int i = 0; i < 2000; i++){
		rand4 = rand() % 4;
		int counter = 0;
		if(map[originY][originX + 1] == 0)
			counter++;
		if(map[originY][originX - 1] == 0)
			counter++;
		if(map[originY + 1][originX] == 0)
			counter++;
		if(map[originY - 1][originX] == 0)
			counter++;	
		if(map[originY + 1][originX + 1] == 0)
			counter++;
		if(map[originY - 1][originX - 1] == 0)
			counter++;
		if(map[originY + 1][originX - 1] == 0)
			counter++;
		if(map[originY - 1][originX + 1] == 0)
			counter++;
		if(map[originY][originX] == 2 && counter < 5)
			map[originY][originX] = 0;
		else{
			originX = rand() % (sizeX - 3) + 1;
			originY = rand() % (sizeY - 3) + 1;
		}
		switch(rand4){
			case 0:	
				if(originY - 1 > 0)
					originY--;
				break;
			case 1:
				if(originY + 2 < sizeY)
					originY++;
				break;
			case 2:
				if(originX - 1 > 0)
					originX--;
				break;
			case 3:
				if(originX + 2 < sizeX)
					originX++;
				break;	
		}
	}

}
int main(int argc, char*argv[]){
	initscr();
	int sizeX = 50;
	int sizeY = 30;
	init(sizeX, sizeY);
	keypad(stdscr,true);
	srand(time(NULL));
	int xPos = 25;
	int yPos = 25;
	genLab(xPos, yPos, sizeX, sizeY);
	map[yPos][xPos] = 0;	
	findMap[yPos][xPos] = 0;
	noecho();
	while(true){	
		findPath(sizeX, sizeY, xPos, yPos);	
		findMap[yPos][xPos] = 0;
		drawMap(sizeX, sizeY);	
		mvprintw(yPos, xPos, "@");
		int ch = getch();
		switch(ch){
			case KEY_UP:
				if(yPos > 1)
					yPos--;
				break;
			case KEY_DOWN:
				if(yPos < sizeY - 2)
					yPos++;
				break;
			case KEY_LEFT:
				if(xPos > 1)
					xPos--;
				break;
			case KEY_RIGHT:
				if(xPos < sizeX - 2)
					xPos++;
				break;
		}
		findMap[yPos][xPos] = 0;
		refresh();
	}
		getch();
		endwin();
		return 0;
}
