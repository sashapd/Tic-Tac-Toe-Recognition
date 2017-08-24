#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
using namespace std;

vector<int> tttvector;

int whatturnisit(vector<int> a)
{
	int count = 0;
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] == 2)
			count++;
	}
	return a.size() - count;
}
int nextstepwincheck(int b, int c, int d, int a) {
	int winpossition = -1;
	if ((tttvector[d] * tttvector[b] * tttvector[c] == 18) && (a == 3)) {
		if (tttvector[b] == 2) {
			winpossition = b;
		}
		if (tttvector[c] == 2) {
			winpossition = c;
		}
		if (tttvector[d] == 2) {
			winpossition = d;
		}
	}
	if ((tttvector[d] * tttvector[b] * tttvector[c] == 50) && (a == 5)) {
		if (tttvector[b] == 2) {
			winpossition = b;
		}
		if (tttvector[c] == 2) {
			winpossition = c;
		}
		if (tttvector[d] == 2) {
			winpossition = d;
		}
	}
	return winpossition;
}
int gameovercheck(int b, int c, int d) {
	int winplayer = -1;
	if (tttvector[d] * tttvector[b] * tttvector[c] == 27) {
		winplayer = 3;
	}
	if (tttvector[d] * tttvector[b] * tttvector[c] == 125) {
		winplayer = 5;
	}
	return winplayer;
}
int nextstepwinposs(int a){
	int winpos = -1;
	winpos = nextstepwincheck(0, 1, 2, a);
	if (winpos < 0) {
	    winpos = nextstepwincheck(3, 4, 5, a);
	    if (winpos < 0) {
		 
			    winpos = nextstepwincheck(6, 7, 8, a);
			    if (winpos < 0) {
				    winpos = nextstepwincheck(0, 3, 6, a);
				    if (winpos < 0) {
					    winpos = nextstepwincheck(1, 4, 7, a);
				        if (winpos < 0) {
					        winpos = nextstepwincheck(2, 5, 8, a);
					        if (winpos < 0) {
						        winpos = nextstepwincheck(0, 4, 8, a);
						        if (winpos < 0) {
									winpos = nextstepwincheck(2, 4, 6, a);
								}
							}
						}
					}
				}
			
		}
	}
	return winpos;
}
// gameoverposs returns {-1,-1,-1,-1} if there is no win situation. If else
// it will return {a,b,c,d} where a,b,c is winner coordinates and d is 3 or 5
// which means X or O
vector<int> gameoverposs(vector<int> a) {
	vector <int> winvect = { -1,-1,-1 };
	int winner = -1;
	winner = gameovercheck(0, 1, 2);
	if (winner < 0) {
		winner = gameovercheck(3, 4, 5);
		if (winner < 0) {
			winner = gameovercheck(6, 7, 8);
			if (winner < 0) {
				winner = gameovercheck(0, 3, 6);
				if (winner < 0) {
					winner = gameovercheck(1, 4, 7);
					if (winner < 0) {
						winner = gameovercheck(2, 5, 8);
						if (winner < 0) {
							winner = gameovercheck(0, 4, 8);
							if (winner < 0) {
								winner = gameovercheck(2, 4, 6);
								if (winner > 0){
									winvect = { 2, 4, 6 };
								}
							}
							else {
								winvect = { 0, 4, 8 };
							}
						}
						else {
							winvect = { 2, 5, 8 };
						}
					}
					else {
						winvect = { 1, 4, 7 };
					}
				}
				else {
					winvect = { 0, 3, 6 };
				}
			}
			else {
				winvect = { 6, 7, 8 };
			}
		}
		else {
			winvect = { 3, 4, 5 };
		}
	}
	else {
		winvect = { 0, 1, 2 };
	}
	vector<int> retvect = { -1,-1,-1,-1 };
	if (winner > 0) {
		retvect[0] = winvect[0];
		retvect[1] = winvect[1];
		retvect[2] = winvect[2];
		retvect[3] = winner;
	}
	return retvect;
}
int Make2(vector<int> a) {
	if (a[4] == 2){
		return 4;
	}
	else {
		int k = rand() % 4;
		k = 2 * k + 1;
		return k;
	}
}

// Main function accepts the state-of-game in 1d vector with 9 elemnts
// It transforms it into next-step-state-of-game vector. It will pick up by  
// itself correct player (X or O) and do the step according 
// to best possible strategy
int main() {
	tttvector = { 2, 3, 2, 5, 3, 5, 2, 2, 3};
	for (int i=0; i<tttvector.size(); i++){
		cout << tttvector[i] << ",";
	}
	cout << endl;
	int b = whatturnisit(tttvector);
	cout << b << endl;
	if (b == 0) {
		tttvector[0] = 3;
	}
	if (b == 1) {
		if (tttvector[4] == 2) {
			tttvector[4] = 5;
		}
		else {
			tttvector[1] = 5;
		}
	}
	if (b == 2) {
		if (tttvector[8] == 2) {
			tttvector[8] = 3;
		}
		else {
			tttvector[2] = 3;
		}
	}
	if (b == 3) {
		if (nextstepwinposs(3) > 0) {
			tttvector[nextstepwinposs(3)] = 5;
		}
		else {
			tttvector[Make2(tttvector)] = 5;
		}
	}
	if (b == 4) {
		if (nextstepwinposs(3) > 0) {
			tttvector[nextstepwinposs(3)] = 3;
		}
		else {
			if (nextstepwinposs(5) > 0) {
				tttvector[nextstepwinposs(5)] = 3;
			}
			else {
				if (tttvector[6] == 2) {
					tttvector[6] = 3;
				}
				else {
					tttvector[2] = 3;
				}
			}
		}
	}
	if (b == 5) {
		if (nextstepwinposs(5) > 0) {
			tttvector[nextstepwinposs(5)] = 5;
		}
		else {
			if (nextstepwinposs(3) > 0) {
				tttvector[nextstepwinposs(3)] = 5;
   			}?
			else {
				tttvector[Make2(tttvector)] = 5;
			}
		}
	}
	if ((b == 6) || (b==8)) {
		if (nextstepwinposs(3) > 0) {
			tttvector[nextstepwinposs(3)] = 3;
		}
		else {
			if (nextstepwinposs(5) > 0) {
				tttvector[nextstepwinposs(5)] = 3;
			}
			else {
				for (int i = 0; i < tttvector.size(); i++) {
					if (tttvector[i] == 2) {
						tttvector[i] = 3;
						break;
					}
				}
			}
		}
	}
	if (b == 7) {
		if (nextstepwinposs(5) > 0) {
			tttvector[nextstepwinposs(5)] = 5;
		}
		else {
			if (nextstepwinposs(3) > 0) {
				tttvector[nextstepwinposs(3)] = 5;
			}
			else {
				for (int i = 0; i < tttvector.size(); i++) {
					if (tttvector[i] == 2) {
						tttvector[i] = 5;
						break;
					}
				}
			}
		}
	}
	for (int i = 0; i<tttvector.size(); i++) {
		cout << tttvector[i] << ",";
	}
    system("pause");
	return 0;
}
