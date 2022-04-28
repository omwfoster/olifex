

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

/*!
 * @brief This API initialises the command-line interface.
 * 
 * @param[in] cli : Pointer to cli handle struct.
 * 
 * @return cli_status_t
 */
cli_status_t init_olifex_cli(cli_t **cli);

/*!
 * @brief This API deinitialises the command-line interface.
 * 
 * @param[in] cli : Pointer to cli handle struct.
 * 
 * @return cli_status_t
 */
cli_status_t cli_deinit(cli_t *cli);

/*!
 * @brief This API must be periodically called by the user to process and execute
 *        any commands received.
 * 
 * @param[in] cli : Pointer to cli handle struct.
 * 
 * @return cli_status_t
 */
cli_status_t cli_process(cli_t *cli);

/*!
 * @brief This API should be called from the devices interrupt handler whenever a
 *        character is received over the input stream.
 * 
 * @param[in] cli : Pointer to cli handle struct.
 * @param[in]   c : The character received.
 * 
 * @return cli_status_t
 */
cli_status_t cli_put(cli_t *cli, char *  c);


#ifdef __cplusplus
}
#endif /* End of CPP guard */
#endif /* CLI_H_ */
/** @}*/
