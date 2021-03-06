/**
 * @file        datalog_api_types.h
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       Datatypes used across the API
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

#ifndef __DATALOG_API_TYPES_H__
#define __DATALOG_API_TYPES_H__

/**
* @enum DATALOG_LIT_t
* @brief used to specify the type of literal to be created
*/
typedef enum{
    DL_CC,  /*!< constant constant */
    DL_CV,  /*!< constant variable */
    DL_VV,  /*!< variable variable */
    DL_VC   /*!< variaben constant */
} DATALOG_LIT_t;

typedef enum{
    DL_TERM_C = 0x00,
    DL_TERM_V = 0x01,
} DATALOG_TERM_t;

typedef struct datalog_term datalog_term_t;

struct datalog_term{
    char* value;
    DATALOG_TERM_t type;
    datalog_term_t* next;
};

/**
* @typedef datalog_literal_t
* @brief Typdef for datalog_literal
*/
typedef struct datalog_literal datalog_literal_t;

/**
* @struct datalog_literal
* @brief Struct to represent a datalog literal
*/
struct datalog_literal{
    char* predicate; /**< Literal's precidate string represenation*/
    int term_count;
    datalog_term_t* term_head;      /**< String representation of the literals first arg*/

    int (*add_term)(datalog_literal_t*, char*, DATALOG_TERM_t);
    int (*set_predicate)(datalog_literal_t*, char*);
    void (*clear_terms)(datalog_literal_t*);
    int (*print)(datalog_literal_t*);
    int (*assert)(datalog_literal_t*);
    void (*free)(datalog_literal_t**);
};

#endif
