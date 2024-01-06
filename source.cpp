#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

const int EASY = 9;
const int MEDIUM = 15;
const int HARD = 20;
int score = 0;
int rows, cols;
string name;
int hint_i, hint_j, mine_count = 0;

//function prototypes
void initialise_board(char grid[][HARD]);
void initialise_disp_board(char grid[][HARD]);
void print_board(char grid[][HARD]);
void print_disp_board(char grid[][HARD]);
void generate_mines(char grid[][HARD], int total_mines);
void hints(int x, int y, char grid[][HARD]);
void input(char grid[][HARD], char grid2[][HARD]);
void open_multiple_boxes_when0(int x, int y, char grid[][HARD], char grid2[][HARD]);
void open_cell(char grid[][HARD], char grid2[][HARD]);
void flag(char grid[][HARD], char grid2[][HARD]);
void unflag(char grid[][HARD], char grid2[][HARD]);
void flag_counter(char grid[][HARD], char grid2[][HARD]);
void reveal_mines(char grid[][HARD], char grid2[][HARD]);
bool win(char grid[][HARD], char grid2[][HARD], int total_mines);
void unique_highlight(char grid[][HARD], char grid2[][HARD], int x, int y);
void writeScoreToFile();
void play_game();

int main()
{
	play_game();

	 return 0;
		
}

//function to keep track of the score each time you play the game
void writeScoreToFile() {
	string line;
	ofstream outfile("scores.txt", ios_base::app);
	if (outfile.is_open()) {
		outfile << name << "     " + to_string(score) << endl;
		outfile.close();
		cout << "Name  Score" << endl;
		ifstream infile("scores.txt");
		while (!infile.eof())
		{
			getline(infile, line);
			cout << line << endl;
		}
		infile.close();
	}
	else {
		cout << "Unable to open file." << endl;
	}
}


//the start-up function of the game which includes all the function calls
void play_game()
{
	cout << "-----Welcome To Minesweeper----" << endl;
	start:
	cout << "Enter your name : ";
	cin >> name;
	cout << "Difficulty:" << endl << "1.Easy\n2.Medium\n3.Hard" << endl;
	char choice;
	int mines_generated;
	char grid[HARD][HARD];
	char grid2[HARD][HARD];
	bool won = true;
	begin:
	cin >> choice;
	while (true)
	{
		switch (choice)
		{
		case '1':
			rows = EASY; // 9 rows
			cols = EASY; // 9 columns
			mines_generated = 10; 
			hint_i = rand() % rows; // a variable used for a function which I myself added to the game for twisting the dynamics
			hint_j = rand() % cols; // a variable used for a function which I myself added to the game for twisting the dynamics
			initialise_board(grid);
			initialise_disp_board(grid2);
			generate_mines(grid, mines_generated);
			while (true)
			{
				flag_counter(grid, grid2);
				print_disp_board(grid2);
				input(grid, grid2);
				if (won == win(grid, grid2, mines_generated))
				{
					cout << "CONGRATULATIONS!! YOU WON!" << endl;
					writeScoreToFile();
					begin:
					cout << "Would you like to play again? (Y/N)" << endl;
					char choice;
					cin >> choice;
					switch (choice)
					{
					case 'y':
					case 'Y':
						system("cls");
						goto start; // in case the user wants to play again begin from the start again
						break;
					case 'n':
					case 'N':
						cout << "THANK YOU FOR PLAYING!" << endl;
						exit(0);
					default:
						system("cls");
						cout << "Invalid choice" << endl;
						exit(0); // exit the game 
						break;
					}
				}
			}
			break;
		case '2':
			rows = MEDIUM; // 15 rows
			cols = MEDIUM; // 15 columns
			mines_generated = 30;
			hint_i = rand() % rows;
			hint_j = rand() % cols;
			initialise_board(grid);
			initialise_disp_board(grid2);
			generate_mines(grid, mines_generated);
			print_board(grid);
			while (true)
			{
				flag_counter(grid, grid2);
				print_disp_board(grid2);
				input(grid, grid2);
				if (won == win(grid, grid2, mines_generated))
				{
					cout << "CONGRATULATIONS!! YOU WON!" << endl;
					writeScoreToFile();
					cout << "Would you like to play again? (Y/N)" << endl;
					char choice;
					cin >> choice;
					switch (choice)
					{
					case 'y':
					case 'Y':
						system("cls");
						goto start;
						break;
					case 'n':
					case 'N':
						cout << "THANK YOU FOR PLAYING!" << endl;
						exit(0);
					default:
						system("cls");
						cout << "Invalid choice" << endl;
						exit(0);
						break;
					}

				}
			}

			break;
		case '3':
			rows = HARD; //20 rows
			cols = HARD; //20 columns
			mines_generated = 60;
			hint_i = rand() % rows;
			hint_j = rand() % cols;
			initialise_board(grid);
			initialise_disp_board(grid2);
			generate_mines(grid, mines_generated);
			print_board(grid);
			while (true)
			{
				flag_counter(grid, grid2);
				print_disp_board(grid2);
				input(grid, grid2);
				if (won == win(grid, grid2, mines_generated))
				{
					cout << "CONGRATULATIONS!! YOU WON!" << endl;
					writeScoreToFile();
					cout << "Would you like to play again? (Y/N)" << endl;
					char choice;
					cin >> choice;
					switch (choice)
					{
					case 'y':
					case 'Y':
						system("cls");
						goto start;
						break;
					case 'n':
					case 'N':
						cout << "THANK YOU FOR PLAYING!" << endl;
						exit(0);
					default:
						system("cls");
						cout << "Invalid choice" << endl;
						exit(0);
						break;
					}

				}
			}
			break;
		default:
			cout << "Enter a valid choice" << endl; 
			goto begin; // in case user enters an invalid input at the start of the game
			break;
		}
	}
	
}

