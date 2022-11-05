#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int players = 0;
int counter_capital = 0;
int counter_titan = 0;
int capital_score = 0;
int titan_score = 0;
int maxscore = 0;
int match = 0;
int maxscore_match = 0;
int individual_score = 0;
int titanswin = 0;
int capitalswin = 0;
int tied = 0;
int permatchscore = 0;
sem_t barrier;
sem_t mut;

void *bat(void *playerid)
{

    sem_wait(&barrier);
    sem_wait(&mut);

    int run = 0;
    if (counter_capital < 11)
    {
        counter_capital++;
        run = rand() % 101;
        capital_score += run;
        // printf("Capital with Jersey No. %d Score %d runs\n", *(int*)playerid, run);
        if (run > individual_score)
            individual_score = run;

        if (run > permatchscore)
            permatchscore = run;
    }
    else
    {
        counter_titan++;
        run = rand() % 101;
        titan_score += run;
        // printf("Titan with Jersey No. %d Score %d runs \n", *(int*)playerid, run);
        if (run > individual_score)
        {
            individual_score = run;
        }
        if (run > permatchscore)
        {
            permatchscore = run;
        }
    }
    sem_post(&mut);
    sem_post(&barrier);
}

void *startGame(void *match)
{
    // Start of the Game all variables reset
    permatchscore = 0;
    counter_capital = 0;
    counter_titan = 0;
    capital_score = 0;
    titan_score = 0;

    pthread_t players[22];
    int i;
    for (i = 0; i < 22; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if(pthread_create(players + i, NULL, &bat, a)!=0){
            printf("Failed to Create Player Thread \n");
        }
    }

    for (i = 0; i < 22; i++)
    {
        pthread_join(players[i], NULL);
    }

    //  Match Summary

    printf("---------------------MATCH : ( %d ) Summary------------------\n\n", (*(int*)match)+1);
    printf("SCORE: Capitals : %d :: Titans : %d \n", capital_score, titan_score);
    printf("Highest Individual Score : %d \n", permatchscore);

    if (capital_score > titan_score)
    {
        printf("Result : Capitals won by %d runs\n", capital_score - titan_score);
        capitalswin++;
        if (capital_score > maxscore)
        {
            maxscore = capital_score;
        }
    }
    else if (titan_score > capital_score)
    {
        titanswin++;
        printf("Result : Titans won by %d runs\n", titan_score - capital_score);
        if (titan_score > maxscore)
        {
            maxscore = titan_score;
        }
    }
    else
    {
        tied++;
        printf("Result : Match Tied\n");
        maxscore = capital_score > maxscore ? capital_score : maxscore;
    }
    printf("\n");
}

int main(int argc, char *argv[])
{

    int threadNum = atoi(argv[1]);
    int matches = threadNum / 22;
    pthread_t games[matches];
    sem_init(&barrier, 0, 22);
    sem_init(&mut, 0, 1);

    srand(time(NULL));

    int i;
    for (i = 0; i < matches; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(games + i, NULL, &startGame, a) != 0)
        {
            perror("Failed to create Game thread");
            return 1;
        }

        if (pthread_join(games[i], NULL) != 0)
        {
            return 2;
        }
        // printf("Match Over \n");

    }

    //Print Days Summary

    printf("---------------------SUMMARY OF THE DAY---------------------\n");
    printf("Matches Played : %d \n", matches);
    printf("Titans   :: Won : %d || Lost : %d || Tied : %d\n", titanswin, matches - titanswin, tied);
    printf("Capitals :: Won : %d || Lost : %d || Tied : %d\n", capitalswin, matches - capitalswin, tied);
    printf("Highest Team Score          : %d\n", maxscore);
    printf("Highest Individual Score    : %d\n", individual_score);
    printf("------------------------------------------------------------\n");

    return 0;
}