/**
 * @file        datalog_cli_asserter.c
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

#include <stdlib.h>

#include "datalog_cli_asserter.h"

datalog_literal_t* dl_cli_wrap_literal(datalog_cli_literal_t* lit)
{
    return (datalog_literal_t*)lit;
}

datalog_query_t* dl_cli_wrap_query(datalog_cli_command_t* command)
{
    datalog_query_t* ret = 
        (datalog_query_t*)malloc(sizeof(datalog_query_t));

    if(ret == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: wrapping cli query  alloc failed\n");
#endif
    }

    ret->literal = dl_cli_wrap_literal(command->head);

    return ret;
}

datalog_clause_t* dl_cli_wrap_body(datalog_cli_command_t* command)
{
    datalog_clause_t* ret = 
        (datalog_clause_t*)malloc(sizeof(datalog_clause_t));

    if(ret == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: wrapping cli body clause alloc failed\n");
#endif
        return NULL;
    }

    ret->head = (datalog_literal_t*)malloc(sizeof(datalog_literal_t));
    
    if(ret->head == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: wrapping cli body head litteral alloc failed\n");
#endif
        return NULL;
    }

    ret->head = dl_cli_wrap_literal(command->head);

    ret->body_list = 
        (datalog_literal_t**)calloc(command->body_count, sizeof(datalog_literal_t*));

    for(int i = 0; i < command->body_count; i++)
        ret->body_list[i] = dl_cli_wrap_literal(command->body[i]);

    return ret;    
}

void dl_cli_assert_command(datalog_cli_command_t* command)
{
    //type of command
    switch(command->cmd_type){
        case DL_CLI_FACT:
            datalog_create_and_assert_literal_s(
                    dl_cli_wrap_literal(command->head));
            break;
        case DL_CLI_RULE:{
            datalog_clause_t* clause = 
                dl_cli_wrap_body(command);
            datalog_create_and_assert_clause_s(clause);
            free(clause->head);
            free(clause->body_list);
            free(clause);
            }
            break;
        case DL_CLI_QUERY:{
            datalog_query_t* query = dl_cli_wrap_query(command);
            datalog_query_s(query);
            free(query);
            }
            break;
        case DL_CLI_RETRACTION:
            break;
        default:
            break;
    }
    free(command);
}
