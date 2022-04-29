
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
    cli_t1 = malloc(sizeof(cli_t));
    cli_register_callback("hsv_scrl",(cmd_func_ptr_t)hsv_scroll,(cli_t *)cli);
    cli_register_callback("rgb_scrl",(cmd_func_ptr_t)rgb_scroll,(cli_t *)cli);
    cli_register_callback("hsv_wave",(cmd_func_ptr_t)hsv_wave,(cli_t *)cli);
    cli_register_callback("hsv_fire",(cmd_func_ptr_t)fire_fill,(cli_t *)cli);
    cli_register_callback("hsv_perl",(cmd_func_ptr_t)perlin,(cli_t *)cli);
   // cli->cmd_running = hsv_scroll;

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

void cli_register_callback(char * name,cmd_func_ptr_t fp,cli_t *cli)
{

	if(cli->cmd_cnt<MAX_FUNCTIONS)
	{
	cli->cmd_tbl[cli->cmd_cnt].func=fp;
	strncpy(cli->cmd_tbl[cli->cmd_cnt].cmd,name,8);
	cli->cmd_cnt++;
	}

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




