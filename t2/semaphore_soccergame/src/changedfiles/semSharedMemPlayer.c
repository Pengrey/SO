/**
 *  TODO: REMOVEE  CAPTAIN -> player forming team
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
    int key;                         /*access key to shared memory and semaphore set */
    char *tinp;                      /* numerical parameters test flag */
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

    /* connection to the semaphore set and the shared memory
     *   region and mapping the shared region onto the
     *   process address space
     */
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
    //printf("PLAYER %d has left\n", n);
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

    /* TODO: insert your code here (DONE) */

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
    sh->fSt.playersArrived++;
    sh->fSt.playersFree++;

#define FST sh->fSt
                            //2 * 4
    if ( FST.playersArrived > 2 * NUMTEAMPLAYERS )
    {
    /*
     *      PLAYERS THAT ARE LATE
     */
      //STATE CHANGE ARRIVING -> LATE
      FST.st.playerStat[id] = LATE;
      saveState(nFic , &FST);
      FST.playersFree--;
    }
    else
    {
    /*
     *                     PLAYERS THAT CAN FORM A TEAM
     *
     *   IS THIS CONDITION CORRECT ?
     *   cant be  FST.playersFree == NUMTEAMPLAYERS && FST.goaliesFree == NUMTEAMGOALIES
     *
     */
      if ( FST.playersFree >= NUMTEAMPLAYERS && FST.goaliesFree >= NUMTEAMGOALIES )
      {

        //STATE CHANGE ARRIVING -> FORMING TEAM

        //printf("\033[0;31mPLAYER %d is forming TEAM %d\033[0m\n",id,ret);
        FST.st.playerStat[id] = FORMING_TEAM;
        saveState(nFic , &FST);


        for ( int i = 0 ; i < ( NUMTEAMPLAYERS - 1 )  ; i++)
        {
        // permita a um jogador que esta a espera de uma equipa que se registe
          if (semUp(semgid, sh->playersWaitTeam) == -1 )
          {
            perror("Error using semUp op to unlock a player who is waiting for a team");
            exit(EXIT_FAILURE);
          }
          //printf("semup JOGADORES\n");

        // ficar bloqueado enquanto o player anterior se regista
          if ( semDown(semgid, sh->playerRegistered) == -1 )
          {
            perror("Error using semUp op to unlock a player who is waiting for a team");
            exit(EXIT_FAILURE);
          }
          //printf("semdown JOGADORES ----------------\n");

          // ja que chamou o PLAYER?(0..9) decrementa o numero de jogadores livres
          FST.playersFree--;
        }
     /*   printf("\033[0;31mPLAYER %d is calling \\
        goalie to  TEAM %d\033[0m\n",id,ret);*/

        for ( int i = 0 ; i < NUMTEAMGOALIES  ; i++)
        {
            // permita a um goalie que esta a espera de uma equipa que se registe
          if ( semUp(semgid, sh->goaliesWaitTeam) == -1 )
          {
            perror("Error using semUp op to unlock a player who is waiting for a team");
            exit(EXIT_FAILURE);
          }
          //printf("semup goalie\n");

           // ficar bloqueado enquanto o goalie anterior se regista
          if ( semDown(semgid, sh->playerRegistered) == -1 )
          {
            perror("Error using semUp op to unlock a player who is waiting for a team");
            exit(EXIT_FAILURE);
          }
          //printf("semdown  goalie -------------------------\n");

           // ja que chamou o Goalie?(0..2) decrementa o numero de goalies livres
          FST.goaliesFree--;
        }

        //!!STATE CHANGE  FORMING TEAM -> WAITING_START_?(1 ou 2)
                                          //ret=1             ret=2
        FST.st.playerStat[id] = ret == 1 ? WAITING_START_1 : WAITING_START_2;
        saveState(nFic , &FST);

        /*   "descontar-se" a si mesmo porque esta incluido
        //   na equipa que criou
        */
        FST.playersFree--;
        // Incrementa depois do assignment
        //printf("\033[0;31mPPPPLAYER %d is forming TEAM %d\033[0m\n",id,ret);
        //printf("\033[0;31mPPPPLAYER %d is forming TEAM %d\033[0m\n",id,ret);
        if( semUp(semgid, sh->refereeWaitTeams) == -1)
        {
          perror (" Error on the up operation who signales a referee to say that \
              a team was formed ( PL )");
          exit(EXIT_FAILURE);
        }
        ret = FST.teamId++;
      }
      else
      {
    /*
     *      PLAYERS THAT CANNOT FORM A TEAM BUT AREN'T LATE
     */
        //STATE CHANGE ARRIVING -> WAITING TEAM
        FST.st.playerStat[id] = WAITING_TEAM;
        saveState(nFic , &FST);
      }
    }
