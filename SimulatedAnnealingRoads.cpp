#include <iostream>
#include <windows.h>
using namespace std;

//Size of problem
const int N = 1000;
//Settings
const double MaxCoord = 1000.0;
//Problem subject
struct City
{
	int id;
	double x, y;
};
//Solution representation
struct Solution
{
	City cities[N + 1];
	double energy;
};

// Swapping elements in given Solution
void SwapRandom(Solution* s)
{
	//a,b - random indexes within (0,1...,N-1,0)
	int a = 1 + rand() % (N - 1);
	int b = a;
	while (a == b) { b = 1 + rand() % (N - 1); }
	//Swapping
	City temp = (*s).cities[a];
	(*s).cities[a] = (*s).cities[b];
	(*s).cities[b] = temp;
}
//Create coords for cities
void CreateCities(Solution* s)
{
	for (int i = 0; i < N; i++)
	{
		(*s).cities[i].id = i;
		(*s).cities[i].x = ((double)rand() / RAND_MAX) * MaxCoord;
		(*s).cities[i].y = ((double)rand() / RAND_MAX) * MaxCoord;
	}
	//First and last city are the same
	(*s).cities[N].id = 0;
	(*s).cities[N].x = (*s).cities[0].x;
	(*s).cities[N].y = (*s).cities[0].y;
}
//Shuffle given solution
void NewSolution(Solution* s)
{
	for (int i = 0; i < N - 1; i++) { SwapRandom(s); }
}
//Calculate energy of given solution
void CalcEnergy(Solution* s)
{
	double e = 0.0;
	double dx, dy;
	for (int i = 0; i < N; i++)
	{
		dx = (*s).cities[i].x - (*s).cities[i + 1].x;
		dy = (*s).cities[i].y - (*s).cities[i + 1].y;
		e += sqrt(dx * dx + dy * dy);
	}
	(*s).energy = e;
}
//Copy solution from ref to aim
void CopySolution(Solution ref, Solution* aim)
{
	(*aim).energy = ref.energy;
	for (int i = 0; i < N; i++)
	{
		(*aim).cities[i] = ref.cities[i];
	}
	(*aim).cities[N] = ref.cities[N];
}
//Print result vector of cities
void PrintSolution(Solution s)
{
	for (int i = 0; i < N + 1; i++)
	{
		cout << "ID [" << s.cities[i].id << "] x[" << s.cities[i].x << "] y[" << s.cities[i].y << "]" << endl;
	}
}

int main()
{
	system("chcp 1251>nul");
	srand(time(NULL));

	double
		Tstart = 100.0,
		Tcurrent = Tstart,
		Tend = 0.1,
		Talpha = 0.99;
	int IPT = 10000;
	double deltaE, Probability;
	bool New, Best;
	Solution SCurr, SWork, SBest;
	int accepted = 0;
	double genRand = 0.0;
	int randBorder = 1000000;

	Best = false;
	CreateCities(&SCurr);
	NewSolution(&SCurr);
	CalcEnergy(&SCurr);

	CopySolution(SCurr, &SWork);
	CopySolution(SCurr, &SBest);

	while (Tcurrent > Tend)
	{
		accepted = 0;
		for (int iter = 0; iter < IPT; iter++)
		{
			//Create new solution
			New = false;
			SwapRandom(&SWork);
			CalcEnergy(&SWork);

			//If new solution is better
			if (SWork.energy <= SCurr.energy) New = true;
			//Else check allowance criteria
			else
			{
				deltaE = SWork.energy - SCurr.energy;
				Probability = exp(-deltaE / Tcurrent);
				genRand = (rand() % randBorder) / (double)(randBorder);
				if (Probability > genRand)
				{
					accepted++;
					New = true;
				}
			}
			//If New solution was found
			if (New)
			{
				//Copying working solution to current
				New = false;
				CopySolution(SWork, &SCurr);

				//Check if it's best solution among others
				if (SCurr.energy < SBest.energy)
				{
					CopySolution(SCurr, &SBest);
					Best = true;
				}
			}
			else
			{
				CopySolution(SCurr, &SWork);
			}
		}
		cout << "T: "<< Tcurrent << "\tE: "<< SBest.energy << "\tA: "<< accepted << endl;
		Tcurrent *= Talpha;
	}
	if (Best) PrintSolution(SBest);

	system("pause>nul");
	return 0;
}