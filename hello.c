#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define PI 3.14159265358979323846

int main(int argc, char*argv[]){
	initscr();
	start_color();
	char line[50] = "";
	float angle = 0;
	int sizeX = 0;
	int sizeY = 0;
	int xPos = 0, step = 20;
	int viewRadius = 4,index = 0;
	int yPos = 0,cntBul = 0;
	int inventory[25];
	int visible[500][2];
	for(int i = 0; i < 50; i++){
		visible[i][0] = -1;
		visible[i][1]= -1;
	}
	struct bullet{
		int xPos;
		int yPos;
		int type;
		int dirX;
		int dirY;
	};	
	init_pair(1, COLOR_RED, COLOR_BLACK);	
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	//attron(COLOR_PAIR(1));
	struct bullet bullets[100];
	srand(time(NULL));
	getmaxyx(stdscr,sizeY,sizeX);
	xPos = 40;
	yPos = 20;
	sizeX -= step;
	int map[sizeY][sizeX];
	int findMap[sizeY][sizeX];
	
	for(int i = 0; i < 25; i++){
		inventory[i] = 0;
	}
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){
			map[k][i] = 2;
		}
	}
	int originX,originY;
	int rand4;
	originX = rand() % (sizeX - 3) + 1;
	originY = rand() % (sizeY - 3) + 1;
	map[originY][originX] = 0;
	for(int i = 0; i < 7000; i++){
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
	for(int i = 0; i < 5; i++){
		int randX = rand() % (sizeX - 15) + 2;
		int randY = rand() % (sizeY - 15) + 2;
		int siz1 = rand() % 3 + 6;
		int siz2 = rand() % 3 + 6;
		int cntr = 0;
		for(int k = randY; k <= randY + siz1; k++){
			for(int l = randX; l <= randX + siz2; l++){
				if(k == randY || k == randY + siz1
					|| l == randX || l == randX + siz2){
					if(map[k - 1][l] != 2 && map[k + 1][l] != 2 &&
						 cntr < 3){
						map[k][l] = 3;
						cntr++;
					}
					else if(map[k][l - 1] != 2 &&
					 map[k][l + 1] != 2 && cntr < 3){
						map[k][l] = 3;
						cntr++;	
					}
					else
						map[k][l] = 2;
				}
				else map[k][l] = 0;		
			}
		}
		cntr = 0;
	}
	for(int i = 0; i < 0; i++){
		int sX = rand() % sizeX;
		int sY = rand() % sizeY;
		map[sX][sY] = 4;
	}
	for(int i1 = 0; i1 < 10; i1++){
		for(int k1 = 0; k1 < 10; k1++){
			map[i1 - 5 + yPos][k1 - 5 + xPos] = 0;
		}
	}
	map[yPos + 1][xPos + 1] = 5;
	map[yPos + 2][xPos + 2] = 3;
	map[yPos + 3][xPos + 3] = 7;
	map[yPos - 1][xPos - 1] = 6;
	map[yPos][xPos] = 4;
	keypad(stdscr,true);
	noecho();
	halfdelay(100);
	bool ex = false;
	curs_set(0);
	int way[100][2];
	bool chDirect = false, chDirect2 = false;
	while(!ex){
		//clear();
		/*for(int i = 0; i < sizeX; i++){
			for(int k = 0; k < sizeY; k++){
				if(map[k][i] == 2)
					mvprintw(k,i + step,"#");
				if(map[k][i] == 0)
					mvprintw(k,i + step," ");
			}
		}*/
		if(xPos < 30 && map[20][40] == 0)
			map[20][40] = 6;	
		for(angle = -4; angle <= 4; angle += 0.2){
			for(float x = 0; x >= -4; x -= 0.2){	
				if(map[(int)(angle * x + yPos)]
					[(int)(x + xPos + 0.5)] == 2){
						break;
				}else{
					visible[index][0] = (int)(angle * x + yPos);
					visible[index][1] = (int)(x + xPos + 0.5);
					index++;
				}					
			}
			for(float x = 0; x <= 4; x += 0.2){	
				if(map[(int)(angle * x + yPos)]
					[(int)(x + xPos + 0.5)] == 2){
						break;
				}else{
					visible[index][0] = (int)(angle * x + yPos);
					visible[index][1] = (int)(x + xPos + 0.5);
					index++;	
				}
			}
		}				
		for(int y = 0; y >= -viewRadius; y--){
			if(map[(int)(y + yPos)][xPos] == 2){
				break;
			}else{
				visible[index][0] = (int)(y + yPos);
				visible[index][1] = xPos;
				index++;
			}
		}	
		for(int y = 0; y <= viewRadius; y++){
			if(map[(int)(y + yPos)][xPos] == 2){
				break;
			}else{
				visible[index][0] = (int)(y + yPos);
				visible[index][1] = xPos;
				index++;
			}
		}              
		for(int i = 0; i < index; i++){
			int i1 = visible[i][0];
			int i2 = visible[i][1];
			int wid = xPos - i2;
			int hei = yPos - i1;
			if(hei * hei + wid * wid <= viewRadius * viewRadius){
				switch(map[i1][i2]){
					case 0:
						mvprintw(i1, i2 + step, ".");
						break;
					case 3:	
						attron(COLOR_PAIR(1));
						mvprintw(i1, i2 + step, "/");	
						attroff(COLOR_PAIR(1));
						break;
					case 4:
						mvprintw(i1, i2 + step, "P");
						break; 
					case 5:
						mvprintw(i1, i2 + step, "$");
						break;
					case 6:
						mvprintw(i1, i2 + step, "Y");
						break;
					case 7:
						mvprintw(i1, i2 + step, "*");
						break;
				}
			}		
		}
		for(int i = 0; i < sizeX; i++){
			for(int k = 0; k < sizeY; k++){
				int xView = xPos - i;
				int yView = yPos - k;
				findMap[k][i] = -1;
				//if(xView * xView + yView * yView > viewRadius * viewRadius){
					if(map[k][i] == 2){
						if(k == 0 || k == sizeY - 1 ||
							i == 0 || i == sizeX - 1){
							mvprintw(k,i + step,"#");
						}else{
							if(map[k - 1][i] !=2 &&
								 map[k + 1][i] != 2)
								mvprintw(k,i + step,"-");
							else
								mvprintw(k, i + step, "|");
							}
					}
			}
		}
		index = 0;
		findMap[yPos][xPos] = 0;
		//int queueX[200]; //FIXME: remove magic number
		//int queueY[200]; //FIXME: remove magic number
		//int qStart=0, qEnd=1;
		//x = queueX[qStart]; y = queueY[qStart]; qStart = (qStart +1)%200;
		//queueX[qEnd] = x; queueY[qEnd] = y; qEnd = (qEnd + 1) % 200;
		int ra1 = 1;/*
		while(ra1 < sizeY / 2 - 2){
			for(int i = xPos - ra1; i < xPos + ra1; i++){
				for(int k = yPos - ra1; k < yPos + ra1; k++){
					if(findMap[k][i] != - 1){
						if(findMap[k - 1][i] != -1 &&
						map[k - 1][i] != 2)
							findMap[k - 1][i] = findMap[k][i] + 1;
						if(findMap[k + 1][i] != -1 && 
						map[k + 1][i] != 2)
							findMap[k + 1][i] = findMap[k][i] + 1;	
						if(findMap[k][i - 1] != -1 &&
						map[k][i - 1] != 2)
							findMap[k][i - 1] = findMap[k][i] + 1;	
						if(findMap[k][i + 1] != -1 &&
						map[k][i + 1] != 2)
							findMap[k][i + 1] = findMap[k][i] + 1;
					}
				}
			} ra1++;
		}*/
		attron(COLOR_PAIR(1));
		mvprintw(yPos,xPos + step,"@",3);
		attroff(COLOR_PAIR(1));
		mvprintw(1,1,"             ",line);
		switch(map[yPos][xPos]){
			case 0:
				strcpy(line, "_");
				break;
			case 4:
				strcpy(line, "A key!");
				break;
			case 5:
				strcpy(line, "Some money!");
				break;
			case 6:
				strcpy(line, "A slingshot!");
				break;
			case 7:
				strcpy(line, "A bullet!");
				break;
		}
		mvprintw(1,1,"%s",line);
		int ch = getch();
		for(int i = 0; i < cntBul; i++){
			mvprintw(bullets[i].yPos,bullets[i].xPos,"*");
			bullets[i].xPos += bullets[i].dirX;
			bullets[i].yPos += bullets[i].dirY;
			if(map[bullets[i].xPos][bullets[i].yPos] == 2){
				cntBul--;	
			}
		}
//		if(timer2 != timer){
		if(chDirect || chDirect2){
			switch(ch){
				case KEY_UP:
					if(map[yPos - 1][xPos] == 3 && chDirect){
						map[yPos - 1][xPos] = 0;
						inventory[4]--;
						chDirect = false;
					}	
					if(chDirect2){
						cntBul++;
						inventory[7]--;
						bullets[cntBul].xPos = xPos;
						bullets[cntBul].yPos = yPos;
						bullets[cntBul].type = 0;
						bullets[cntBul].dirX = 0;
						bullets[cntBul].dirY = -1;
					}
					break;
				case KEY_DOWN:
					if(map[yPos + 1][xPos] == 3 && chDirect){
						map[yPos + 1][xPos] = 0;
						inventory[4]--;
						chDirect = false;
					}
					if(chDirect2){
						cntBul++;
						inventory[7]--;
						bullets[cntBul].xPos = xPos;
						bullets[cntBul].yPos = yPos;
						bullets[cntBul].type = 0;
						bullets[cntBul].dirX = 0;
						bullets[cntBul].dirY = 1;
					}
					break;
				case KEY_LEFT:
					if(map[yPos][xPos - 1] == 3 && chDirect){
						map[yPos][xPos - 1] = 0;
						inventory[4]--;
						chDirect = false;
					}
					if(chDirect2){
						cntBul++;
						inventory[7]--;
						bullets[cntBul].xPos = xPos;
						bullets[cntBul].yPos = yPos;
						bullets[cntBul].type = 0;
						bullets[cntBul].dirX = -1;
						bullets[cntBul].dirY = 0;
					}
					break;
				case KEY_RIGHT:
					if(map[yPos][xPos + 1] == 3 && chDirect){
						map[yPos][xPos + 1] = 0;
						inventory[4]--;
						chDirect = false;
					}
					if(chDirect2){
						cntBul++;
						inventory[7]--;
						bullets[cntBul].xPos = xPos;
						bullets[cntBul].yPos = yPos;
						bullets[cntBul].type = 0;
						bullets[cntBul].dirX = 1;
						bullets[cntBul].dirY = 0;
					}
					break;
			}
		}
		switch(ch){	
				case 115:
					if(inventory[7] > 0 && inventory[6] > 0){
						chDirect2 = true;
					}
					break;
				case 112:
					switch(map[yPos][xPos]){
						case 4:
							inventory[4] += 1;
							map[yPos][xPos] = 0;
							break;
						case 5:
							inventory[5] += 1;
							map[yPos][xPos] = 0;
							break;
						case 6:
							inventory[6] += 1;
							map[yPos][xPos] = 0;
							break;
						case 7:
							inventory[7] += 1;
							map[yPos][xPos] = 0;
							break;
					}
					break;
				case 111:
					if(inventory[4] > 0)
						chDirect = true;
					break;
				case KEY_UP:
					if(map[yPos - 1][xPos] != 2 &&
						yPos > 0 && map[yPos - 1][xPos] != 3)
						yPos--;
					break;
				case KEY_DOWN:
					if(map[yPos + 1][xPos] != 2 &&
						 yPos < sizeY - 1 &&
						 map[yPos + 1][xPos] != 3)
						yPos++;
					break;
				case KEY_LEFT:
					if(map[yPos][xPos - 1] != 2 &&
					xPos > 0 && map[yPos][xPos - 1] != 3)
						xPos--;	
					break;
				case KEY_RIGHT:
					if(map[yPos][xPos + 1] != 2 && 
					xPos < sizeX - 1 && map[yPos][xPos + 1] != 3)
						xPos++;
					break;
				case KEY_F(2):
					ex = true;
					break;
				default:
					break;
//			}	
		}
	//	clear();
		/*for(int i = 0; i < sizeX; i++){
			for(int k = 0; k < sizeY; k++){
				mvprintw(k,i,"%d",findMap[k][i]);
			}
		}*/
		refresh();
	
	}
	getch();
	endwin();		
} 
//chtype ch = '#' | COLOR_PAIR(1);
//mvaddch(1,20, ch);
