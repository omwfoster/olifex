

/*! @file cli.h
 * @brief Command-line interface API definitions.
 */

/*!
 * @defgroup CLI API
 */
#ifndef _CLI_H_
#define _CLI_H_

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <olifex_cli_defs.h>


cli_status_t init_olifex_cli(cli_t **cli);
cli_status_t cli_deinit(cli_t *cli);
cli_status_t cli_process(cli_t *cli,char func_str[8]);
void cli_register_callback(char * name,cmd_func_ptr_t fp,cli_t *cli);

#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* CLI_H_ */
/** @}*/
