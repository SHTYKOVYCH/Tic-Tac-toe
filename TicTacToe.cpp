#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>//for shuffling
#include <ctime>

using namespace std;

void drawField(char field[3][3])	//simply outputing the field
{
	system("clear");
	cout << "  1 2 3\n";	//Printing the numbers of column for
				//user comfort
	int i, j;
	for( i = 0; i < 3; ++i)
	{
		cout << i+1 << "|";	//printing raw number for same reason
		for(j = 0; j < 3; ++j)
		{
			cout << field[i][j] << "|";
		}
		cout << endl;
	}
}

void emptier(char field[3][3])	//this function clear the given field
{
	int i, j;
	for(i = 0; i < 3; ++i)
	{
		for(j = 0; j < 3; ++j)
		{
			field[i][j] = ' ';
		}
	}
}

int win(char field[3][3])//checking if there a win for any side
	//if X wins - return 1
	//if O wind - return -1
	//if draw - return 0
{
	int i, j;
	for(i = 0; i < 3; ++i)	//just going thought every possible combination
	{
		if(field[i][0] == field[i][1] && field[i][0] == field[i][2])
		{
			if(field[i][0] == 'X')
			{
				return 1;
			}
			if(field[i][0] == 'O')
			{
				return -1;
			}
		}
		if(field[0][i] == field[1][i] && field[0][i] == field[2][i])
		{
			if(field[0][i] == 'X')
			{
				return 1;
			}
			if(field[0][i] == 'O')
			{
				return -1;
			}
		}
	}
	if(field[0][0] == field[1][1] && field[0][0] == field[2][2])
	{
		if(field[0][0] == 'X')
		{
			return 1;
		}
		if(field[0][0] == 'O')
		{
			return -1;
		}	
	}
	if(field[0][2] == field[1][1] && field[0][2] == field[2][0])
	{
		if(field[0][2] == 'X')
		{
			return 1;
		}
		if(field[0][2] == 'O')
		{
			return -1;
		}	
	}
	return 0;	
}

void copy(char where[3][3], char from[3][3])	//simply creating a copy of given field
						//and putting it in another container
{
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			where[i][j] = from[i][j];
		}
	}
}

int freed(char field[3][3])	//calculate number of free cells
{
	int c = 0;
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(field[i][j] == ' ')
			{
				c += 1;
			}
		}
	}
	return c;
}

