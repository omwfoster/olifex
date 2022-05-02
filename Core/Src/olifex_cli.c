
#include <olifex_cli.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static volatile uint8_t rx_buf[MAX_BUF_SIZE];      /* CLI Rx byte-buffer */
static volatile uint8_t cmd_pending;

const char cli_prompt[] = ">> ";       /* CLI prompt displayed to the user */
const char cli_unrecog[] = "CMD: Command not recognised\r\n";


/*!
 * @brief This internal API prints a message to the user on the CLI.
 */
static void cli_print(cli_t *cli, const char *msg);

/*!
 * @brief This API initialises the command-line interface.
 */
cli_status_t init_olifex_cli(cli_t **cli)
{
    * cli = malloc(sizeof(cli_t));
    cli_t * c1 = *cli;
    (*cli)->cmd_cnt = MAX_FUNCTIONS;


    cli_register_callback("hsv_scrl",(cmd_func_ptr_t)hsv_scroll,*cli);
    cli_register_callback("rgb_scrl",(cmd_func_ptr_t)rgb_scroll,*cli);
    cli_register_callback("hsv_wave",(cmd_func_ptr_t)hsv_wave,*cli);
    cli_register_callback("hsv_fire",(cmd_func_ptr_t)fire_fill,*cli);
    cli_register_callback("hsv_perl",(cmd_func_ptr_t)perlin,*cli);

    (*cli)->cmd_running = &c1->cmd_tbl[0];



    return CLI_OK;
}

cli_status_t next_olifex_cli(cli_t *cli)
{

//	cli->cmd_running = ((*cli->cmd_running.func) == (&cli->cmd_tbl[MAX_FUNCTIONS].func))?(&cli->cmd_tbl[0]):&cli->cmd_tbl[0];
}

/*!
 * @brief This API deinitialises the command-line interface.
 */
cli_status_t cli_deinit(cli_t *cli)
{
	free(cli);
    return CLI_OK;
}

void cli_register_callback(char * name,cmd_func_ptr_t fp,cli_t *cli)
{



	if((cli->cmd_running)<=(&cli->cmd_tbl[MAX_FUNCTIONS]))
	{
	cli->cmd_running++;
	cli->cmd_running->func=fp;
	strncpy(cli->cmd_running->cmd,name,8);
	}
}

void cli_register_println(println_func_ptr_t fp,cli_t *cli)
{
	cli->println = fp;
}


cli_status_t cli_process(cli_t *cli, char func_str[8])
{
	if (!cmd_pending)
		return CLI_IDLE;

    for(size_t i = 0 ; i < cli->cmd_cnt ; i++)
    {
        if((strncmp(func_str, cli->cmd_tbl[i].cmd,8 )==0))
        {
        	cli->cmd_running = &cli->cmd_tbl[i];
        	return CLI_OK;
        }
    }
    cli_print(cli, cli_prompt); /* Print the CLI prompt to the user.             */

    cmd_pending = 0;
    return CLI_E_CMD_NOT_FOUND;
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




