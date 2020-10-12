#include <stdio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const char *shipName[] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
const int shipShots[] = {5, 4, 3, 3, 2};

typedef struct {
	int squares[5][3]; // 0=x, 1=y, 2=hit
} ship;

typedef struct {
	ship ships[5];
	// Game symbols
	// ~ = water
	// S = Ship
	// X = hit
	// O = miss
	char map[10][10];
	char shots[10][10];
	int destroyed;
} player;

void name();
void first();
void playerInit(player *p);
int playerTurn(player *u, player *a);
int computerTurn(player *a, player *b);
void placeShips(char map[10][10], ship ships[5]);
int isValid(int x, int y);
int isClear(char map[10][10], int x, int y);
void printMap(player *p);
void initialize(char arr[10][10]);
int shot(int x, int y, player *from, player *to);
int c2i(char c);
char i2c(int i);

void name() {
   char name[25];
   FILE *file;

   file = fopen("program.txt", "w");

   if(file == NULL) {
      printf("Error!");   
      exit(1);             
   }

    printf("Enter name: ");
    scanf("%s", name);

	fprintf(file,"%s\n",name);

    fclose(file);	
}


void first(){
	int userInput = 0;
	int choice;


			printf("\t\t------------------------------------------------------------------------------------\n");
			printf("\t\t  ☆               ___     _  ___ ___      __   __       ___   __      _____         \n");
			printf("\t\t                 |___\\   /_\\  |   |  |   |__  /__  |__|  |   |__|     \\   /      \n");
			printf("\t\t                 |___/  /   \\ |   |  |__ |__  ___\\ |  | _|_  |         | |      ☆ \n");
            printf("\t\t                                                                      /   \\        \n");
            printf("\t\t      ☆                     ___                 ____                 |  ✇  |        \n");
            printf("\t\t                           |       _   |\\  /|  |                      \\___/       \n");
            printf("\t\t                ☆          | __   /_\\  | \\/ |  |----           ☆       \\ /       \n");
			printf("\t\t                           |___| /   \\ |    |  |____                    |          \n");
			printf("\t\t     ☆                                                                              \n");              
            printf("\t\t                                ©Danil Sisov                                 ☆      \n");
			printf("\t\t                 .      ☆                            ☆                              \n");
			printf("\t\t  ☆            __|_____                                        <      ☆            ☆\n");
			printf("\t\t      ________|  o   o |______          ☆                   ___|____                \n");
			printf("\t\t      \\   o   o   o   o   o  /              _______________/  o   o \\____         \n");
			printf("\t\t~~~~~~~\\~~~~~~~~~~~~~~~~~~~~/~~~~~~~~~~~~~~/~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\~~~~~~~ \n");
			printf("\t\t                                                                                    \n");
			printf("\t\t                              			                                           \n");
			printf("\t\t                                1. Play game                                        \n");
			printf("\t\t                                2. Exit                                             \n");
			printf("\t\t------------------------------------------------------------------------------------\n");
			
			printf("Select option (1-2): ");
			scanf("%d", &userInput);

		switch (userInput) {
		
		case 1: 
            printf("                         Welcome to Battelship Game");
			printf("\n1. You will play against the computer\n");
			printf("2. Enemy card on the left, your card with ships on the right\n");
			printf("3. Every game ships put randomly\n");
			printf("4. To strike, you need to choose a letter from a to j and numbers from 0 to 9\n");
			printf("5. Game symbols\n~ = water \nS = Ship \nX = hit \nO = miss\n\n");

			printf("Type 1 if you want return to main menu\n");
			printf("Type any number since 2 to start play\n");
			printf("Option: ");
			scanf("%d", &choice);
			  if(choice == 1) {
				  return first();
			  } 
			  else {

			  }

			break;

		case 2:
			exit(0);	
		}
	
}

void playerInit(player *p) {

	initialize(p->map);
	
	initialize(p->shots);
	
	placeShips(p->map, p->ships);
	
	p->destroyed = 0;
	
}

int playerTurn(player *u, player *a) {
	char x;
	int y;
	
	printf("Insert coordinates: ");
	int s = scanf(" %c%d", &x, &y);
	if (s == 2 && x >= 'a' && x <= 'j' && y >= 0 && y <= 9 ) { 	
		if( shot(c2i(x), y, u, a)) {
			return 1;
		}
	} 

	else {
		printf("Error, choose a letter from a to j and numbers from 0 to 9\n");
		playerTurn(u, a);
	}
	return 0;
}

