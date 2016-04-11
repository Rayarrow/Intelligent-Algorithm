#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

struct city
{
	double x;
	double y;
};

#define N 14 // The number of cities, corresponds to the struct defined following.

struct city cities[N] = {
	{ 16.47, 96.10 },
	{ 16.47, 94.44 },
	{ 20.09, 92.54 },
	{ 22.39, 93.37 },
	{ 25.23, 97.24 },
	{ 22.00, 96.05 },
	{ 20.47, 97.02 },
	{ 17.20, 96.29 },
	{ 16.30, 97.38 },
	{ 14.05, 98.12 },
	{ 16.53, 97.38 },
	{ 21.52, 95.59 },
	{ 19.41, 97.13 },
	{ 20.09, 92.55 }
};

double D[N][N]; // Store the distance between any two cities.
double tInitial = 1000; // The initial newSolutionerature.
double tEnd = 0.01; // The ending newSolutionerature.
double coolingRate = 0.9;
int L = 200; // Times of the nested loop.

void initialize();
double getDistance(int solution[], int n); // Get the overall travelling distance along all cities provided the sequence (feasible solution).
void getNewSolution(int solution[], int n); // Randomly exchange two cities and get a new sequence (feasible solution).
void copyArrayElement(int solution[], int newSolution[], int n); // Copy the elements inside the array solution to newSolution.

int main()
{
	initialize();
	int solution[14]; // Keep track of the sequence after each nested loop.
	double distance; // Keep track of the distance after each nested loop.
	int count = 0; // Keep track of the iteration time.
	double t = tInitial; // The current newSolutionerature.

	for (int i = 0; i < N; ++i)
	{
		solution[i] = i;
	}

	while (t > tEnd)
	{
		printf("Iterating %d...\n", ++count);
		distance = getDistance(solution, N); // The distance for the default sequence (1->2->...->14).

		// Initialization for the next nested loop.
		int minSolution[14]; // Keep tracks of the optimal solution for a nested loop of L (200 as defined above) iterations.
		double minDistance = distance; // Keep track of the currently minimum distance in the nested loop.
		int newSolution[14]; // Store the newly generated random sequence (feasible solution) temporarily.
		double newDistance; // Corresponds to newSolution defined immediately above.
		copyArrayElement(solution, minSolution, N);
		for (int j = 0; j < L; ++j)
		{
			copyArrayElement(minSolution, newSolution, N);
			getNewSolution(newSolution, N);
			newDistance = getDistance(newSolution, N);

			if (newDistance < minDistance)
			{
				copyArrayElement(newSolution, minSolution, N);
				minDistance = newDistance;
			}

			else if (newDistance > distance && exp((distance - newDistance) / t) > (double)rand() / RAND_MAX)
			{
				copyArrayElement(newSolution, minSolution, N);
				minDistance = newDistance;
			}
		}

		t *= coolingRate; // Reduce the temperature, given the specified cooling rate.

		if (minDistance < distance)
		{
			distance = minDistance;
			copyArrayElement(minSolution, solution, N);
		}
		printf("The min distance: %lf\n", distance);

		for (int i = 0; i < N - 1; ++i)
		{
			printf("%d->", solution[i] + 1);
		}

		printf("%d\n", solution[N - 1] + 1);
	}

	return 0;
}

void initialize()
{
	srand(time(NULL)); // Set the rand seed, in case of identical optimal solutions. If reproduction is needed, then remove this line.
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			D[i][j] = sqrt(pow(cities[i].x - cities[j].x, 2) + pow(cities[i].y - cities[j].y, 2));
		}
	}
}

double getDistance(int solution[], int n)
{
	double result = 0;
	for (int i = 0; i < n - 1; ++i)
	{
		result += D[solution[i]][solution[i+1]];
	}
	return result;
}

void getNewSolution(int solution[], int n)
{
	int index1 = rand() % n;
	int index2 = rand() % n;
	int newSolution = solution[index1];
	solution[index1] = solution[index2];
	solution[index2] = newSolution;
}

void copyArrayElement(int solution[], int newSolution[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		newSolution[i] = solution[i];
	}
}