// Soubor:      proj2.c
// Projekt:     Projekt 2 - Sprava procesu
// Predmet:     Operacni systemy (IOS)
// Autor:       Vladimir Dusek, xdusek27
// Skola:       VUT FIT 1BIT
// Datum:       18/4/2017
// Prekladac:   gcc 5.4.0
// Spusteni:    $ ./proj2 A C AGT CGT AWT CWT
//     A je počet procesů adult; A > 0
//     C je počet procesů child; C > 0
//     AGT je maximální hodnota doby (v ms), po které je generován nový proces adult; AGT >= 0 && AGT < 5001
//     CGT je maximální hodnota doby (v ms), po které je generován nový proces child; CGT >= 0 && CGT < 5001
//     AWT je maximální hodnota doby (v ms), po kterou proces adult simuluje činnost v centru; AWT >= 0 && AWT < 5001
//     CWT je maximální hodnota doby (v ms), po kterou proces child simuluje činnost v centru; CWT >= 0 && CWT < 5001
//     Všechny parametry jsou celá čísla
 

#include "proj2.h"


/**********************************************/
/* Deklarace globalnich promennych a semaforu */
/**********************************************/

/* Identifikatory adult a child procesu (nejsou sdilene promenne) */
unsigned int adult_counter = 1,
             child_counter = 1;

/* Sdilene promenne - ID */
unsigned int identifier_id, // A == poradove cislo provadene akce
             adults_in_center_id, // CA == soucasny pocet adult v centru
             children_in_center_id, // CC == soucasny pocet child v centru
             pcounter_id, // pocet procesu celkem (child + adult)
             adults_coming_id, // kolik jeste prijde adults (kvuli zablokovani aplikace)
             wait_come_id, // pocet child, ktery cekaji na vstup do centra
             wait_leave_id, // pocet adult, ktery cekaji na opusteni centra
             no_adult_id; // true == uz zadny adult neprijde, kvuli zablokovani aplikace 

/* Sdilene promenne - ukazatele */
unsigned int *identifier, 
             *adults_in_center, 
             *children_in_center, 
             *pcounter, 
             *adults_coming, 
             *wait_come, 
             *wait_leave;
bool *no_adult;

/* Semafory */
sem_t *sem_mutex, // mutex
      *sem_gena, // aby hlavni proces skoncil az po procesu ktery generuje adult
      *sem_genc, // aby hlavni proces skoncil az po procesu ktery generuje child
      *sem_inside, // aby vsechny podpodprocesy (adults a children) skoncily nakonec a soucasne
      *sem_child_queue, // fronta pro child, kdyz vstupuji
      *sem_adult_queue, // fronta pro adult, kdyz odchazeji
      *sem_wait_child, // aby adult ktery vstoupi, pockal na child ktery jdou hned za nim
      *sem_wait_adult; // aby child ktery odchazi, pockal na adult ktery jdou hned za nim

/* Zapisovani do souboru */
FILE *fw;
       

/**************************************************/
/* Testovani zadani spravnosti argumentu programu */
/**************************************************/
config_t test_args(int argc, char *argv[])
{
	config_t config;
	char *p_error;
	long test;
	bool error = false;

	/* Neplatny pocet argumentu ---> Ukoncit a vypsat chybove hlaseni */
	if (argc != 7)
	{
		fprintf(stderr, "Chyba! Nespravne spusteni programu, chybny pocet argumentu.\n");
		exit(1);
	}

	/* Argument A ---> A > 0 && A < MAX_UINT */
	test = strtol(argv[1], &p_error, 10);
	if (test < 0 || test > UINT_MAX)
		error = true;
	config.A = (unsigned int) test;
	if (!(config.A > 0 && *p_error == '\0'))
		error = true;

	/* Argument C ---> C > 0 && C < MAX_UINT */
	test = strtol(argv[2], &p_error, 10);
	if (test < 0 || test > UINT_MAX)
		error = true;
	config.C = (unsigned int) test;
	if (!(config.C > 0 && *p_error == '\0'))
		error = true;

	/* Argument AGT ---> AGT >= 0 && AGT <= 5000 */
	test = strtol(argv[3], &p_error, 10);
	if (test < 0 || test > UINT_MAX)
		error = true;
	config.AGT = (unsigned int) test;
	if (!(config.AGT <= 5000 && *p_error == '\0'))
		error = true;

	/* Argument CGT ---> CGT >= 0 && CGT <= 5000 */
	test = strtoul(argv[4], &p_error, 10);
	if (test < 0 || test > UINT_MAX)
		error = true;
	config.CGT = (unsigned int) test;
	if (!(config.CGT <= 5000 && *p_error == '\0'))
		error = true;

	/* Argument AWT ---> AWT >= 0 && AWT <= 5000 */
	test = strtoul(argv[5], &p_error, 10);
	if (test < 0 || test > UINT_MAX)
		error = true;
	config.AWT = (unsigned int) test;
	if (!(config.AWT <= 5000 && *p_error == '\0'))
		error = true;

	/* Argument CWT ---> CWT >= 0 && CWT <= 5000 */
	test = strtoul(argv[6], &p_error, 10);
	if (test < 0 || test > UINT_MAX)
		error = true;
	config.CWT = (unsigned int) test;
	if (!(config.CWT <= 5000 && *p_error == '\0'))
		error = true;

	/* Nektery z argumentu mimo meze ---> Ukoncit a vypsat chybove hlaseni */
	if (error)
	{
		fprintf(stderr, "Chyba! Nespravne spusteni programu, neplatne hodnoty argumentu.\n");
		exit(1);
	}

	return config;
}