bool compare(char what[3][3], char with[3][3])	//simply compare two fields
{
	for(int i = 0; i < 3; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(what[i][j] != with[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

struct tree	//the structure of every possible moves
{
	vector<tree> roots;	//all possible move from containing field
	char grid[3][3];	//the field of this knot
	int weight;		//contain the chanses for win for sides
				//if it's positive then X wins, if negative - then O wins
				//if it's zero? then it's draw
	int Xwin;		//A simple way to optimise a choose of move in game
	int total;		//

	tree(char field[3][3], int sim = 0)
		//field - a grid that need to be analised
		//sim - the side that make move. 0-X, 1-O
	{
		weight = 0;
		copy(grid, field);

		if(win(grid) != 0)	//checking if there a win
		{
			weight = win(field);
			if(win(grid) == 1)
			{
				Xwin = 1;
			}
			total = 1;
			return;
		}

		for(int i = 0; i < 3; ++i)	//going thought all possible moves
		{
			for(int j = 0; j < 3; ++j)
			{
				if(field[i][j] == ' ')
				{
					char temp[3][3];
					copy(temp, field);
					if(!sim)	//making a move
					{
						temp[i][j] = 'X';
					}
					else
					{
						temp[i][j] = 'O';
					}

					//Analising resulting grid
					roots.push_back(tree(temp, (sim + 1) % 2));
				}
			}
		}

		total = 0;
		Xwin = 0;

		for(auto i: roots)	//if this field is not wining
					//then calculate its weight as sum of weight of its roots
		{
			weight += i.weight;
			total += i.total;
			Xwin += i.Xwin;
		}
	}

	bool search(char field[3][3], char sim = 'X')
		//searching best move for "sim"
	{
		sim = (sim == 'X') ? 1 : -1;
		if(compare(grid, field))	//if given field is the one
						//we hold in a knot then analise it
		{
			//if searching for 'X' then we searching the greatest weight
			//if searching for 'O' then search for smallest weight
			int m = (sim == 1) ? -1000000 : 1000000;

			for(auto i: roots)
			{
				bool out = false;
				for(auto j: i.roots)
					//firstly cheching the roots of this field for a win
				{
					if(win(j.grid) == -sim)
						//if there is a win of
						//enemy then don't check this
						//move
					{
						out = true;
						break;
					}
				}

				if(out)	//if we found a win for our enemy
					//then we just don't making that move
				{
					continue;
				}

				if(sim == 1)	//or else we're searching
						//for a maximum winning
				{
					if(i.weight > m)
					{
						m = i.weight;
					}
				}
				else
				{
					if(i.weight < m)
					{
						m = i.weight;
					}
				}
			}

			vector<tree> temp;//good moves

			for(auto i: roots)
			{
				bool out = false;//the same, cutting off bad moves
				for(auto j: i.roots)
				{
					if(win(j.grid) == -sim)
					{
						out = true;
						break;
					}
				}
				if(out)
				{
					continue;
				}

				if(i.weight == m)	//remembering all
							//good moves
				{
					temp.push_back(i);
				}
			}

			if(temp.size() == 0)	//if there are no good moves
						//then makeing a random one
			{
				for(auto i: roots)
				{
					temp.push_back(i);
				}
			}

			srand(time(NULL));
			random_shuffle(temp.begin(), temp.end());//if there is mu;tiples moves then choose random one
			copy(field, temp[0].grid);
			
			return true;
		}
		else
		{
			if(freed(grid) == freed(field))
				//if field is not equal, but there was equal count of moves then it's wrong field
			{
				return false;
			}
			else
				//if not then search for right field in roots
			{
				for(auto i: roots)
				{
					if(i.search(field, sim == 1 ? 'X' : 'O'))
					{
						return true;
					}
				}
			}
		}
	}
	
	bool new_search(char field[3][3], char sim = 'X')
		//better working algorithm wich uses statistics of winds instead of just weight
	{
		sim = (sim == 'X') ? 1 : -1;
		if(compare(grid, field))
		{
			//if searching for 'X' then we searching the greatest weight
			//if searching for 'O' then search for smallest weight
			int m = (sim == 1) ? -1000000 : 1000000;
			for(auto i: roots)
			{
				bool out = false;
				for(auto j: i.roots)
				{
					if(win(j.grid) == -sim)
						//if there is a win of
						//enemy then don't check this
						//move
					{
						out = true;
						break;
					}
				}
				if(out)
				{
					continue;
				}
				if(sim == 1)
				{
					if(i.Xwin/i.total > m)
					{
						m = i.Xwin/i.total;
					}
				}
				else
				{
					if(i.Xwin/i.total < m)
					{
						m = i.Xwin/i.total;
					}
				}
			}
			vector<tree> temp;//good moves

			for(auto i: roots)
			{
				bool out = false;//the same, cutting off bad moves
				for(auto j: i.roots)
				{
					if(win(j.grid) == -sim)
					{
						out = true;
						break;
					}
				}
				if(out)
				{
					continue;
				}
				if(i.Xwin/i.total == m)
				{
					temp.push_back(i);//remembering all good moves
				}
			}
			if(temp.size() == 0)
			{
				for(auto i: roots)
				{
					temp.push_back(i);
				}
			}
			srand(time(NULL));
			random_shuffle(temp.begin(), temp.end());//if there is mu;tiples moves then choose random one
			copy(field, temp[0].grid);
			return true;
		}
		else
		{
			if(freed(grid) == freed(field))
				//if field is not equal, but there was equal count of moves then it's wrong field
			{
				return false;
			}
			else
				//if not then search for right field in roots
			{
				for(auto i: roots)
				{
					if(i.search(field, sim == 1 ? 'X' : 'O'))
					{
						return true;
					}
				}
			}
		}
	}
};

int main()
{
	char field[3][3];	//creating a field
	emptier(field);		//making it empy
	tree bot(field);	//creating a bot that will play
	int side;
	while(true)
	{
		system("clear");
		cout << "Who's play for X\n1.You\n2.Computer\nYour choise:";
		cin >> side;
		cin.ignore();	//cutting of the \n
		
		if(side == 1 || side == 2)
		{
			break;
		}
		else	//if the input was wrong ther repeat it
		{
			cout << "Wrong choise!";
			cin.get();
		}
	}

	side -= 1;	//making it comfortable for machine
	int turn = side;//choosing who gonna make a move first

	while(true)
	{
		if(turn == 0)
		{
			while(true)
			{
				drawField(field);	//outputting the field
				int x, y;
				cout << "Choose your grid:";	//reading the grid position
				cin >> x >> y;
				cin.ignore();
				if(field[x-1][y-1] == ' ')
				{
					field[x - 1][y - 1] = side == 0 ? 'X' : 'O';	//filling it
					break;
				}
				else	//if input was incorrect
				{
					cout << "not empty cell\n";
					cin.get();
					continue;
				}

				if(x > 3 || x < 0 || y < 0 || y > 3)
				{
					cout << "This cell doesn't exist\n";
					cin.get();
				}
			}

			drawField(field);//redrawing the field
			
			if(win(field) != 0)	//checking for a win
			{
				cout << "you won!\n";
				break;
			}
			if(freed(field) == 0)	//checking for a draw
			{
				cout << "Draw!" << endl;
				break;
			}
		}
		else	//giving to bot a move
		{
			bot.new_search(field, side == 0 ? '0' : 'X');
			drawField(field);
			if(win(field) != 0)
			{
				cout << "you lost\n";
				break;
			}
			if(freed(field) == 0)
			{
				cout << "Draw!" << endl;
				break;
			}
		}
		turn = (turn + 1) % 2; // changing move
	}
	return 0;
}

