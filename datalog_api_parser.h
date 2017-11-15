/**
 * @file        datalog_api_parser.h
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       Wrapper for the Datalog API parser into the Datalog API
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

#ifndef __DATALOG_API_PARSER_H__
#define __DATALOG_API_PARSER_H__

#include "datalog_api.h"
#include "datalog_parser.h"

/**
 * @brief wraps the rule list from the passer into the API and
 * assersts it
 *
 * The linked list of rules is traversed and wrapped into the 
 * datalog_clause_t datatypes
 *
* @param doc pointer to parser document stuct
 * @return DATALOG_ERR_t error message
 */
DATALOG_ERR_t datalog_assert_rule_list(dl_parser_doc_t* doc);

/**
 * @brief wraps the fact list from the passer into the API and
 * assersts it
 *
 * The linked list of facts is traversed and wrapped into the 
 * datalog_clause_t datatypes
 *
* @param doc pointer to parser document stuct
 * @return DATALOG_ERR_t error message
 */
DATALOG_ERR_t datalog_assert_fact_list(dl_parser_doc_t* doc);

/**
 * @brief wraps a dl_parser_literal_t object into a
 * datalog_literal_t object
 *
 * @param literal pointer to the literal that is to be wrapped
 * @return datalog_literal_t* literal object used to store the 
 * wrapped literal
 */
datalog_literal_t* datalog_wrap_literal(dl_parser_literal_t* literal);

/**
 * @brief wraps a dl_parser_rule_t object into a datalog_clause_t
 * object
 *
 * @param rule pointer to the rule that is to be wrapped
 * @return datalog_clause_t* clause object used to store the
 * wrapped rule
 */
datalog_clause_t* datalog_wrap_rule(dl_parser_rule_t* rule);

/**
 * @brief wraps a dl_parser_fact_t object into a datalog_literal_t
 * object
 *
 * @param fact pointer to the fact that is to be wrapped
 * @return datalog_literal_t* literal object used to store the 
 * wrapped fact
 */
datalog_literal_t* datalog_wrap_fact(dl_parser_fact_t* fact);

#endif