/*****************************************************************/
/* Vrati pseudonahodne cislo (unsigned int) z intervalu <0; max> */
/*****************************************************************/
unsigned int ran_num(unsigned int max)
{
	if (max == 0)
		return 0;
	srand((unsigned int) time(NULL)); 
	return rand() % max; 
}


/**********************************************************************/
/* Nastavi zdroje - sdilenou pamet, semafory a otevre vystupni soubor */
/**********************************************************************/
void set_resources(unsigned int n, unsigned int A)
{
	bool error = false;
	long test;

	/* Otevreni souboru pro zapisovani dat */
	if ((fw = fopen(NAME, "w")) == NULL) 
	{
		fprintf(stderr, "Chyba! Vystupni soubor %s se nepodarilo otevrit.\n", NAME);
		exit(2);
	}

	/*************************************/
	/* Alokace sdilenych segmentu pameti */
	/*************************************/
	if ((test = shmget(IPC_PRIVATE, sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
		error = true;
	identifier_id = (unsigned int) test;

	if ((test = shmget(IPC_PRIVATE, sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
		error = true;
	adults_in_center_id = (unsigned int) test;

	if ((test = shmget(IPC_PRIVATE, sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
		error = true;
	children_in_center_id = (unsigned int) test;

	if ((test = shmget(IPC_PRIVATE, sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
		error = true;
	pcounter_id = (unsigned int) test;

	if ((test = shmget(IPC_PRIVATE, sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
		error = true;
	adults_coming_id = (unsigned int) test;

	if ((test = shmget(IPC_PRIVATE, sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
		error = true;
	wait_come_id = (unsigned int) test;

	if ((test = shmget(IPC_PRIVATE, sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
		error = true;
	wait_leave_id = (unsigned int) test;

	if ((test = shmget(IPC_PRIVATE, sizeof(bool), IPC_CREAT | 0666)) < 0)
		error = true;
	no_adult_id = (unsigned int) test;

	/************************************************/
	/* Ziskani ukazatelu na sdilene segmenty pameti */
	/************************************************/
	if ((identifier = (unsigned int *) shmat(identifier_id, NULL, 0)) == NULL)
		error = true;

	if ((adults_in_center = (unsigned int *) shmat(adults_in_center_id, NULL, 0)) == NULL)
		error = true;

	if ((children_in_center = (unsigned int *) shmat(children_in_center_id, NULL, 0)) == NULL)
		error = true;

	if ((pcounter = (unsigned int *) shmat(pcounter_id, NULL, 0)) == NULL)
		error = true;

	if ((adults_coming = (unsigned int *) shmat(adults_coming_id, NULL, 0)) == NULL)
		error = true;

	if ((wait_come = (unsigned int *) shmat(wait_come_id, NULL, 0)) == NULL)
		error = true;

	if ((wait_leave = (unsigned int *) shmat(wait_leave_id, NULL, 0)) == NULL)
		error = true;

	if ((no_adult = (bool *) shmat(no_adult_id, NULL, 0)) == NULL)
		error = true;

    /*******************************************/
	/* Inicializace hodnot sdilenych promenych */
	/*******************************************/
	*identifier = 1;
	*adults_in_center = 0;
	*children_in_center = 0;
	*pcounter = n;
	*adults_coming = A;
	*wait_come = 0;
	*wait_leave = 0;
	*no_adult = false;

	/************************************/
	/* Vytvoreni pojmenovanych semaforu */
	/************************************/
	if ((sem_mutex = sem_open("xdusek27_mutex", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) 
    	error = true;

	if ((sem_gena = sem_open("xdusek27_gena", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) 
    	error = true;

	if ((sem_genc = sem_open("xdusek27_genc", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) 
    	error = true;

	if ((sem_inside = sem_open("xdusek27_inside", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) 
    	error = true;

    if ((sem_child_queue = sem_open("xdusek27_child_queue", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) 
    	error = true;
	
	if ((sem_adult_queue = sem_open("xdusek27_adult_queue", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) 
		error = true;
	
	if ((sem_wait_child = sem_open("xdusek27_wait_adult", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) 
		error = true;
	
	if ((sem_wait_adult = sem_open("xdusek27_wait_child", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED) 
		error = true;

	/* Kontrola jestli vse probehlo spravne, pripadne ukonceni s chybou */
	if (error)
	{
		fprintf(stderr, "Chyba! Nepodarilo se vytvorit semafory nebo alokovat sdilenou pamet.\n");
		clean_resources();
		exit(2);
	}
}


/********************************************************************/
/* Uvolni zdroje - sdilenou pamet, semafory a zavre vystupni soubor */
/********************************************************************/
void clean_resources()
{
	bool error = false;

	/**********************************************/
	/* Zlikvidovani ukazatelu na sdilene segmenty */
	/**********************************************/
	if (shmdt(identifier) == -1)
		error = true;

	if (shmdt(adults_in_center) == -1)
		error = true;

	if (shmdt(children_in_center) == -1)
		error = true;

	if (shmdt(pcounter) == -1)
		error = true;

	if (shmdt(adults_coming) == -1)
		error = true;

	if (shmdt(wait_come) == -1)
		error = true;

	if (shmdt(wait_leave) == -1)
		error = true;

	if (shmdt(no_adult) == -1)
		error = true;

	/**************************************/
	/* Uvolneni sdilenych segmentu pameti */
	/**************************************/
	if (shmctl(identifier_id, IPC_RMID, NULL) == -1)
		error = true;

	if (shmctl(adults_in_center_id, IPC_RMID, NULL) == -1)
		error = true;

	if (shmctl(children_in_center_id, IPC_RMID, NULL) == -1)
		error = true;

	if (shmctl(pcounter_id, IPC_RMID, NULL) == -1)
		error = true;

	if (shmctl(adults_coming_id, IPC_RMID, NULL) == -1)
		error = true;

	if (shmctl(wait_come_id, IPC_RMID, NULL) == -1)
		error = true;

	if (shmctl(wait_leave_id, IPC_RMID, NULL) == -1)
		error = true;

	if (shmctl(no_adult_id, IPC_RMID, NULL) == -1)
		error = true;

	/**********************************/
	/* Zavreni pojmenovanych semaforu */
	/**********************************/
	sem_close(sem_mutex);
	sem_close(sem_gena);
	sem_close(sem_genc);
	sem_close(sem_inside);
	sem_close(sem_child_queue);
	sem_close(sem_adult_queue);
	sem_close(sem_wait_child);
	sem_close(sem_wait_adult);

	/**********************************/
	/* Smazani pojmenovanych semaforu */
	/**********************************/
    sem_unlink("xdusek27_mutex");
    sem_unlink("xdusek27_mutex2");
    sem_unlink("xdusek27_gena");
    sem_unlink("xdusek27_genc");
    sem_unlink("xdusek27_inside");
    sem_unlink("xdusek27_child_queue");
    sem_unlink("xdusek27_adult_queue");
    sem_unlink("xdusek27_wait_adult");
    sem_unlink("xdusek27_wait_child");

    /* Zavreni vystupniho souboru */
    if (fclose(fw) == EOF)
	{
		fprintf(stderr, "Chyba! Vystupni soubor %s se nepodarilo zavrit.\n", NAME);
		exit(2);
	}

	/* Kontrola jestli vse probehlo spravne, kdyz ne, ukonceni s chybou */
	if (error)
	{
		fprintf(stderr, "Chyba! Nepodarilo se uvolnit sdilenou pamet nebo semafory.\n");
		exit(2);
	}
}


/*******************************/
/* Proces reprezentujici adult */
/*******************************/
void adult(config_t config)
{
	sem_t *sem_inside = sem_open("xdusek27_inside", 0);

	/* adult se spustil */
	sem_wait(sem_mutex);
		fprintf(fw, "%-8u: A %-4u: started\n", (*identifier)++, adult_counter);
	sem_post(sem_mutex);

	/* adult vstoupi do centra, nic mu nebrani */
	sem_wait(sem_mutex);
		fprintf(fw, "%-8u: A %-4u: enter\n", (*identifier)++, adult_counter);
		(*adults_in_center)++;
		if (*wait_come)
		{
			int n = (3 < (*wait_come)) ? 3 : (*wait_come);
			for (int i = 0; i < n; i++)
				sem_post(sem_child_queue);
			(*wait_come) -= n;
			(*children_in_center) += n;
			sem_post(sem_mutex);
			for (int i = 0; i < n; i++)
				sem_wait(sem_wait_child);
		}
		//else if (*wait_leave)
		//{
			// Neni v zadani
		//}
		else
			sem_post(sem_mutex);

	/* adult je v centru nahodnou dobu z <0; AWT> ms */
	usleep(ran_num(config.AWT) * 1000);

	/* adult se pokusi odejit */
	sem_wait(sem_mutex);
		fprintf(fw, "%-8u: A %-4u: trying to leave\n", (*identifier)++, adult_counter);
		if (*children_in_center <= 3 * ((*adults_in_center) - 1))
		{
			fprintf(fw, "%-8u: A %-4u: leave\n", (*identifier)++, adult_counter);
			(*adults_in_center)--;
			sem_post(sem_mutex);
		}
		else
		{
			fprintf(fw, "%-8u: A %-4u: waiting : %-4u: %-4u\n", (*identifier)++, adult_counter, 
				*adults_in_center, *children_in_center);
			(*wait_leave)++;
			sem_post(sem_mutex);

			sem_wait(sem_adult_queue);
				fprintf(fw, "%-8u: A %-4u: leave\n", (*identifier)++, adult_counter);
			sem_post(sem_mutex);
			sem_post(sem_wait_adult);
		}

	/* vsechny procesy adult a child se ukonci az nakonec a zaroven */
	sem_wait(sem_mutex);
		(*pcounter)--;
		(*adults_coming)--;
	sem_post(sem_mutex);

	/* zadny adult uz neprijde - aby se aplikace nezablokovala */
	if (*adults_coming == 0)
	{
		sem_wait(sem_mutex);
			*no_adult = true;
			for (unsigned int i = 0; i < config.C; i++)
			{
				sem_post(sem_child_queue);
				sem_post(sem_wait_adult);
			}
		sem_post(sem_mutex);
	}

	/* odstartuje ukonceni vsech procesu soucasne */
	if (*pcounter == 0)
		sem_post(sem_inside);
	sem_wait(sem_inside);
	sem_post(sem_inside);

	/* adult se ukoncil */
	sem_wait(sem_mutex);
		fprintf(fw, "%-8u: A %-4u: finished\n", (*identifier)++, adult_counter);
	sem_post(sem_mutex);

	exit(0);
}


/*******************************/
/* Proces reprezentujici child */
/*******************************/
void child(config_t config)
{
	sem_t *sem_inside = sem_open("xdusek27_inside", 0);

	/* child se spustil */
	sem_wait(sem_mutex);
		fprintf(fw, "%-8u: C %-4u: started\n", (*identifier)++, child_counter);
	sem_post(sem_mutex);

	/* child se pokusi vstoupit do centra */
	sem_wait(sem_mutex);
		if ((*children_in_center < 3 * (*adults_in_center)) || *no_adult)
		{
			fprintf(fw, "%-8u: C %-4u: enter\n", (*identifier)++, child_counter);
			(*children_in_center)++;
			sem_post(sem_mutex);
		}
		else
		{
			fprintf(fw, "%-8u: C %-4u: waiting : %-4u: %-4u\n", (*identifier)++, child_counter, 
				*adults_in_center, *children_in_center);
			(*wait_come)++;
			sem_post(sem_mutex);
			sem_wait(sem_child_queue);

			sem_wait(sem_mutex);
				fprintf(fw, "%-8u: C %-4u: enter\n", (*identifier)++, child_counter);
				sem_post(sem_wait_child);
			sem_post(sem_mutex);
		}

	/* child je v centru nahodnou dobu z <0; CWT> ms */
	usleep(ran_num(config.CWT) * 1000);

	/* child odejde, nic mu nebrani */
	sem_wait(sem_mutex);
		fprintf(fw, "%-8u: C %-4u: trying to leave\n", (*identifier)++, child_counter);
		fprintf(fw, "%-8u: C %-4u: leave\n", (*identifier)++, child_counter);
		(*children_in_center)--;

		if (((*children_in_center) <= 3 * ((*adults_in_center) - 1)) && *wait_leave)
		{
			(*wait_leave)--;
			(*adults_in_center)--;
			sem_post(sem_adult_queue);
			sem_wait(sem_wait_adult);
		}
		//else if (*wait_come)
		//{
			// Neni v zadani
		//}
		else
			sem_post(sem_mutex);

	/* vsechny procesy adult a child se ukonci az nakonec a zaroven */
	sem_wait(sem_mutex);
		(*pcounter)--;
	sem_post(sem_mutex);

	/* odstartuje ukonceni vsech procesu soucasne */
	if (*pcounter == 0)
		sem_post(sem_inside);
	sem_wait(sem_inside);
	sem_post(sem_inside);
 
	/* child se ukoncil */
	sem_wait(sem_mutex);
		fprintf(fw, "%-8u: C %-4u: finished\n", (*identifier)++, child_counter);
	sem_post(sem_mutex);

	exit(0);
}


/*********/
/* CLEAN */
/********
void somethingswrong()
{
    sem_close(sem_mutex);
    sem_unlink("xdusek27_mutex");
    sem_close(sem_gena);
    sem_unlink("xdusek27_gena");
    sem_close(sem_genc);
    sem_unlink("xdusek27_genc");
    sem_close(sem_inside);
    sem_unlink("xdusek27_inside");
    sem_close(sem_adult_queue);
    sem_unlink("xdusek27_adult_queue");
    sem_close(sem_child_queue);
    sem_unlink("xdusek27_child_queue");
    sem_close(sem_wait_child);
    sem_unlink("xdusek27_wait_adult");
    sem_close(sem_wait_adult);
    sem_unlink("xdusek27_wait_child");

    exit(0);
}
*/


/********/
/* MAIN */
/********/
int main(int argc, char *argv[])
{	
    //somethingswrong();

	/* zpracovani argumentu */
	config_t config = test_args(argc, argv);

	/* vytvoreni sdilene pameti a semaforu */
	set_resources(config.C + config.A, config.A);

	/* vycisteni vystupu */
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	setbuf(fw, NULL);

	int wait_ida = 0;
	pid_t process_id = fork();

	/* Chyba pri fork() */
	if (process_id < 0)
	{
		fprintf(stderr, "Chyba! Nepodaril se fork.\n");
		clean_resources();
		return 2;
	}
	/* Pomocny proces, ktery se bude dale vetvit */
	else if (process_id == 0)
	{
		process_id = fork();

		/* Chyba pri fork() */
		if (process_id < 0)
		{
			fprintf(stderr, "Chyba! Nepodaril se fork.\n");
			clean_resources();
			return 2;
		}
		/* Pomocny proces, ktery generuje child procesy */
		else if (process_id == 0)
		{
			sem_wait(sem_genc);
			/* cyklem vygeneruje prave C procesu reprezentujicich children */
			for (unsigned int i = 0; i < config.C; i++)
			{
				usleep(ran_num(config.CGT) * 1000);
				process_id = fork();
				
				if (process_id < 0)
				{
					fprintf(stderr, "Chyba! Nepodaril se fork.\n");
					clean_resources();
					return 2;
				}
				else if (process_id == 0)
				{
					child(config);
					break;
				}
				else 
					child_counter++;
			}

			for (unsigned int i = 0; i < config.C; i++)
				wait(NULL);

			sem_post(sem_genc);
			return 0;
		}
		/* Pomocny proces, ktery generuje adult procesy */
		else
		{
			sem_wait(sem_gena);
			wait_ida = process_id;
			/* cyklem vygeneruje prave A procesu reprezentujicich adults */
			for (unsigned int i = 0; i < config.A; i++)
			{
				usleep(ran_num(config.AGT) * 1000);
				process_id = fork();
				
				if (process_id < 0)
				{
					fprintf(stderr, "Chyba! Nepodaril se fork.\n");
					clean_resources();
					return 2;
				}
				else if (process_id == 0)
				{
					adult(config);
					break;
				}
				else
					adult_counter++;
			}

			for (unsigned int i = 0; i < config.A; i++)
				wait(NULL);

			sem_post(sem_gena);
			return 0;
		}
	}
	/* Hlavni proces, ceka na ukonceni vsech a pak skonci */
	else
	{
		waitpid(wait_ida, NULL, 0);
		sem_wait(sem_gena);
		sem_wait(sem_genc);
	}

	clean_resources();
	return 0;
}
