/**
 * @file        datalog_cli_asserter.h
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       Wrapper for the Datalog API CLI into the Datalog API
 *  
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Alexander Hoffman, 2017
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */

#ifndef __DATALOG_CLI_ASSERTER_H__
#define __DATALOG_CLI_ASSERTER_H__

#include "datalog_api_types.h"
#include "datalog_cli.h"
#include "datalog_api.h"

/**
* @brief Wraps a CLI literal into an API literal
* 
* @param lit Pointer to CLI literal object
* @return datalog_literal_t* Pointer to API literal object
*/
datalog_literal_t* dl_cli_wrap_literal(datalog_cli_literal_t* lit);

/**
* @brief Wraps a CLI command object into an API query object
* 
* @param command Pointer to CLI command object
* @return Pointer to new query object
*/
datalog_query_t* dl_cli_wrap_query(datalog_cli_command_t* command);

/**
* @brief Wraps a CLI command object into an API clause object
* 
* @param command Pointer to CLI command object
* @return datalog_clause_t* Pointer to new clause object
*/
datalog_clause_t* dl_cli_wrap_body(datalog_cli_command_t* command);

/**
* @brief Assersts a CLI command object using the API function set
* 
* @param command Pointer to CLI command object
* @return void
*/
void dl_cli_assert_command(datalog_cli_command_t* command);

/**
* @brief Assersts a CLI command object using the API function set,
* returning the output of the command
* 
* @param command Pointer to CLI command object
* @return char* Returns the output of the command
*/
char* dl_cli_assert_command_ret_str(datalog_cli_command_t* command);

#endif
