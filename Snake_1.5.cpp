#include <iostream>
#include <ncurses.h>
#include <chrono>
#include <thread>
#include <ctime>
#include <vector>
using namespace std;
using namespace std::this_thread;


int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } 
    else 
        return 0;
}


bool GameOver;
int N = 30;
int y, x, FruttaY, FruttaX, score = 0;
vector <int> codaX, codaY;
vector <int> OstacoloX, OstacoloY;
int nCoda = 0;
enum Direzioni {STOP = 0, SU, DESTRA, SOTTO, SINISTRA};
int Difficolta = 1;
int Bordi = 0;
Direzioni Dir;

void Spawn()
{
	bool check = false;
	do
	{
		check = false;
		FruttaY = rand() % N;
		FruttaX = rand() % N;
		if(OstacoloX.size() > 0)
		{
			for(int z = 0; z < OstacoloX.size(); z++)
			{
				if(FruttaY == OstacoloY.at(z) && FruttaX == OstacoloX.at(z))
					check = true;
			}
		}
	}while(check);
	

	if(codaY.size() % 2 == 0)
	{
		for(int i = 0; i < Difficolta; i++)
		{
			check = false;
			int OY, OX;
			do
			{
				check = false;
				OY = rand() % N;
				OX = rand() % N*2;
				if(codaY.size() > 0)
				{
					for(int z = 0; z < codaY.size(); z++)
					{
						if(OY == codaY.at(z) && OX == codaX.at(z))
							check = true;
					}
				}
				if(OY == FruttaY && OX == FruttaX)
					check = true;
				
				if(OY == y && OX == x)
					check = true;

				if(OstacoloX.size() > 0)
				{
					for(int z = 0; z < OstacoloX.size(); z++)
					{
						if(OX == OstacoloX.at(z) && OY == OstacoloY.at(z))
							check = true;
					}
				}

			}while(check);

			OstacoloX.push_back(OX);
			OstacoloY.push_back(OY);
		
		}
	}
}
void Start()
{
	Dir = STOP;
	GameOver = false;
	y = N / 2;
	x = N;
	Spawn();
}

void Stampa()
{
	int K = N*2;
	attron(COLOR_PAIR(4));
	for(int i = 0; i < K; i++)
		printw("#");
	printw("##");
	attroff(COLOR_PAIR(4));
	printw("\n");

	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j <= K; j++)
		{
			bool check = false;

			if(j == 0)
			{
				attron(COLOR_PAIR(4));
				printw("#");
				attroff(COLOR_PAIR(4));
			}
			if(j == K)
			{
				attron(COLOR_PAIR(4));
				printw("#");
				attroff(COLOR_PAIR(4));
			}

			if(OstacoloX.size() > 0)
			{
				for(int z = 0; z < OstacoloX.size(); z++)
				{	
					if(OstacoloX.at(z) == j && OstacoloY.at(z) == i)
					{
						check = true;
						attron(COLOR_PAIR(3));
						printw("#");
						attroff(COLOR_PAIR(3));
					}
				}
			}	

			if(i == y && j == x)
			{
				attron(COLOR_PAIR(1));
				check = true;
				printw("O");
				attroff(COLOR_PAIR(1));
			}
			if(i == FruttaY && j == FruttaX)
			{
				attron(COLOR_PAIR(2));
				check = true;
				printw("@");
				attroff(COLOR_PAIR(2));
			}
			if(!check)
			{
				for(int z = 0; z < codaY.size(); z++)
				{
					if(codaY.at(z) == i && codaX.at(z) == j)
					{	
					
						attron(COLOR_PAIR(1));			
						check = true;
						printw("o");
						attroff(COLOR_PAIR(1));
					}
				}
			}

			if(!check)
				printw(" ");

		}
		printw("\n");
	}

	attron(COLOR_PAIR(4));
	for(int i = 0; i < K; i++)
		printw("#");

	printw("##");
	attroff(COLOR_PAIR(4));
	printw("\n");

	printw("Score : %u", score);
	printw("\n\n");
	printw("Usa le frecce direzionali");
	printw("\n");
	printw("press ESC to exit");
}

void Input()
{
	bool precedente = false;
	int C = 0;
	static int prev = 0;
	if(kbhit())
	{
		C = getch();
		switch(C)
		{
			case KEY_LEFT:
			if(prev != KEY_RIGHT)
				Dir = SINISTRA;
			break;

			case KEY_UP:
			if(prev != KEY_DOWN)
				Dir = SU;
			break;

			case KEY_DOWN:
			if(prev != KEY_UP)
				Dir = SOTTO;
			break;

			case KEY_RIGHT:
			if(prev != KEY_LEFT)
				Dir = DESTRA;
			break;

			case 27:
			GameOver = true;
			break;

			default :
			Dir = STOP;
		}
		precedente = false;
		if( (prev == KEY_UP && C == KEY_DOWN) || (prev == KEY_DOWN && C == KEY_UP) || (prev == KEY_RIGHT && C == KEY_LEFT) || (prev == KEY_LEFT && C == KEY_RIGHT) )       
			precedente = true;
		
		if(!precedente)
			prev = C;
	}
}

