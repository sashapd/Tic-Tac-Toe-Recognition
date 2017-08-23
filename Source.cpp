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
int threepositionscheck(int b, int c, int d, int a) {
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
int posswin(int a){
	int winpos = -1;
	winpos = threepositionscheck(0, 1, 2, a);
	if (winpos < 0) {
	    winpos = threepositionscheck(3, 4, 5, a);
	    if (winpos < 0) {
		   	winpos = threepositionscheck(3, 4, 5, a);
		    if (winpos < 0) {
			    winpos = threepositionscheck(6, 7, 8, a);
			    if (winpos < 0) {
				    winpos = threepositionscheck(0, 3, 6, a);
				    if (winpos < 0) {
					    winpos = threepositionscheck(1, 4, 7, a);
				        if (winpos < 0) {
					        winpos = threepositionscheck(2, 5, 8, a);
					        if (winpos < 0) {
						        winpos = threepositionscheck(0, 4, 8, a);
						        if (winpos < 0) {
									winpos = threepositionscheck(2, 4, 6, a);
								}
							}
						}
					}
				}
			}
		}
	}
	return winpos;
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


int main() {
	tttvector = { 3, 5, 3, 2, 5, 2, 2, 2, 2};
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
		if (posswin(3) > 0) {
			tttvector[posswin(3)] = 5;
		}
		else {
			tttvector[Make2(tttvector)] = 5;
		}
	}
	if (b == 4) {
		if (posswin(3) > 0) {
			tttvector[posswin(3)] = 3;
		}
		else {
			if (posswin(5) > 0) {
				tttvector[posswin(5)] = 3;
			}
			else {
				if (tttvector[6] == 2) {
					tttvector[6] == 3;
				}
				else {
					tttvector[2] == 3;
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