//custom function which twists the game dynamics by either revealing a mine or telling you the mine count of a certain row
void unique_highlight(char grid[][HARD], char grid2[][HARD], int x, int y)
{
	
	if (grid[x][y] == 'X') // if you hit a mine
	{
		cout << "You've hit a lucky cell. This is a mine" << endl;
	}
	else
	{
		if (grid[x][y] != '0' && grid2[x][y] == '+')
		{
			grid2[x][y] = grid[x][y];
			for (int i = 0; i < rows; i++) //counts the number of mines in a row
			{
				if (grid[x][i] == 'X')
				{
					mine_count++;
				}
			}
			cout << "You've hit a lucky cell. Row " << x + 1 << " has " << mine_count << " mines" << endl;
			mine_count = 0; //reset mine_count
		}
		else
		{
			if (grid[x][y] == '0' && grid2[x][y] == '+')
			{
				open_multiple_boxes_when0(x, y, grid, grid2); //so that the input works normally even when you've hit a special cell
				while (grid[x][y] == '0') //changing the the index of the grid until 0 keeps encountering
				{
					x = rand() % rows;
					y = rand() % cols;
				}
				for (int i = 0; i < rows; i++)
				{
					if (grid[x][i] == 'X')
					{
						mine_count++;
					}
				}
				cout << "You've hit a lucky cell. Row " << x + 1 << " has " << mine_count << " mines" << endl;
				mine_count = 0;
			}
			else
			{
				cout << "Box already opened" << endl; // if you are clicking on an already opened cell
			}
		}
	}
}

//function to initialise real board
	void initialise_board(char grid[][HARD])
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				grid[i][j] = '0';
			}
		}
	}
//function to print real board
	void print_board(char grid[][HARD])
	{
		// Display column indices
		cout << setw(34);
		for (int j = 1; j <= rows; j++) {
			cout << j << "  ";
		}
		cout << endl;

		// Display rows and row indices
		for (int i = 0; i < rows; i++) {
			cout << setw(30);
			cout << i + 1 << " | "; // Display row index

			for (int j = 0; j < cols; j++)
			{
				cout << grid[i][j] << "  ";
			}
			cout << "| " << endl << endl;
		}

	}
//function to initialise display board
	void initialise_disp_board(char grid[][HARD])
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				grid[i][j] = '+';
			}
		}
	}
