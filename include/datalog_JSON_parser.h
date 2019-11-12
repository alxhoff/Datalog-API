/**
 * @file        datalog_JSON_parser.c
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       Parser for parsing JSON files containing Datalog rules
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

#ifndef __KNOWLEDGELOADER_H__
#define __KNOWLEDGELOADER_H__

#include "cJSON.h"
#include "datalog_parser_types.h"

/**
 * @enum JSON_PARSER_ERR_t
 * @brief error messages
 * */
typedef enum { JSON_PARSER_OK = 0, JSON_PARSER_NOT_FOUND = -1, JSON_PARSER_MEM = -2, JSON_PARSER_INVAL = -3 } JSON_PARSER_ERR_t;

/**
 * @brief Parses the terms in a JSON literal
 *
 * @param terms JSON literal object pointer
 * @param lit Pointer to literal object that will contains parsed object
 * @return JSON_PARSER_ERR_t error message
 */
JSON_PARSER_ERR_t json_parser_terms(cJSON* terms, dl_parser_literal_t* lit);

/**
 * @brief Parses the head of a JSON rule
 *
 * @param head JSON head object pointer
 * @param rule Pointer to rule object that will contain parsed rule
 * @return JSON_PARSER_ERR_t error message
 */
JSON_PARSER_ERR_t json_parser_head(cJSON* head, dl_parser_rule_t* rule);

/**
 * @brief Parses the body of a JSON rule
 *
 * @param body JSON body object pointer
 * @param rule Pointer to rule object that will contain parsed rule
 * @return JSON_PARSER_ERR_t error message
 */
JSON_PARSER_ERR_t json_parser_body(cJSON* body, dl_parser_rule_t* rule);

/**
 * @brief Wraps a parser literal into a Datalog API literal
 *
 * @param dl_lit Datalog API literal to store the wrapped literal
 * @param parser_lit Parser literal object to be wrapped
 * @return JSON_PARSER_ERR_t error message
 */
JSON_PARSER_ERR_t json_parser_wrap_literal(datalog_literal_t* dl_lit, dl_parser_literal_t* parser_lit);

/**
 * @brief Asserts a parsed rule into the Datalog database
 *
 * @param rule Rule object to be asserted
 * @return JSON_PARSER_ERR_t error message
 */
JSON_PARSER_ERR_t json_parser_assert_rule(dl_parser_rule_t* rule);

/**
 * @brief Parses a JSON document, asserting rules found
 *
 * @param void
 * @return JSON_PARSER_ERR_t error message
 */
JSON_PARSER_ERR_t json_parser_runtime(char* filename);

#endif
