
#include <olifex_cli.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static volatile uint8_t rx_buf[MAX_BUF_SIZE];      /* CLI Rx byte-buffer */
static volatile uint8_t *rx_buf_ptr;               /* Pointer to Rx byte-buffer */

static uint8_t cmd_buf[MAX_BUF_SIZE];  /* CLI command buffer */
static volatile uint8_t cmd_pending;

const char cli_prompt[] = ">> ";       /* CLI prompt displayed to the user */
const char cli_unrecog[] = "CMD: Command not recognised\r\n";
cli_t  * cli_t1;






/*!
 * @brief This internal API prints a message to the user on the CLI.
 */
static void cli_print(cli_t *cli, const char *msg);

/*!
 * @brief This API initialises the command-line interface.
 */
cli_status_t init_olifex_cli(cli_t **cli)
{
    /* Set buffer ptr to beginning of buf */
    rx_buf_ptr = rx_buf;
    cmd_pending = 0;




    cli_t1 = malloc(sizeof(cli_t));
    cli_register_callback("hsv_scrl",(cmd_func_ptr_t)hsv_scroll);
    cli_register_callback("rgb_scrl",(cmd_func_ptr_t)rgb_scroll);
    cli_register_callback("hsv_wave",(cmd_func_ptr_t)hsv_wave);
    cli_register_callback("hsv_fire",(cmd_func_ptr_t)fire_fill);
    cli_register_callback("hsv_perl",(cmd_func_ptr_t)perlin);



    return CLI_OK;
}

/*!
 * @brief This API deinitialises the command-line interface.
 */
cli_status_t cli_deinit(cli_t *cli)
{
	free(cli_t1);
    return CLI_OK;
}

void cli_register_callback(char * name,cmd_func_ptr_t fp)
{

	if(cli_t1->cmd_cnt<MAX_FUNCTIONS)
	{
	cli_t1->cmd_tbl[cli_t1->cmd_cnt].func=fp;
	strncpy(cli_t1->cmd_tbl[cli_t1->cmd_cnt].cmd,name,8);
	cli_t1->cmd_cnt++;
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

		    cli_print(cli, cli_prompt); /* Print the CLI prompt to the user.             */
		    cmd_pending = 0;
//			cli = cli_t1->cmd_tbl[i].func;
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
cli_status_t cli_put(cli_t *cli, char *  c)
{
    switch(* c)
    {
    case CMD_TERMINATOR:
        
    	if (!cmd_pending) {
			*rx_buf_ptr = '\0';            /* Terminate the msg and reset the msg ptr.      */
			strcpy(cmd_buf, rx_buf);       /* Copy string to command buffer for processing. */
			cmd_pending = 1;
			rx_buf_ptr = rx_buf;              /* Reset buf_ptr to beginning.                   */
    	}
        break;

    case '\b':
        /* Backspace. Delete character. */
        if(rx_buf_ptr > rx_buf)
            rx_buf_ptr--;
        break;

    default:
        /* Normal character received, add to buffer. */
        if((rx_buf_ptr - rx_buf) < MAX_BUF_SIZE)
            *rx_buf_ptr++ = * c;
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
    char send[50];

    strcpy(send, msg);
    cli->println(send);
}




