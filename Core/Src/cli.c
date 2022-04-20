
#include "cli.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static volatile uint8_t buf[MAX_BUF_SIZE];      /* CLI Rx byte-buffer */
static volatile uint8_t *buf_ptr;               /* Pointer to Rx byte-buffer */

static uint8_t cmd_buf[MAX_BUF_SIZE];  /* CLI command buffer */
static volatile uint8_t cmd_pending;

const char cli_prompt[] = ">> ";       /* CLI prompt displayed to the user */
const char cli_unrecog[] = "CMD: Command not recognised\r\n";
cli_t  * cli_dt1;






/*!
 * @brief This internal API prints a message to the user on the CLI.
 */
static void cli_print(cli_t *cli, const char *msg);

/*!
 * @brief This API initialises the command-line interface.
 */
cli_status_t cli_init(cli_t **cli)
{
    /* Set buffer ptr to beginning of buf */
    buf_ptr = buf;
    cmd_pending = 0;


    cli_dt1 = malloc(sizeof(cli_t));
    cli_dt1->cmd_cnt = 0;


    return CLI_OK;
}

/*!
 * @brief This API deinitialises the command-line interface.
 */
cli_status_t cli_deinit(cli_t *cli)
{
	free(cli_dt1);
    return CLI_OK;
}

void cli_register_callback(char * name,cmd_func_ptr_t fp)
{

	if(cli_dt1->cmd_cnt<MAX_FUNCTIONS)
	{
	cli_dt1->cmd_tbl[cli_dt1->cmd_cnt].func=fp;
	strncpy(cli_dt1->cmd_tbl[cli_dt1->cmd_cnt].cmd,name,8);
	cli_dt1->cmd_cnt++;
	}

}



/*! @brief This API must be periodically called by the user to process and execute
 *         any commands received.
 */
cli_status_t cli_process(cli_t *cli)
{
	if (!cmd_pending)
		return CLI_IDLE;



    /* Search the command table for a matching command, using argv[0]
     * which is the command name. */
    for(size_t i = 0 ; i < cli->cmd_cnt ; i++)
    {
        if(strncmp("", cli->cmd_tbl[i].cmd,8 ))
        {
            /* Found a match, execute the associated function. */
			cli_status_t return_value = cli->cmd_tbl[i].func();
		    cli_print(cli, cli_prompt); /* Print the CLI prompt to the user.             */
		    cmd_pending = 0;
			return return_value;
        }
    }

    /* Command not found */
    cli_print(cli, cli_unrecog);

    cli_print(cli, cli_prompt); /* Print the CLI prompt to the user.             */

    cmd_pending = 0;
    return CLI_E_CMD_NOT_FOUND;
}

/*!
 * @brief This API should be called from the devices interrupt handler whenever a
 *        character is received over the input stream.
 */
cli_status_t cli_put(cli_t *cli, char c)
{
    switch(c)
    {
    case CMD_TERMINATOR:
        
    	if (!cmd_pending) {
			*buf_ptr = '\0';            /* Terminate the msg and reset the msg ptr.      */
			strcpy(cmd_buf, buf);       /* Copy string to command buffer for processing. */
			cmd_pending = 1;
			buf_ptr = buf;              /* Reset buf_ptr to beginning.                   */
    	}
        break;

    case '\b':
        /* Backspace. Delete character. */
        if(buf_ptr > buf)
            buf_ptr--;
        break;

    default:
        /* Normal character received, add to buffer. */
        if((buf_ptr - buf) < MAX_BUF_SIZE)
            *buf_ptr++ = c;
        else
            return CLI_E_BUF_FULL;
        break;
    }
}

/*!
 * @brief Print a message on the command-line interface.
 */
static void cli_print(cli_t *cli, const char *msg)
{
    /* Temp buffer to store text in ram first */
    char buf[50];

    strcpy(buf, msg);
    cli->println(buf);
}




