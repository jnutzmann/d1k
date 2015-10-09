/********************************************************************
d1k_portal.c

Copyright (c) 2014, Jonathan Nutzmann

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
********************************************************************/

/****************************************************************************
 * Includes
 ***************************************************************************/

#include "stdlib.h"
#include "d1k_stdio.h"
#include "d1k_portal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "d1k_led.h"


/****************************************************************************
 * Defines
 ***************************************************************************/

#define MAX_CMD_LEN				(64)
#define MAX_ARG_LIST_LENGTH		(5)

/****************************************************************************
 * Typedefs
 ***************************************************************************/

typedef struct d1kPortalEntry_t d1kPortalEntry;

struct d1kPortalEntry_t {
	d1kPortalFxn fxn;
	char* cmd;
	d1kPortalEntry* next;
};

/****************************************************************************
 * Global Variables
 ***************************************************************************/

static d1kPortalEntry* portalList = NULL;
static uint8_t currentCmdBuffer[MAX_CMD_LEN];
static uint16 currentCmdBufferPos = 0;
static xTaskHandle portalTaskHandle;

/****************************************************************************
 * Private Prototypes
 ***************************************************************************/

static void d1k_portal_Task             ( void * pvParameters );
static void d1k_portal_ProcessCmdBuffer ( void );
static void d1k_portal_help     ( int argc, char** argv );
static void d1k_portal_dispatch ( int argc, char** argv);
static void penisCMD            ( int argc, char** argv );


/****************************************************************************
 * Public Functions
 ***************************************************************************/

/**
 * Initializes the D1K portal.
 */
void d1k_portal_Init( void )
{
	d1k_stdio_Init();

	d1k_portal_RegisterFunction("?",d1k_portal_help);
	d1k_portal_RegisterFunction("penis",penisCMD);

	xTaskCreate(d1k_portal_Task,"PORTAL",1024,NULL,2,&portalTaskHandle);
}

/**
 * Registers a command in the D1K portal.
 * @param cmd - Base string command to register.
 * @param fxn - Callback functions when the string is entered.
 */
void d1k_portal_RegisterFunction( char* cmd, d1kPortalFxn fxn)
{
	d1kPortalEntry* currentEntry = portalList;
	d1kPortalEntry* newEntry = (d1kPortalEntry*) malloc(sizeof(d1kPortalEntry));
	newEntry->cmd = cmd;
	newEntry->fxn = fxn;
	newEntry->next = NULL;

	if (currentEntry == NULL)
	{
		portalList = newEntry;
		return;
	}

	while (currentEntry->next != NULL)
	{
		currentEntry = currentEntry->next;
	}

	currentEntry->next = newEntry;
}

/****************************************************************************
 * Private Functions
 ***************************************************************************/

static void d1k_portal_Task( void * pvParameters )
{

	uint8_t ch;

	while (1)
	{
		if (d1k_stdio_WaitForReceived(&ch) == pdTRUE)
		{
			if (ch == '\r')
			{
				d1k_portal_ProcessCmdBuffer();
			}
			else if (ch == 8)
			{
				// TODO: Make backspace work.
				printf(" \b");
			}
			else if (ch == '\n')
			{

			}
			else
			{
				d1k_stdio_PutCh(ch);
				currentCmdBuffer[currentCmdBufferPos] = ch;
				currentCmdBufferPos = (currentCmdBufferPos + 1) % MAX_CMD_LEN;
				currentCmdBuffer[currentCmdBufferPos] = 0;

			}
		}
	}

}

static void d1k_portal_ProcessCmdBuffer( void )
{
	char* argv[5];
	int argc = 0;


	argv[argc] = strtok((char*)currentCmdBuffer, " ");
	if (argv[argc] != NULL)
	{
		argc++;
		while((argv[argc] = strtok(NULL, " "))) argc++;
	}

	if (argc > 0)
	{
		d1k_portal_dispatch(argc,argv);
	}

	currentCmdBufferPos = 0;
	currentCmdBuffer[currentCmdBufferPos] = 0;

	printf("\n-> ");
}

static void d1k_portal_dispatch( int argc, char** argv)
{
	d1kPortalEntry* entry = portalList;

	while (entry != NULL)
	{
		if ( strcmp(entry->cmd, argv[0]) == 0 )
		{
			(entry->fxn)( argc, argv );
			return;
		}

		entry = entry->next;
	}

	printf("\nUnknown Command.");
}

static void d1k_portal_help( int argc, char** argv )
{
	d1kPortalEntry* entry = portalList;

	printf("\n\nAvailable Commands:\n");

	while (entry != NULL)
	{
		printf("%s\n",entry->cmd);
		entry = entry->next;
	}
}

static void penisCMD( int argc, char** argv )
{
	if (argc > 1)
	{
		int count;

		if (strcmp(argv[1],"slavo" ) == 0 )
		{
			count = 0;
		}
		else if (strcmp(argv[1],"dansdad" ) == 0 )
		{
			count = 30;
		}
		else if (strcmp(argv[1],"bryansdad" ) == 0 )
		{
			count = 45;
		}
		else
		{
			count = atoi(argv[1]);
		}

		printf("\n\n<");

		for (int i=0; i < count; i++)
		{
			printf("=");
		}

		printf("3\n\n");
	}
}