#undef FST

    // end of code

    if (semUp (semgid, sh->mutex) == -1) {                                                         /* exit critical region */
        perror ("error on the down operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    // begingin of code
    //      so faz para jogadores que estao a espera que os chamem para formar uma equipa
    if( sh->fSt.st.playerStat[id] == WAITING_TEAM )
    {
      /*printf(" \033[0;32mPLAYER %d  is WAITING FOR TEAM (ret) %d
       MYSTAT  %d  semDown WAITING TEAM \033[0m\n",id,ret,sh->fSt.st.playerStat[id]);*/

      if (semDown(semgid, sh->playersWaitTeam) == -1)
      {
        perror("Error using semUp op to unlock a player who is waiting for a team");
        exit(EXIT_FAILURE);
      }
      ret = sh->fSt.teamId;
      //printf(" \033[0;35mPLAYER %d  is WAITING FOR TEAM ( ret )  %d   teamID =  %d \033[0m\n",id,ret,sh->fSt.teamId);
      sh->fSt.st.playerStat[id] = ret == 1 ?  WAITING_START_1 : WAITING_START_2;
      saveState(nFic , &sh->fSt);
      if(semUp(semgid, sh->playerRegistered) == -1)
      {
      // bloquear o jogador ate alguem dizer para ele ir jogar semUP register
      // should it be assigned here? isn t it a dangerous  operation since its shared memory?
        perror("semUP player 332");
        exit(EXIT_FAILURE);
      }
    }
      /* TODO: insert your code here */

    //printf(" \033[0;32mPLAYER %d  is returning (ret) %d with status %d \033[0m\n", id, ret, sh->fSt.st.playerStat[id]);
    // end of code
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

    /*printf(" \033[0;32mPLAYER %d  is WAITING FOR TEAM (team) %d \\
     *STAT  %d  semDown WAITING REFEREE \033[0m\n",id,team,sh->fSt.st.playerStat[id]);*/
    /* TODO: insert your code here */
    sh->fSt.st.playerStat[id] = team == 1 ?  WAITING_START_1 :WAITING_START_2  ;
    saveState(nFic , &sh->fSt);

    if (semUp (semgid, sh->mutex) == -1) {                                                         /* exit critical region */
        perror ("error on the down operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }
    /* TODO: insert your code here */
    if ( semDown(semgid,sh->playersWaitReferee) == -1)
    {
       perror (" Error on the up operation who signales a players to wait \
           for the begining of the game (PL)");
       exit(EXIT_FAILURE);
    }
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
    sh->fSt.st.playerStat[id] = (team == 1) ? PLAYING_1 : PLAYING_2 ;
    saveState(nFic , &sh->fSt);


    if (semUp (semgid, sh->mutex) == -1) {                                                         /* exit critical region */
        perror ("error on the down operation for semaphore access (PL)");
        exit (EXIT_FAILURE);
    }

    /* TODO: insert your code here */
    //printf("PLayer %d\n",id);
    if ( semDown(semgid , sh->playersWaitEnd) == -1 )
    {
           perror (" Error on the down operation that causes any player \
               process to hang until a referee signals them down meaning the game ended (PL)");
           exit(EXIT_FAILURE);
    }
}
