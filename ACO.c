#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

struct city
{
	double x;
	double y;
};

struct index
{
	int visited; // Once a city has been vistied, it won't be considered as a possibility when an ant's transitting to the next city.
	double cumP;
};

#define nrOfCities 14 // The number of cities, corresponds to the struct defined following.
#define nrOfAnts 5 // The number of ants.

struct city cities[nrOfCities] = {
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

double alpha = 1.0; // Pheromone indicator
double beta = 5.0; // Heuristic indicator
double rho = 0.1; // Pheromone volatilization indicator
double Q = 1.0; // Constant
int iteration = 10; // Maximum iteration
double D[nrOfCities][nrOfCities]; // Distance matrix, with rows as sources, and columns as destinations.
double eta[nrOfCities][nrOfCities]; // Heuristic matrix.
double tau[nrOfCities][nrOfCities]; // Pheromone matrix.
double deltaTau[nrOfCities][nrOfCities]; // A temporary matrix used to update pheromone matrix tau.
struct index routeIndex[nrOfCities];
int route[nrOfAnts][nrOfCities]; // The routes an ant has covered was recorded in a row.
double distance[nrOfAnts]; // ditto.

void initialize();
void iterationInitialize();
double getDistance(int route[], int n); // Get the overall travelling distance along all cities provided the sequence (feasible solution).
int transit(struct index routeIndex[], int source); // Determine the next destination given the current location of an ant.
double minIndex(double a[], int n); // Return the minimum value in the array a[].
void copyArrayElement(int solution[], int newSolution[], int n); // Copy the elements inside the array solution to newSolution.

int main()
{
	initialize();
	double minDistance = DBL_MAX; // The very largest allowed value for double type.
	for (int i = 0; i < iteration; ++i)
	{
		iterationInitialize();
		printf("Iterating %d ...\n", i + 1);

		for (int j = 0; j < nrOfAnts; ++j)
		{
			for (int iIndex = 0; iIndex < nrOfCities; ++iIndex)
			{
				routeIndex[iIndex].cumP = 0;
				routeIndex[iIndex].visited = 0;
			}

			int temp = rand() % nrOfCities; // Randomly choose a city as the very starting point for the ant j.
			route[j][0] = temp;
			routeIndex[temp].visited = 1;

			// Determine the next city given the current city, and a complete route covering all cities is built for the ant j afterwards.
			for (int k = 1; k < nrOfCities - 1; ++k)
			{
				route[j][k] = transit(routeIndex, route[j][k - 1]);
			}

			// Set the only unvisited city as the last city.
			for (int iIndex = 0; iIndex < nrOfCities; ++iIndex)
			{
				if (!routeIndex[iIndex].visited)
				{
					route[j][nrOfCities - 1] = iIndex;
				}
			}

			distance[j] = getDistance(route[j], nrOfCities);
		}

		// The following 2 double-loop update the pheromone matrix.
		for (int j = 0; j < nrOfAnts; ++j)
		{
			for (int k = 0; k < nrOfCities - 1; ++k)
			{
				deltaTau[route[j][k]][route[j][k + 1]] = Q / distance[j];
			}
		}

		for (int j = 0; j < nrOfCities; ++j)
		{
			for (int k = 0; k < nrOfCities; ++k)
			{
				tau[j][k] += (1 - rho) * tau[j][k] + deltaTau[j][k];
			}
		}

		int minIdx = minIndex(distance, nrOfAnts);
		int minRoute[nrOfCities];
		if (distance[minIdx] < minDistance)
		{
			minDistance = distance[minIdx];
			copyArrayElement(route[minIdx], minRoute, nrOfCities);
		}

		printf("The minimum distance: %lf\n", minDistance);
		printf("Corresponding route:\n");
		for (int i = 0; i < nrOfCities - 1; ++i)
		{
			printf("%d->", route[minIdx][i]+1);
		}
		printf("%d\n", route[minIdx][nrOfCities - 1]+1);
	}

	return 0;
}

void initialize()
{
	srand(time(NULL)); // Set the rand seed, in case of identical optimal solutions. If reproduction is needed, then remove this line.
	for (int i = 0; i < nrOfCities; ++i)
	{
		for (int j = 0; j < nrOfCities; ++j)
		{
			D[i][j] = sqrt(pow(cities[i].x - cities[j].x, 2) + pow(cities[i].y - cities[j].y, 2));
			eta[i][j] = 1.0 / D[i][j];
			tau[i][j] = 1.0;
		}
	}
}

void iterationInitialize()
{
	for (int i = 0; i < nrOfCities; ++i)
	{
		for (int j = 0; j < nrOfCities; ++j)
		{
			deltaTau[i][j] = 0.0;
		}
	}
}

double getDistance(int route[], int n)
{
	double result = 0;
	for (int i = 0; i < n - 1; ++i)
	{
		result += D[route[i]][route[i+1]];
	}
	return result;
}

int transit(struct index routeIndex[], int source)
{
	double cumulatedP = 0.0;
	for (int i = 0; i < nrOfCities; ++i)
	{
		if (!routeIndex[i].visited)
		{
			cumulatedP += pow(tau[source][i], alpha) * pow(eta[source][i], beta);
			routeIndex[i].cumP = cumulatedP;
		}
	}

	for (int i = 0; i < nrOfCities; ++i)
	{
		if (!routeIndex[i].visited)
		{
			routeIndex[i].cumP /= cumulatedP;
		}
	}

	double roulette = (double)rand() / RAND_MAX;

	for (int i = 0; i < nrOfCities; ++i)
	{
		if (!routeIndex[i].visited)
		{
			if (routeIndex[i].cumP >= roulette)
			{
				routeIndex[i].visited = 1;
				return i;
			}
		}
	}
}

double minIndex(double a[], int n)
{
	double minIdx = 0;
	double minValue = a[0];

	for (int i = 1; i < n; ++i)
	{
		if (a[i] < minValue)
		{
			minIdx = i;
			minValue = a[i];
		}
	}

	return minIdx;
}

void copyArrayElement(int solution[], int newSolution[], int n)
{
	for (int i = 0; i < n; ++i)
	{
		newSolution[i] = solution[i];
	}
}