//function to print display board
	void print_disp_board(char grid[][HARD])
	{
		// Display column indices
		cout << setw(34);
		for (int j = 1; j <= rows; j++) {
			if (j >= 10) // just for formatting the output of the grid/board
			{
				cout << j << " ";
			}
			else
			{
				cout << j << "  ";
			}
		}
		cout << endl;

		// Display rows and row indices
		for (int i = 0; i < rows; i++) {
			cout << setw(30);
			cout << i + 1 << " | "; // Display row index

			for (int j = 0; j < cols; j++)
			{
				cout << grid[i][j] << "  ";
			}
			cout << "| " << endl << endl;
		}
	}
//function for generating the mines on the real board
	void generate_mines(char grid[][HARD], int total_mines)
	{
		srand(static_cast<unsigned int>(time(nullptr))); //srand function to generate a random location each time the code executes
		int mines_placed = 0;
		int numRows = rows;
		int numCols = cols;
		bool has_mine[HARD][HARD] = { false }; //an array to keep track of the locations which already have a mine

		while (mines_placed < total_mines) {
			int a = rand() % rows;
			int b = rand() % cols;
			if (!has_mine[a][b]) {
				grid[a][b] = 'X';
				has_mine[a][b] = true;
				mines_placed++;
			}
		}
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				if (grid[i][j] == 'X') //the part where adjacent cells of a mine are incremented to a number indicating hints
				{
					hints(i + 1, j, grid);
					hints(i + 1, j + 1, grid);
					hints(i + 1, j - 1, grid);
					hints(i - 1, j - 1, grid);
					hints(i - 1, j, grid);
					hints(i - 1, j + 1, grid);
					hints(i, j + 1, grid);
					hints(i, j - 1, grid);
				}
			}
		}
	}
//function which generated hints by incrementing the cells which don't contain mines
	void hints(int x, int y, char grid[][HARD])
	{
		if (grid[x][y] != 'X' && x >= 0 && y >= 0 && x < rows && y < cols)
		{
			grid[x][y] += 1;
		}
	}
//function which opens multiple boxes till 0 keeps encountering
	void open_multiple_boxes_when0(int x, int y, char grid[][HARD], char grid2[][HARD])
	{
		if (grid[x][y] != 'X' && grid2[x][y] == '+' && x >= 0 && y >= 0 && x < rows && y < cols)
		{
			grid2[x][y] = grid[x][y];
			if (grid[x][y] == '0') //concept of recursion is used here
			{
				grid2[x][y] = '.';
				open_multiple_boxes_when0(x + 1, y, grid, grid2);
				open_multiple_boxes_when0(x + 1, y + 1, grid, grid2);
				open_multiple_boxes_when0(x + 1, y - 1, grid, grid2);
				open_multiple_boxes_when0(x - 1, y, grid, grid2);
				open_multiple_boxes_when0(x - 1, y - 1, grid, grid2);
				open_multiple_boxes_when0(x - 1, y + 1, grid, grid2);
				open_multiple_boxes_when0(x, y + 1, grid, grid2);
				open_multiple_boxes_when0(x, y - 1, grid, grid2);
			}
		}
	}
//function to open a cell
	void open_cell(char grid[][HARD], char grid2[][HARD])
	{
		int x, y;
		cout << "Enter row number: ";
		cin >> x;
		cout << "Enter column number: ";
		cin >> y;
		x -= 1; //because the indexes start from 0 and they are starting from 1 on the board
		y -= 1; //because the indexes start from 0 and they are starting from 1 on the board
		system("cls");
		if (x == hint_i && y == hint_j)
		{
			unique_highlight(grid, grid2, x, y); //calling the custom function
		}
		else if (grid[x][y] != '0' && grid[x][y] != 'X' && grid2[x][y] == '+' && x >= 0 && y >= 0 && x < rows && y < cols)
		{
			grid2[x][y] = grid[x][y]; //mapping the location of what the real board had on the display board
			score++; //incrementing the score
		}
		else{
			if (grid[x][y] == '0' && grid2[x][y] == '+' && x >= 0 && y >= 0 && x < rows && y < cols)
			{
				open_multiple_boxes_when0(x, y, grid, grid2); //calling the respective function
				score++;
			}
			else
			{
				if (grid[x][y] == 'X' && grid2[x][y] == '+')
				{
					cout << "You exploded a mine!" << endl << "GAME OVER!!" << endl;
					reveal_mines(grid, grid2);
					print_disp_board(grid2);
					writeScoreToFile();
					exit(0); //exit the game
				}
				else
				{
					if (x >= 0 && y >= 0 && x < rows && y < cols)
					{
						cout << "Box already opened" << endl;
					}
					else
					{
						cout << "Invalid row or column number" << endl;
					}
				}
			}
			
		}
		
	}
