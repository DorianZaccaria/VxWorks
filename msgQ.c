/*
	  Echange par Queue de Messages, modle client serveur 

		Un serveur cree un queue de messages dont l'ID est global puis
		il lance deux taches. Chaque tache cree une queue de messages 
		privee dont elle donne l'ID au serveur via la queue globale. Le
		serveur peut alors lire le message de chaque client puis envoyer
		une reponse sur sa queue privee

			startP4 = serveur qui cree les taches
			taskClient1 = client 
			taskClient2 = client
*/

/* T. Joubert 2004 */
/* msg_queue2.c     */

#include "vxWorks.h"
#include "msgQLib.h"

/* globals */
/*
int      DebugMsg   = 1;
int      DoNotBreak = 1;
*/
/* defines */

/* Define logMsg as printf to have display in real-time */
#define logMsg printf

#define DEBUG_MSG(a) {if (DebugMsg) logMsg a;}
#define TASK_PRIORITY      99
#define TASK_STACK_SIZE    2048
#define MAX_MESSAGES       100
#define MAX_MESSAGE_LENGTH 100


/* TODO:  internal message structure */
struct msgStruct
{
  char msg[4];
  MSG_Q_ID queueId;
};

MSG_Q_ID globalQueueId;

/*****************************************************************************/
/*                                                                           */
/* Function   : taskClient1                                                  */ 
/* Args       : mesgQueueId: server message queue ID                         */
/* Return     : none                                                         */
/* Description: Send a message to the server, and wait for the answer        */
/*                                                                           */
/*****************************************************************************/
void taskClient1(MSG_Q_ID mesgQueueId)
{
  MSG_Q_ID clientQueueId;
  struct msgStruct buf;

  /* TODO: create the client 1 message queue */
  clientQueueId = msgQCreate(MAX_MESSAGES, MAX_MESSAGE_LENGTH, MSG_Q_FIFO);
 
  /* TODO: send message to server*/
  buf.queueId = clientQueueId;
  buf.msg[0] = 'i';
  buf.msg[1] = 'd';
  buf.msg[2] = '1';
  buf.msg[3] = '\0';

  logMsg("Hello Im the task number 1 with message %s\n", buf.msg);
  msgQSend(mesgQueueId, (char*) &buf, sizeof(buf), WAIT_FOREVER, MSG_PRI_NORMAL);

  /* TODO: wait for answer*/
  msgQReceive(clientQueueId, (char*) &buf, MAX_MESSAGE_LENGTH, WAIT_FOREVER);
  logMsg("Hello Im the task number 1 with message %s\n", buf.msg);
}


/*****************************************************************************/
/*                                                                           */
/* Function   : taskClient2                                                  */ 
/* Args       : mesgQueueId: server message queue ID                         */
/* Return     : none                                                         */
/* Description: Send a message to the server, and wait for the answer        */
/*                                                                           */
/*****************************************************************************/
void taskClient2(MSG_Q_ID mesgQueueId)
{
  MSG_Q_ID clientQueueId;
  struct msgStruct buf;

  /* TODO: create the client 2 message queue */
  clientQueueId = msgQCreate(MAX_MESSAGES, MAX_MESSAGE_LENGTH, MSG_Q_FIFO);
 
  /* TODO: send message to server*/
  buf.queueId = clientQueueId;
  buf.msg[0] = 'i';
  buf.msg[1] = 'd';
  buf.msg[2] = '2';
  buf.msg[3] = '\0';
  
  logMsg("Hello Im the task number 2 with message %s\n", buf.msg);
  msgQSend(mesgQueueId, (char*) &buf, sizeof(buf), WAIT_FOREVER, MSG_PRI_NORMAL);

  /* TODO: wait for answer*/
  msgQReceive(clientQueueId, (char*) &buf, MAX_MESSAGE_LENGTH, WAIT_FOREVER);

  logMsg("Hello Im the task number 2 with message %s\n", buf.msg);
}

/*****************************************************************************/
/*                                                                           */
/* Function   : taskClient3                                                  */ 
/* Args       : none                                                         */
/* Return     : none                                                         */
/* Description: Send a message to the server, and wait for the answer        */
/*                                                                           */
/*****************************************************************************/
void taskClient3()
{
  MSG_Q_ID clientQueueId;
  struct msgStruct buf;

  /* TODO: create the client 2 message queue */
  clientQueueId = msgQCreate(MAX_MESSAGES, MAX_MESSAGE_LENGTH, MSG_Q_FIFO);
 
  /* TODO: send message to server*/
  buf.queueId = clientQueueId;
  buf.msg[0] = 'i';
  buf.msg[1] = 'd';
  buf.msg[2] = '3';
  buf.msg[3] = '\0';
  
  logMsg("Hello Im the task number 3 with message %s\n", buf.msg);
  msgQSend(globalQueueId, (char*) &buf, sizeof(buf), WAIT_FOREVER, MSG_PRI_NORMAL);

  /* TODO: wait for answer*/
  msgQReceive(clientQueueId, (char*) &buf, MAX_MESSAGE_LENGTH, WAIT_FOREVER);

  logMsg("Hello Im the task number 3 with message %s\n", buf.msg);
}

/*****************************************************************************/
/*                                                                           */
/* Function   : startProg                                                    */ 
/* Args       : none                                                         */
/* Return     : none                                                         */
/* Description: Entry point for the test. Acts as a server task, which       */
/*              first creates two clients, waits for a Hello message, and    */
/*              then replies.                                                */
/*****************************************************************************/
void startP4(void)
{
  char taskName[2];
  struct msgStruct buf;
  MSG_Q_ID serverQueueId;

  /* TODO: create the server message queue */
  serverQueueId = msgQCreate(MAX_MESSAGES, MAX_MESSAGE_LENGTH, MSG_Q_FIFO);

  globalQueueId = serverQueueId;

  /* TODO: spawn the first client task */
  taskName[0] = '1';
  taskName[1] = '\0';
  taskSpawn(taskName, TASK_PRIORITY, 0, TASK_STACK_SIZE, (FUNCPTR)taskClient1, serverQueueId,0,0,0,0,0,0,0,0,0);

  /* TODO: spawn the second client task */
  taskName[0] = '2';
  taskSpawn(taskName, TASK_PRIORITY, 0, TASK_STACK_SIZE, (FUNCPTR)taskClient2, serverQueueId,0,0,0,0,0,0,0,0,0);

  /* TODO: create a loop which waits for message from the clients */
  while (1)
  {
    msgQReceive(serverQueueId, (char*) &buf, MAX_MESSAGE_LENGTH, WAIT_FOREVER);
    /* send answer */
    buf.msg[0] = 'd';
    buf.msg[1] = 'i';
    msgQSend(buf.queueId, (char*) &buf, sizeof(buf), WAIT_FOREVER, MSG_PRI_NORMAL);
  } /* while (DoNotBreak) */
}



