/**
 *  \file semSharedMemSmoker.c (implementation file)
 *
 *  \brief Problem name: SoccerGame
 *
 *  Synchronization based on semaphores and shared memory.
 *  Implementation with SVIPC.
 *
 *  Definition of the operations carried out by the players:
 *     \li arrive
 *     \li playerConstituteTeam
 *     \li waitReferee
 *     \li playUntilEnd
 *
 *  \author Nuno Lau - January 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <math.h>

#include "probConst.h"
#include "probDataStruct.h"
#include "logging.h"
#include "sharedDataSync.h"
#include "semaphore.h"
#include "sharedMemory.h"

/** \brief logging file name */
static char nFic[51];

/** \brief shared memory block access identifier */
static int shmid;

/** \brief semaphore set access identifier */
static int semgid;

/** \brief pointer to shared memory region */
static SHARED_DATA *sh;

/** \brief player takes some time to arrive */
static void arrive (int id);

/** \brief player constitutes team */
static int playerConstituteTeam (int id);

/** \brief player waits for referee to start match */
static void waitReferee(int id, int team);

/** \brief player waits for referee to end match */
static void playUntilEnd(int id, int team);

/**
 *  \brief Main program.
 *
 *  Its role is to generate the life cycle of one of intervening entities in the problem: the player.
 */
int main (int argc, char *argv[])
{
    int key;                                            /*access key to shared memory and semaphore set */
    char *tinp;                                                       /* numerical parameters test flag */
    int n, team;
    /* validation of command line parameters */
    if (argc != 4) {
        freopen ("error_PL", "a", stderr);
        fprintf (stderr, "Number of parameters is incorrect!\n");
        return EXIT_FAILURE;
    }

    /* get goalie id - argv[1]*/
    n = (unsigned int) strtol (argv[1], &tinp, 0);
    if ((*tinp != '\0') || (n >= NUMPLAYERS )) {
        fprintf (stderr, "Player process identification is wrong!\n");
        return EXIT_FAILURE;
    }

    /* get logfile name - argv[2]*/
    strcpy (nFic, argv[2]);

    /* redirect stderr to error file  - argv[3]*/
    freopen (argv[3], "w", stderr);
    setbuf(stderr,NULL);


    /* getting key value */
    if ((key = ftok (".", 'a')) == -1) {
        perror ("error on generating the key");
        exit (EXIT_FAILURE);
    }

    /* connection to the semaphore set and the shared memory region and mapping the shared region onto the
       process address space */
    if ((semgid = semConnect (key)) == -1) {
        perror ("error on connecting to the semaphore set");
        return EXIT_FAILURE;
    }
    if ((shmid = shmemConnect (key)) == -1) {
        perror ("error on connecting to the shared memory region");
        return EXIT_FAILURE;
    }
    if (shmemAttach (shmid, (void **) &sh) == -1) {
        perror ("error on mapping the shared region on the process address space");
        return EXIT_FAILURE;
    }

    /* initialize random generator */
    srandom ((unsigned int) getpid ());


    /* simulation of the life cycle of the player */
    arrive(n);
    if((team = playerConstituteTeam(n))!=0) {
        waitReferee(n, team);
        playUntilEnd(n, team);
    }

    /* unmapping the shared region off the process address space */
    if (shmemDettach (sh) == -1) {
        perror ("error on unmapping the shared region off the process address space");
        return EXIT_FAILURE;;
    }

    return EXIT_SUCCESS;
}

/**
 *  \brief player takes some time to arrive
 *
 *  Player updates state and takes some time to arrive
 *  The internal state should be saved.
 *
 */
