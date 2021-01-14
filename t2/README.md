#NOTES


##    REFEREE
  Referee -> /*TODO: error treating em semaforos*/
##    PLAYER
  Player ->
          1. /TODO: Fix player forming a team */
          2. /TODO: Fix teamId ( whok change's it?)*/
          3. /*TODO: error treating em semaforos*/
##    GOALIE
  Goalie ->
          1. /*TODO: TUDO*/
          1. /*TODO: error treating em semaforos*/



so o proprio tipo altera o seu estados


##prob datastruct estruturas de dados


~~ STAT; ~~
typedef struct {
    /** \brief players state */                   #estados para mudar
    `unsigned int playerStat[NUMPLAYERS];          #estados para mudar`
    /** \brief goalies state */                   #estados para mudar
    `unsigned int goalieStat[NUMGOALIES];          #estados para mudar`
    /** \brief referees state */
    `unsigned int refereeStat;`

} STAT;
------------------------------------------------------------
sh
^
|
~~ FULL_STAT; ~~
typedef struct
{   /** \brief state of all intervening entities */
    STAT st;

    /** \brief total number of players */
`    int nPlayers;`

    /** \brief total number of goalies */
    `int nGoalies;`

    /** \brief total number of referees */
    `int nReferees;`

    /** \brief number of players that already arrived */
    `int playersArrived;``
    /** \brief number of goalies that already arrived */
`    int goaliesArrived; `
    /** \brief number of players that arrived and are free (no team) */
    `int playersFree;`
    /** \brief number of goalies that arrived and are free (no team) */
    `int goaliesFree;`

    /** \brief id of team that will be formed next - initial value=1 */
    `int teamId;`

}
~~ FULL_STAT; end ~~
---------------------------------------------------------------------------
typedef struct
        { /** \brief full state of the problem */
          FULL_STAT fSt;

    ##/* semaphores ids */
          **brief identification of critical region protection semaphore – val = 1**
          `unsigned int mutex;`
          **brief identification of semaphore used by players**
          **to wait for forming team teammate - val = 0**
          `unsigned int playersW&aitTeam;`
          **brief identification of semaphore used by goalies to wait**
          **for forming team teammate - val = 0**
          `unsigned int goaliesWaitTeam;`
          **brief identification of semaphore used by players**
          **and goalies to wait for the match to start - val = 0**
          `unsigned int playersWaitReferee;`
          **brief identification of semaphore used by players and**
          **goalies to wait for the match to end - val = 0**
          `unsigned int playersWaitEnd;`
          **brief identification of semaphore used by referee to wait**
          **for teams to be formed – val = 0**
          `unsigned int refereeWaitTeams;`
          **brief identification of semaphore used by players and goalies**
          **to acknowledge team registration – val = 0**
          `unsigned int playerRegistered;`

        } SHARED_DATA;
---------------------------------------------------------------------------
~~ Available states~~
    /* Player/Goalie state constants */

    /** \brief player/goalie initial state, arriving */
    `#define  ARRIVING          0`
    /** \brief player/goalie waiting to constitute team */
    `#define  WAITING_TEAM      1`
    /** \brief player/goalie waiting to constitute team */
    `#define  FORMING_TEAM      2`
    /** \brief player/goalie waiting for referee to start game in team 1 */
    `#define  WAITING_START_1   3`
    /** \brief player/goalie waiting for referee to start game in team 2 */
    `#define  WAITING_START_2   4`
    /** \brief player/goalie playing in team 1 */
    `#define  PLAYING_1         5`
    /** \brief player/goalie playing in team 2 */
    `#define  PLAYING_2         6`
    /** \brief player/goalie playing */
    `#define  LATE              7`

    /* Referee state constants */

    /** \brief referee initial state, arriving  */
    `#define  ARRIVING          0`
    /** \brief referee waiting for both teams  */
    `#define  WAITING_TEAMS     1`
    /** \brief referee starting game  */
    `#define  STARTING_GAME     2`
    /** \brief referee refereeing  */
    `#define  REFEREEING        3`
    /** \brief referee ending game  */
    `#define  ENDING_GAME       4`


    /* Player/Goalie state constants */

    /** \brief player/goalie initial state, arriving */
    `#define  ARRIVING          0`
    /** \brief player/goalie waiting to constitute team */
    `#define  WAITING_TEAM      1`
    /** \brief player/goalie waiting to constitute team */
    `#define  FORMING_TEAM      2`
    /** \brief player/goalie waiting for referee to start game in team 1 */
    `#define  WAITING_START_1   3`
    /** \brief player/goalie waiting for referee to start game in team 2 */
    `#define  WAITING_START_2   4`
    /** \brief player/goalie playing in team 1 */
    `#define  PLAYING_1         5`
    /** \brief player/goalie playing in team 2 */
    `#define  PLAYING_2         6`
    /** \brief player/goalie playing */
    `#define  LATE              7`

    /* Referee state constants */

    /** \brief referee initial state, arriving  */
    `#define  ARRIVING          0`
    /** \brief referee waiting for both teams  */
    `#define  WAITING_TEAMS     1`
    /** \brief referee starting game  */
    `#define  STARTING_GAME     2`
    /** \brief referee refereeing  */
    `#define  REFEREEING        3`
    /** \brief referee ending game  */

#define  ENDING_GAME       4
---------------------------------------------------------------------------------------
##Important Semaphores

    `probSemSharedMemSoccerGame.c:135    sh->playersWaitTeam             = PLAYERSWAITTEAM;`
    `probSemSharedMemSoccerGame.c:136    sh->goaliesWaitTeam             = GOALIESWAITTEAM;`
    `probSemSharedMemSoccerGame.c:137    sh->playersWaitReferee          = PLAYERSWAITREFEREE;`
    `probSemSharedMemSoccerGame.c:138    sh->playersWaitEnd              = PLAYERSWAITEND;`
    `probSemSharedMemSoccerGame.c:139    sh->refereeWaitTeams            = REFEREEWAITTEAMS;`
    `probSemSharedMemSoccerGame.c:140    sh->playerRegistered            = PLAYERREGISTERED;`

---------------------------------------------------------------------------------------
##Important Constats

    `probConst.h:17#define  NUMPLAYERS       10`
    `probConst.h:19#define  NUMGOALIES        3`
    `probConst.h:21#define  NUMREFEREES       1`
    `probConst.h:24#define  NUMTEAMPLAYERS     4`
    `probConst.h:26#define  NUMTEAMGOALIES     1`
    `probConst.h:32#define  ARRIVING          0`
    `probConst.h:34#define  WAITING_TEAM      1`
    `probConst.h:36#define  FORMING_TEAM      2`
    `probConst.h:38#define  WAITING_START_1   3`
    `probConst.h:40#define  WAITING_START_2   4`
    `probConst.h:42#define  PLAYING_1         5`
    `probConst.h:44#define  PLAYING_2         6`
    `probConst.h:46#define  LATE              7`
    `probConst.h:51#define  ARRIVING          0`
    `probConst.h:53#define  WAITING_TEAMS     1`
    `probConst.h:55#define  STARTING_GAME     2`
    `probConst.h:57#define  REFEREEING        3`
    `probConst.h:59#define  ENDING_GAME       4`


---------------------------------------------------------------------------------------

probSemSharedMemSoccerGame main
sem* any
semSharedMemGoalie
semSharedMemReferee
semSharedMemPlayer

sharedDataSync numeros de semaforos sh poiter para a memoria partilhada
shared memory usado antes



make all_bin -> solucao do setor


d run ficheiros para ajudar a correr e a limpar a memoria partilhada


estados dos players .... em probConst.h


formar equipa apos haver 4 P e 1 G

quem chega tarde fica com 7
1. perceber ciclos de vida
**../src/semSharedMemGoalie.c:145    /* TODO: insert your code here */**
**../src/semSharedMemGoalie.c:179    /* TODO: insert your code here */**
**../src/semSharedMemGoalie.c:186    /* TODO: insert your code here */**
**../src/semSharedMemGoalie.c:207    /* TODO: insert your code here */**
**../src/semSharedMemGoalie.c:214    /* TODO: insert your code here */**
**../src/semSharedMemGoalie.c:234    /* TODO: insert your code here */**
**../src/semSharedMemGoalie.c:241    /* TODO: insert your code here */**
**../src/semSharedMemPlayer.c:147    /* TODO: insert your code here */**
**../src/semSharedMemPlayer.c:182    /* TODO: insert your code here */**
`**semSharedMemPlayer.c:177/*TODO: **falta usar o semaforo de registration e de forming** */*`
**semSharedMemPlayer.c:189/*TODO: **falta usar o semaforo de registration e de forming** */**
**../src/semSharedMemPlayer.c:210    /* TODO: insert your code here */**
**../src/semSharedMemPlayer.c:217    /* TODO: insert your code here */**
**../src/semSharedMemPlayer.c:237    /* TODO: insert your code here */**
**../src/semSharedMemPlayer.c:244    /* TODO: insert your code here */**
**../src/semSharedMemReferee.c:145    /* DO:TO insert your code here *I think its working/**
**../src/semSharedMemReferee.c:170    /* DO:TO insert your code here *I think its working/**
**../src/semSharedMemReferee.c:177    /* DO:TO insert your code here *I think its working/**
**../src/semSharedMemReferee.c:195    /* DO:TO insert your code here *I think its working/**
**../src/semSharedMemReferee.c:202    /* DO:TO insert your code here *I think its working/**
**../src/semSharedMemReferee.c:220    /* DO:TO insert your code here *I think its working/**
**../src/semSharedMemReferee.c:244    /* DO:TO insert your code here *I think its working/**
**../src/semSharedMemReferee.c:251    /* DO:TO insert your code here *I think its working/**


                                                                      I think its working
                                                                      I think its working
                                                                      I think its working
                                                                      I think its working

semaforos nao chegam

fst ->
        - struct stat >> estado de todas as entidades
        - cada entidade so altera o seu estado




referee Chega  faz semaphorerefereeWaitTeams up ---> waitForTeams ---> semaphorerefereeWatiTeams down  --- Inicia o jogo -> Arbitra ----> acaba
player1 chega
player2 chega
.
.
goaler1 chega
.
player 4 chega -- forma equipa



wait dor team


sh->fst.st.refereeStat = WAITING_TEAMS;

saveState(nFic , &sh->st)