//function to flag a cell
	void flag(char grid[][HARD], char grid2[][HARD])
	{
		int x, y;
		cout << "Enter row number: ";
		cin >> x;
		cout << "Enter column number: ";
		cin >> y;
		x -= 1;
		y -= 1;
		system("cls");
		if (grid2[x][y] == '+' && grid2[x][y] != '.' && x >= 0 && y >= 0 && x < rows && y < cols)
		{
			grid2[x][y] = 'F';
		}
		else
		{
			if (grid2[x][y] == '.' && x >= 0 && y >= 0 && x < rows && y < cols)
			{
				cout << "Can't flag an empty space" << endl;
			}
			else
			{
				if (grid2[x][y] == 'F' && x >= 0 && y >= 0 && x < rows && y < cols)
				{
					cout << "Box already flagged" << endl;
				}
				else
				{
					if (x >= 0 && y >= 0 && x < rows && y < cols)
					{
						cout << "Can't flag a hint" << endl;
					}
					else
					{
						cout << "Invalid row or column number" << endl;
					}
				}
			}
		}

	}
//function to unflag a cell
	void unflag(char grid[][HARD], char grid2[][HARD])
	{
		int x, y;
		cout << "Enter row number: ";
		cin >> x;
		cout << "Enter column number: ";
		cin >> y;
		x -= 1;
		y -= 1;
		system("cls");
		if (grid2[x][y] == 'F' && x >= 0 && y >= 0 && x < rows && y < cols)
		{
			grid2[x][y] = '+';
		}
		else
		{
			if (x < 0 || y < 0 || x >= rows || y >= cols)
			{
				cout << "Invalid row or column number to unflag" << endl;
			}
			else
			{
				cout << "Can't unflag an empty space or already opened cell" << endl;
			}
			
		}
	}
//function to keep track of the number of mines in your difficulty mode and the number of flags you've placed
	void flag_counter(char grid[][HARD], char grid2[][HARD])
	{
		int mines = 0;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid[i][j] == 'X')
				{
					mines++; //counting the number of mines
				}
			}
		}
		cout << "Total Number Of Mines = " << mines << endl;
		int flag = 0;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid2[i][j] == 'F')
				{
					flag++; //counting the number of flags
				}
			}
		}
		cout << "Total Number Of Flags = " << flag << endl;
	}
//function to reveal mines when lost the game
	void reveal_mines(char grid[][HARD], char grid2[][HARD])
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid[i][j] == 'X')
				{
					grid2[i][j] = grid[i][j]; //maps all the mines of the real board on display board
				}
			}
		}
	}
//function to check if you won the game
	bool win(char grid[][HARD], char grid2[][HARD], int total_mines)
	{
		int count = 0;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (grid2[i][j] == 'F' && grid[i][j] == 'X')
				{
					count++; //to track how many mines were correctly flagged
				}
			}
		}
		if (count == total_mines)
		{
			score += count; //incrementing the score by number of mines if you won
			return true;
		}
		else
		{
			return false;
		}
	}
//function used for user input
	void input(char grid[][HARD], char grid2[][HARD])
	{
		char choice;
		cout << "1.Open a cell" << endl << "2.Set a Flag" << endl << "3.Unflag" << endl;
		cin >> choice;
		switch (choice)
		{
		case '1':
			open_cell(grid, grid2);
			break;
		case '2':
			flag(grid, grid2);
			break;
		case '3':
			unflag(grid, grid2);
			break;
		default:
			system("cls");
			cout << "Invalid choice" << endl;
		}
	}