static void arrive(int id)
{
    if (semDown (semgid, sh->mutex) == -1)  {                                                     /* enter critical region */
        perror ("error on the up operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    sh->fSt.st.playerStat[id] = ARRIVING;
    saveState(nFic , &sh->fSt);

    /* TODO: insert your code here */

    if (semUp (semgid, sh->mutex) == -1) {                                                         /* exit critical region */
        perror ("error on the down operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    usleep((200.0*random())/(RAND_MAX+1.0)+50.0);
}

/**
 *  \brief player constitutes team
 *
 *  If player is late, it updates state and leaves.
 *
 *  If there are enough free players and free goalies to form a team, player forms team allowing
 *  team members to proceed and waiting for them to acknowledge registration.
 *  Otherwise it updates state, waits for the forming teammate to "call" him, saves its team
 *  and acknowledges registration.
 *  The internal state should be saved.
 *
 *  \param id player id
 *
 *  \return id of player team (0 for late goalies; 1 for team 1; 2 for team 2)
 *
 */
static int playerConstituteTeam (int id)
{
    int ret = 0;
    if (semDown (semgid, sh->mutex) == -1)  {                                                     /* enter critical region */
        perror ("error on the up operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */

    // begingin of code
    //printf("\033[0;31mPLAYER %d HAS CHEGADO\033[0m\n", id);

    sh->fSt.playersArrived++;  // sinalizar a chegada de mais um "futebolista"
    sh->fSt.playersFree++;// sinalizar a chegada de mais um "futebolista" e que este esta disponivel


    if ( sh->fSt.teamId > 2)
    {   //todos os jogadores que entram aqui chegaram tarde demais porque ja chegaram mais de 8 jogadores
     // printf("\033[0;31mPLAYER %d LATE\033[0m\n", id);
      sh->fSt.st.playerStat[id] = LATE;
      saveState(nFic , &sh->fSt);
      sh->fSt.playersFree--;
    }
    else if ( sh->fSt.playersFree >= 4 && sh->fSt.goaliesFree >= 1)
    {
      ret = sh->fSt.teamId;
      printf(" \033[0;35mPLAYER %d FORMINGTEAM\033[0m\n", id);
      sh->fSt.st.playerStat[id] = FORMING_TEAM;
      saveState(nFic , &sh->fSt);
      /*                              4  - 1 porque um deles e o que entrar aqui e se fizermos 4 semUp e 4 semDown o processo vai bloquear em semDown  */
      for( int i = 0; i < NUMTEAMPLAYERS - 1 ; i++)
      {
        printf(" \033[0;35mPLAYER %d FORMINGTEAM %d\033[0m\n", id,i);
        semUp(semgid, sh->playersWaitTeam);
        printf(" \033[0;35mPLAYER %d FORMINGTEAM %d at 1st Stop\033[0m\n", id,i);
        semDown(semgid, sh->playerRegistered);
        printf(" \033[0;35mPLAYER %d FORMINGTEAM %d at 2nd stop\033[0m\n", id,i);
      }
      //   sinalizar a um goalie para entrar na nossa equipa
      for( int i = 0; i < NUMTEAMGOALIES ; i++)
      {
        printf("FEZ UMA VEZ ISTOOOOO \n");
        semUp(semgid, sh->goaliesWaitTeam);
        semDown(semgid, sh->playerRegistered);
      }
      sh->fSt.st.playerStat[id] = (ret == 1) ? WAITING_START_1 : WAITING_START_2;
      saveState(nFic , &sh->fSt);
    }
    else
    {
      sh->fSt.st.playerStat[id] = WAITING_TEAM;
      saveState(nFic , &sh->fSt);
      ret = sh->fSt.teamId;
      //printf(" \033[0;32mPLAYER %d WAITINGFORTEAM saved TEAM --> %d\033[0m\n", id, ret);
    }
    //printf("\033[0;31mNUMBER OF Players Arrived %d\033[0m\n", sh->fSt.playersArrived );
    if ( sh->fSt.playersArrived == NUMTEAMPLAYERS && sh->fSt.goaliesArrived == NUMTEAMGOALIES ) //TODOOO THIS CODITION MUST BE CHANGED
      {
        //printf("\033[0;31mPLAYER %d HAS CHEGADO INCREMENTEI\033[0m \n", id);
        ret = sh->fSt.teamId;
        sh->fSt.teamId++;
      }
    // end of code

    if (semUp (semgid, sh->mutex) == -1) {                                                         /* exit critical region */
        perror ("error on the down operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }
    // bloquear o jogador ate alguem dizer para ele ir jogar semUP register

    // begingin of code
    // end of code
    if(sh->fSt.st.playerStat[id] != LATE  )
    {
      //printf(" \033[0;32mPLAYER %d  is WAITING FOR TEAM\033[0m\n",id);
      semDown(semgid, sh->playersWaitTeam);
    while( 1 )
    {
      if(semUp(semgid, sh->playerRegistered) != -1)
      {
        //printf(" \033[0;35mPLAYER %d  is REgistering Itself FOR TEAM %d\033[0m\n",id,ret);
        sh->fSt.st.playerStat[id] = (ret == 1) ? WAITING_START_1 : WAITING_START_2;
        saveState(nFic , &sh->fSt);
        break;
      }
    }
    }
      /* TODO: insert your code here */

    return ret;
}

/**
 *  \brief player waits for referee to start match
 *
 *  The player updates its state and waits for referee to end match.
 *  The internal state should be saved.
 *
 *  \param id   player id
 *  \param team player team
 */
static void waitReferee (int id, int team)
{
    if (semDown (semgid, sh->mutex) == -1)  {                                                     /* enter critical region */
        perror ("error on the up operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */
    if( team == 1 )
      sh->fSt.st.playerStat[id] = WAITING_START_1 ;
    else
      sh->fSt.st.playerStat[id] = WAITING_START_2 ;
    saveState(nFic , &sh->fSt);

    if (semUp (semgid, sh->mutex) == -1) {                                                         /* exit critical region */
        perror ("error on the down operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */
    semDown(semgid,sh->playersWaitReferee);
}

/**
 *  \brief player waits for referee to end match
 *
 *  The player updates its state and waits for referee to end match.
 *  The internal state should be saved.
 *
 *  \param id   player id
 *  \param team player team
 */
static void playUntilEnd (int id, int team)
{
    if (semDown (semgid, sh->mutex) == -1)  {                                                     /* enter critical region */
        perror ("error on the up operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */
    if( team == 1 )
      sh->fSt.st.playerStat[id] = PLAYING_1 ;
    else
      sh->fSt.st.playerStat[id] = PLAYING_2 ;
    saveState(nFic , &sh->fSt);


    if (semUp (semgid, sh->mutex) == -1) {                                                         /* exit critical region */
        perror ("error on the down operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */
    semDown(semgid , sh->playersWaitEnd);

}