int computerTurn(player *a, player *u) {
	int i, j, x, y, k;
	
	//hit and shoot
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
		
			if (a->shots[i][j] == 'x') { 
				for (k = 0; k < 3; k++) {
					x = i + rand() % 3 - 1;
					y = j + rand() % 3 - 1;

					if (isValid(x, y) && a->shots[x][y] == '~') {
							printf("Computer shot %c%d. ", i2c(x), y);

							if (shot(x, y, a, u)){
								return 1;
							} 

						return 0;
					}
				}
			}
		}
	}
	
	//random shoot
	do {
		x = rand() % 10;
		y = rand() % 10;
	} while(a->shots[x][y]!='~');
	
	printf("Computer shot %c%d. ", i2c(x), y);

	if (shot(x, y, a, u)) {
		return 1;
	}

	return 0;
}

void placeShips(char map[10][10], ship ships[5]) {
	int s, i, d, c, x, y, j, cx, cy;	

	for (i = 0; i < 5; i++) {
		d = rand() % 2; //direction, 0 is vertical
		c = 0; //check, 1 when the ship is placed

		while (c == 0) { // I pick one square randomly and check if I can put the current ship there
			c = 1;
			x = rand() % 10;
			y = rand() % 10;
			cx = x;
			cy = y;
			c = 1;

			for (j = 0; j < shipShots[i]; j++) {
				if (isClear(map, cx, cy)) {
					if (d == 0) {
						cy++;
					} 
					else {
						cx++;
					}
				} 
				else {
					c = 0;
					break;
				}
			}
			
			if (c == 0) {
			 continue;
			}  
			
			cx = x;
			cy = y;
			
			for (j = 0; j < shipShots[i]; j++) {
				map[cx][cy] = 'S';//i + '0';
				ships[i].squares[j][0] = cx;
				ships[i].squares[j][1] = cy;
				ships[i].squares[j][2] = 0;
				if (d == 0) {
					cy++;
				} 
				else {
					cx++;
				}
			}
		}
	}
}

int isValid(int x, int y) {
	if (x < 0 || y < 0){
		return 0;
	} 
	if (x > 9 || y > 9) {
		return 0;
	}  

	return 1;
}

int isClear(char map[10][10], int x, int y) {
	if (isValid(x, y) && map[x][y] == '~') {
		return 1;
	} 

	return 0;
}

void printMap(player *p) {
	int x, y;
	
	printf("\n  0 1 2 3 4 5 6 7 8 9       0 1 2 3 4 5 6 7 8 9\n");

	for (x = 0; x < 10; x++) {
		printf("%c ", 'A'+x);

		for (y = 0; y < 10; y++) {
			printf("%c ", p->shots[x][y]);
		}

		printf("    ");
		printf("%c ", 'A'+x);

		for (y = 0; y < 10; y++) {
			printf("%c ", p->map[x][y]);
		}
		
		printf("\n");
	}
	printf("\n");
}

void initialize(char arr[10][10]) {
	int x, y;

	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			arr[x][y] = '~';
		}
	}	
}

int shot(int x, int y, player *from, player *to) { //it returns 1 if player from wins
	if (to-> map[x][y] == '~' || to-> map[x][y] == 'o') { //miss
		to-> map[x][y] = 'o';
		from-> shots[x][y] = 'o';
		printf("Miss!\n");
	} 

	else if (to->map[x][y] == 'S') { //hit
		to-> map[x][y] = 'X';
		from-> shots[x][y] = 'X';

		//updating ship's state
		int i, j, k;
		for (i = 0; i < 5; i++) {
			for (j = 0; j < shipShots[i]; j++) {
				if (to-> ships[i].squares[j][0] == x && to-> ships[i].squares[j][1] == y) {
					to->ships[i].squares[j][2] = 1;
					int isSunk = 1;
					for (k = 0; k < shipShots[i]; k++) {
						if (to->ships[i].squares[k][2] == 0) {
							isSunk = 0;
							break;
						}
					}
					if (isSunk) {
						printf("%s hit and sunk!\n", shipName[i]);
						from-> destroyed++;
						if (from-> destroyed== 5){
							return 1;
						} 

						return 0;
					}
				}
			}
		}	
		printf("Hit!\n");	

	} 
	else { //already hit
		printf("It was already hit there.\n");
	}
	
	return 0;
}


int c2i(char c) {
	return c - 'a';
}

char i2c(int i) {
	return i + 'a';
}



int main() {

	srand (time(NULL) );
	
	player user, computer;
	name();
	first();
	playerInit(&user);
	playerInit(&computer);
	

	while(1) {
		printMap(&user);
		//printMap(&ai); just for debugging
		printf("\n\n");

		if (playerTurn(&user, &computer)) {
				printMap(&user);
				printf("\n\nWIN!!!!!!!!!!!!!!!!!\n");
				break;
		}

		if (computerTurn(&computer, &user)) {
				printMap(&user);
				printf("\n\nYOU LOST!!!!!!!!!!!\n");
				break;
		}
	}

	return 0;
}


