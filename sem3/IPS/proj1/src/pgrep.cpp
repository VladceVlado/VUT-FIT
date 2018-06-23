/*
* Author:      Vladimir Dusek, xdusek27
* Project:     IPS - proj1, parallel grep
* University:  VUT FIT 2BIT
* Date:        13/10/2017
* Description: $ ./pgrep MIN_SCORE RE1 SC1 [ RE2 SC2 ] [ RE3 SC3 ] ...
*/


#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>
#include <iostream>
#include <string.h>
#include <regex>


/* Arrays of locks */
std::vector<std::mutex *> locks;
std::vector<std::mutex *> locks_main;
std::mutex lock_mutex;

/* Global variables */
char *line; 
char **regex; 
int *score; 
int final_score = 0;
bool cont = true;


/*
 * Read line from stdin
 */
char *read_line(int *res) 
{
	std::string line;
	char *str;
	if (std::getline(std::cin, line)) {
		str = (char *) malloc(sizeof(char) * (line.length() + 1));
		strcpy(str, line.c_str());
		*res = 1;
		return str;
	} 
	else {
		*res = 0;
		return NULL;
	}
}

/*
 * Function for threads
 */
void function(int ID) 
{
	(*(locks[ID])).lock();
	
	while (cont) {
		if (std::regex_match(line, std::regex(regex[ID]))) {
			lock_mutex.lock();
			final_score += score[ID];
			lock_mutex.unlock();
		}
		(*(locks_main[ID])).unlock();
		(*(locks[ID])).lock();
	}
}

/*
 * Main
 */
int main(int argc, char *argv[]) 
{
	/* Test if valid number of arguments */
	if (argc < 4 || argc % 2 != 0) {
		fprintf(stderr, "%s\n%s\n", "error: invalid run of the program",
			"./pgrep MIN_SCORE RE1 SC1 [ RE2 SC2 ] [ RE3 SC3 ] ...");
		return 1;
	}

	/* Get min score */
	char *p_error;
	int min_score = (int) strtol(argv[1], &p_error, 10);
	if (*p_error != '\0') {
		fprintf(stderr, "%s\n%s\n", "error: invalid run of the program", 
			"MIN_SCORE has to be an integer");
		return 2;
	}

	/* Calculate number of threads and locks */
	int num = (argc - 2) / 2;

	/* Allocation memory for array of regexs and array for score */
	score = (int *) malloc(num * sizeof(int));
	regex = (char **) malloc(num * sizeof(char *));
	if (score == NULL || regex == NULL) {
		if (score != NULL)
			free(score);
		fprintf(stderr, "%s\n", "error: not enough memory for allocation");
		return 3;
	}

	/* Initialization of arrays */
	int i, j;
	for (i = 3, j = 0; i <= argc; i += 2, j++) {
		score[j] = (int) strtol(argv[i], &p_error, 10);
		regex[j] = argv[i - 1];
		if (*p_error != '\0') {
			fprintf(stderr, "%s\n%s\n", "error: invalid run of the program", 
				"all SCs have to be an integers");
			free(score);
			free(regex);
			return 2;
		}
	}


	/***************************************
	 * Initialization of threads and locks *
	 ***************************************/

	/* Vector of threads */
	std::vector <std::thread *> threads;

	/* Create locks */
	locks.resize(num);
	locks_main.resize(num);

	for (i = 0; i < num; i++) {	
	 	std::mutex *new_lock = new std::mutex();
	 	locks[i] = new_lock;
	 	(*(locks[i])).lock();

	 	new_lock = new std::mutex();
	 	locks_main[i] = new_lock;
	 	(*(locks_main[i])).lock();
	}

	/* Create threads */
	threads.resize(num);
	for (i = 0; i < num; i++) {	
		std::thread *new_thread = new std::thread(function, i);
		threads[i] = new_thread;
	}


	/*********************
	 * PGREP CALCULATION *
	 *********************/
	int res;
	line = read_line(&res);
	while (res) {
		for (i = 0; i < num; i++)
			(*(locks[i])).unlock();
		
		for (i = 0; i < num; i++)
			(*(locks_main[i])).lock();

		if (final_score >= min_score)
			printf("%s\n", line);

		final_score = 0;
		free(line);
		line = read_line(&res);
	}
	cont = false;
	for (i = 0; i < num; i++)
		(*(locks[i])).unlock();


	/*******************
	 * FREE ALL MEMORY *
	 *******************/

	/* Memory after threads */
	for (i = 0; i < num; i++) {
		(*(threads[i])).join();
		delete threads[i];
	}

	/* Memory after locks */
	for (i = 0; i < num; i++) {
	 	delete locks[i];
	 	delete locks_main[i];
	}

	/* Memory after dynamic allocation */
	free(score);
	free(regex);


	return 0;
}
