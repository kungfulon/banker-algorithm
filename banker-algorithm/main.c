#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int requestCanBeGranted(int process, int numOfResources, int *available, int **need, int *request)
{
	for (int i = 0; i < numOfResources; ++i)
		if (available[i] < request[i] || need[process][i] < request[i])
			return 0;

	return 1;
}

int isEnoughResourcesAvailable(int process, int numOfResources, int *available, int **need)
{
	for (int i = 0; i < numOfResources; ++i)
		if (available[i] < need[process][i])
			return 0;

	return 1;
}

int tryBanker(int numOfProcesses, int numOfResources, int *available, int **allocated, int **need, int *terminated)
{
	for (int i = 0; i < numOfProcesses; ++i)
	{
		if (terminated[i])
			continue;

		if (!isEnoughResourcesAvailable(i, numOfResources, available, need))
			continue;

		for (int j = 0; j < numOfResources; ++j)
			available[j] = available[j] + allocated[i][j];

		return i;
	}

	return -1;
}

int isSafeState(int numOfProcesses, int numOfResources, int *available, int **allocated, int **need, int *safeState)
{
	int numTerminated = 0;
	int *terminated = (int *)malloc(numOfProcesses * sizeof(int));
	memset(terminated, 0, numOfProcesses * sizeof(int));

	while (numTerminated < numOfProcesses)
	{
		int process = tryBanker(numOfProcesses, numOfResources, available, allocated, need, terminated);

		if (process == -1)
			break;

		terminated[process] = 1;
		safeState[numTerminated++] = process;
	}

	free(terminated);

	return (numTerminated == numOfProcesses);
}

int main()
{
	int numOfProcesses, numOfResources;
	int *available, *request, *reqAvailable, *safeState;
	int **allocated, **maximum, **need, **reqAllocated, **reqNeed;
	
	scanf("%d%d", &numOfProcesses, &numOfResources);

	safeState = (int *)malloc(numOfProcesses * sizeof(int));
	available = (int *)malloc(numOfResources * sizeof(int));
	request = (int *)malloc(numOfResources * sizeof(int));
	allocated = (int **)malloc(numOfProcesses * sizeof(int *));
	maximum = (int **)malloc(numOfProcesses * sizeof(int *));
	need = (int **)malloc(numOfProcesses * sizeof(int *));
	reqAvailable = (int *)malloc(numOfResources * sizeof(int));
	reqAllocated = (int **)malloc(numOfProcesses * sizeof(int *));
	reqNeed = (int **)malloc(numOfProcesses * sizeof(int *));

	for (int i = 0; i < numOfResources; ++i)
		scanf("%d", available + i);

	for (int i = 0; i < numOfProcesses; ++i)
	{
		maximum[i] = (int *)malloc(numOfResources * sizeof(int));
		
		for (int j = 0; j < numOfResources; ++j)
			scanf("%d", maximum[i] + j);
	}

	for (int i = 0; i < numOfProcesses; ++i)
	{
		allocated[i] = (int *)malloc(numOfResources * sizeof(int));
		need[i] = (int *)malloc(numOfResources * sizeof(int));
		reqAllocated[i] = (int *)malloc(numOfResources * sizeof(int));
		reqNeed[i] = (int *)malloc(numOfResources * sizeof(int));

		for (int j = 0; j < numOfResources; ++j)
		{
			scanf("%d", allocated[i] + j);
			need[i][j] = maximum[i][j] - allocated[i][j];
		}
	}

	while (1)
	{
		int numOfRequests, process;

		scanf("%d", &numOfRequests);

		if (!numOfRequests)
			break;

		for (int req = 0; req < numOfRequests; ++req)
		{
			scanf("%d", &process);

			for (int i = 0; i < numOfResources; ++i)
				scanf("%d", request + i);

			printf("Request #%d: ", req + 1);

			if (!requestCanBeGranted(process, numOfResources, available, need, request))
			{
				printf("Overneeded or not enough resources\n");

				continue;
			}

			for (int i = 0; i < numOfProcesses; ++i)
				for (int j = 0; j < numOfResources; ++j)
				{
					reqAllocated[i][j] = allocated[i][j];
					reqNeed[i][j] = need[i][j];
				}

			for (int i = 0; i < numOfResources; ++i)
			{
				reqAvailable[i] = available[i] - request[i];
				reqAllocated[process][i] += request[i];
				reqNeed[process][i] -= request[i];
			}

			if (isSafeState(numOfProcesses, numOfResources, reqAvailable, reqAllocated, reqNeed, safeState))
			{
				for (int i = 0; i < numOfProcesses; ++i)
					printf("%d ", safeState[i]);
				
				printf("\n");
			}
			else
				printf("Unsafe\n");
		}

		printf("\n");
	}
}
