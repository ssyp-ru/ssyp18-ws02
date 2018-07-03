#include <stdio.h>
#include <ncurses.h>
	
int main(){
	initscr();
	int map[50][50];
	int sizeX = 50;
	int sizeY = 50;
	int ra1 = 1;
	int findMap[50][50];
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){
			map[k][i] = 2;
			findMap[k][i] = -1;
		}
	}
	int xPos = 25;
	int yPos = 25;
	map[yPos][xPos] = 0;	
	map[yPos][xPos+1] = 0;	
	map[yPos][xPos+2] = 0;	
	map[yPos][xPos+3] = 0;	
	map[yPos-1][xPos+3] = 0;		
	map[yPos-2][xPos+3] = 0;	
	map[yPos-3][xPos+3] = 0;									map[yPos-3][xPos+2] = 0;	
	map[yPos-3][xPos+1] = 0;	
	map[yPos-3][xPos] = 0;	
	map[yPos-3][xPos-1] = 0;	
	map[yPos-3][xPos-2] = 0;	
	map[yPos-2][xPos-2] = 0;	

	while(ra1 < 16){
		for(int i = xPos - ra1; i < xPos + ra1; i++){
			for(int k = yPos - ra1; k < yPos + ra1; k++){
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
		} ra1++;
	}
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){
			mvprintw(k, i, "%d", findMap[k][i]);
		}
	}
	refresh();
	getch();
	endwin();
	return 0;
}
