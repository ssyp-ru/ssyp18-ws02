#ifndef _GENERATE_03_07_18_22_01
#define _GENERATE_03_07_18_22_01

map_t* createMap(int width, int height);

void clearMap(map_t* mapp);

void createDividers(int num, int* data);

void generateMap(map_t* mapp, int num);

void generateRoad(map_t* mapp, int num);
#endif