void Logic()
{	
	if(codaY.size() > 0)
	{
		if(Dir != STOP)
		{
			for(int i = codaY.size() - 1; i >= 0; i--)
			{
				if(i == 0)
				{
					codaY.at(i) = y;
					codaX.at(i) = x;
				}
				else
				{
					codaY.at(i) = codaY.at(i-1);
					codaX.at(i) = codaX.at(i-1);
				}
			}
		}
	}

	int K = N*2;
	switch(Dir)
	{
		case SU:
		y--;
		if(y < 0)
		{
			if(Bordi == 0) 
				y = N-1;
			else    
				GameOver = true;
		}
		break;

		case DESTRA:
		x++;
		if(x >= K)
		{
			if(Bordi == 0)
				x = 0;
			else
				GameOver = true;
		}
		break;

		case SOTTO:
		y++;
		if(y >= N)
		{
			if(Bordi == 0)
				y = 0;
			else
				GameOver = true;
		}
		break;

		case SINISTRA:
		x--;
		if(x < 0)
		{
			if(Bordi == 0)
				x = K-1;
			else
				GameOver = true;
		}
		break;
	}

	if(OstacoloX.size() > 0)
	{
		for(int z = 0; z < OstacoloX.size(); z++)
		{
			if(OstacoloY.at(z) == y && OstacoloX.at(z) == x)
				GameOver = true;
		}
	}

	for(int z = 0; z < codaY.size(); z++)
	{
		if(y == codaY.at(z) && x == codaX.at(z))
			GameOver = true;
	}

	if(x == FruttaX && y == FruttaY)
	{
		score += 10;
		Spawn();
		codaY.push_back(y);
		codaX.push_back(x);

	}

}

void Colori()
{
	start_color();
	init_pair (1, COLOR_GREEN, COLOR_BLACK);
	init_pair (2, COLOR_RED, COLOR_BLACK);
	init_color (COLOR_YELLOW, 900, 900, 0);
	init_pair (3, COLOR_YELLOW, COLOR_YELLOW);
	init_pair (4, COLOR_WHITE, COLOR_WHITE);
}

void Splash()
{
	system("clear");
	std::cout << "Menu' : \n1 - Gioca \n2 - Impostazioni" << endl;
	int scelta;
	int x = 1;
	cin >> scelta;
	switch(scelta)
	{
		case 1:
		break;

		case 2:
		int s;
		system("clear");
		std::cout << "1 - Difficoltà\n2 - Bordi" << endl;
		cin >> s;
		switch(s)
		{
			case 1:
			system("clear");
			std::cout << "Difficoltà : \n1 - Facile\n2 - NORMALE\n3 - DIFFICILE\n4 - MOLTO DIFFICILE\n5 - IMPOSSIBILE" << endl;
			std::cout << endl << "Attuale : " << Difficolta << endl;
			cin >> x;
			if(x >= 1 && x <= 5)
			Difficolta = x;
			Splash();
			break;

			case 2:
			system("clear");
			std::cout << "Bordi : \n1 - NO\n2 - SI" << endl;
			std::cout << endl << "Attuale : " << Bordi + 1 << endl;
			cin >> x;
			if(x == 1 || x == 2)
			{
				Bordi = x;
				Bordi -= 1;
			}
			Splash();
			break;

			default:
			Splash();
		}
		break;

		default:
		Splash();
	}
}
int main()
{
	srand(time(NULL));
	Splash();

	std::cout << "Inserisci la dimensione della finestra di gioco : ";
	cin >> N;
	std::cout << "Avvio .." << endl;
	sleep_for(2s);

	initscr();
	clear();
	Colori();
	noecho();
	Start();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	while(!GameOver)
	{
		if(Dir == SU || Dir == SOTTO)
			sleep_for(0.13s);
		else
			sleep_for(0.08s);

		move(0,0);
		Stampa();
		Input();
		Logic();
	}
	while(!kbhit())
	{
		refresh();
		move(N+3, 0);
		printw("Game Over (Press a button) \n\n");
	}
	sleep_for(1s);
	nodelay(stdscr, FALSE);
	getch();
	endwin();

	std::cout << "Fine partita" << endl;
}