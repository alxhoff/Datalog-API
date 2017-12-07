/**
 * @file        datalog_cli.h
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       Command line style interface for the Datalog API
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

#ifndef __DATALOG_CLI_H__
#define __DATALOG_CLI_H__

#include "datalog_api_types.h"

/**
* @enum DATALOG_CLI_HEAD_TYPE_t
* @brief used to specify the type of literal to be created
*/
typedef enum{
    DL_CLI_HEAD_QUERY,      /*!< If a literal ends in a '?' then the literal is a query */
    DL_CLI_HEAD_STATEMENT,   /*!< If a literal ends in a '.' then the literal is either a fact or a rule */
    DL_CLI_HEAD_RETRACTION  /*!< If a literal ends in a '~' then the literal is a retraction */
} DATALOG_CLI_HEAD_TYPE_t;

/**
* @enum DATALOG_CLI_CMD_TYPE_t
* @brief used to specify the type of literal to be created
*/
typedef enum{
    DL_CLI_FACT,            /*!< Denotes a clause that is a fact */
    DL_CLI_RULE,            /*!< Denotes a clause that is a rule*/
    DL_CLI_QUERY,           /*!< Denotes a clause that is a query */
    DL_CLI_RETRACTION       /*!< Denotes a clause that is a retraction */
} DATALOG_CLI_CMD_TYPE_t;

/**
* @enum DATALOG_TERM_TYPE_t
* @brief used to specify the type of literal to be created
*/
typedef enum{
    DL_CLI_CONSTANT = 0x00,     /*!< Denotes a literal term that is a constant */
    DL_CLI_VARIABLE = 0x01,    /*!< Denotes a literal term that is a variable */
} DATALOG_TERM_TYPE_t;

/**
* @enum DATALOG_CLI_ERR_t
* @brief used to specify the type of literal to be created
*/
typedef enum{
    DATALOG_CLI_OK = 0,     /*!< No errors */
    DATALOG_CLI_EXIT,       /*!< Exit has been called */
    DATALOG_CLI_HELP,       /*!< Help has been called */
    DATALOG_CLI_INVAL,      /*!< Invalid argument */
    DATALOG_CLI_SPLIT,      /*!< Spliting of the head and body failed */
    DATALOG_CLI_LIT,        /*!< Parsing literal string failed */
    DATALOG_CLI_MEM,        /*!< Allocating memory failed */
    DATALOG_CLI_SYNTAX      /*!< Syntax error */
} DATALOG_CLI_ERR_t;

/**
* @typedef datalog_cli_unprocessed_body_t
* @brief Typdef for struct datalog_cli_unprocessed_body
*/
/**
* @struct datalog_cli_unprocessed_body
* @brief Represents a body input string that has been split into
* individual literal strings
*/
typedef struct datalog_cli_unprocessed_body{
    int lit_count;          /*!< Number of literal strings to be parsed */
    char** body_literals;   /*!< Pointer array of literal strings to be parsed */
} datalog_cli_unprocessed_body_t;

typedef struct datalog_cli_term datalog_cli_term_t;

struct datalog_cli_term{
    char* value;
    DATALOG_TERM_TYPE_t type;
    datalog_cli_term_t* next;
};

/**
* @typedef datalog_cli_literal_t
* @brief Typdef for struct datalog_cli_literal
*/
/**
* @struct datalog_cli_literal
* @brief Represents a literal
*/
typedef struct datalog_cli_literal{
    char* predicate;        /*!< Literal's predicate */
    int term_count;
    datalog_cli_term_t* term_head; 
} datalog_cli_literal_t;

/**
* @typedef datalog_cli_command_t
* @brief Typdef for struct datalog_cli_command
*/
/**
* @struct datalog_cli_command
* @brief Object representing an inputted command to be asserted
*/
typedef struct datalog_cli_command{
    datalog_cli_literal_t* head;        /*!< Head literal */
    DATALOG_CLI_HEAD_TYPE_t head_type;  /*!< Head literal's type */

    datalog_cli_literal_t** body;       /*!< Pointer array pointing to body literals */
    int body_count;                     /*!< Number of body literals */

    DATALOG_CLI_CMD_TYPE_t cmd_type;    /*!< Type of clause that makes up the command */
} datalog_cli_command_t;

/**
* @brief Removes all the whitespaces from the inputted command string
* 
* @param input Command string to be parsed
* @return char* Command string with whitespaces removed
*/
char* dl_cli_remove_spaces(char* input);

/**
* @brief Splits a command input string into head and body
* 
* @param line Input command line that is to be split
* @param head Pointer to string that is to store the head string
* @param body Pointer to string that is to store the body string
* @return 
*/
DATALOG_CLI_ERR_t dl_cli_split_line(char* line, char** head, char** body);

/**
* @brief Splits an input body string into seperate literal strings
* 
* @param body Input string containing all the bodie's literals
* @return datalog_cli_unprocessed_body_t* object that contains individual
* literal strings as well as a count of how many literals the body
* contains
*/
datalog_cli_unprocessed_body_t* dl_cli_get_body_literals(
        char* body);

/**
* @brief Determines if a string is a constant of a variable
*
* Checks the first letter of a string to see if it is capitalised.
* If it is then the string represents a variable, if not then the
* string represents a constant.
* 
* @param term Term string that is to be checked
* @return DATALOG_TERM_TYPE_t enum representing the type
*/
DATALOG_TERM_TYPE_t dl_cli_process_string_type(char* term);

/**
* @brief Processes a literal string into a literal object
* 
* @param lit_string String that is to be processed
* @return datalog_cli_literal_t* Pointer to literal object
*/
datalog_cli_literal_t* dl_cli_process_literal(char* lit_string);

/**
* @brief Processes a list of body strings into a list of literal
* objects
* 
* @param body_list Object containing the unprocessed body literal
* strings
* @param command Pointer to the object that will store the processed
* body literals
* @return DATALOG_CLI_ERR_t error message
*/
DATALOG_CLI_ERR_t dl_cli_process_body_list(
        datalog_cli_unprocessed_body_t* body_list, datalog_cli_command_t* command);

/**
* @brief Processes the head string of in inputted command and
* returns a command object with the head literal populated
* 
* @param head_string Head string from the inputted command
* @return datalog_cli_command_t* Command object pointer
*/
datalog_cli_command_t* dl_cli_process_head(char* head_string);

/**
* @brief Parses a line of input
*
* The provided string will act as a line of input to the CLI.
* This line is then parsed and asserted.
* 
* @param line String representing a line of input to the CLI
* @return DATALOG_CLI_ERR_t error message 
*/
DATALOG_CLI_ERR_t dl_cli_parse_line(char* line);

/**
* @brief Prints the CLI's help message
* 
* @param void
* @return void
*/
void dl_cli_print_help(void);

#endif
