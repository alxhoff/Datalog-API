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
#include <string.h>

#include "datalog_cli_asserter.h"
#include "datalog_api.h"

datalog_term_t* dl_cli_wrap_term_list(datalog_term_t* term_head)
{
    datalog_term_t *tmp_term, *ret_head = NULL, *ret_head_walker;
    datalog_term_t* tmp_head = term_head;

    while(tmp_head != NULL){
        tmp_term = (datalog_term_t*)calloc(1, sizeof(datalog_term_t));
        if(tmp_term == NULL) return NULL;
        
        if(tmp_head->value != NULL){
            tmp_term->value = (char*)malloc(sizeof(char) * (strlen(tmp_head->value) + 1));
            if(tmp_term->value == NULL) return NULL;
            strcpy(tmp_term->value, tmp_head->value);
        }

        tmp_term->type = tmp_head->type;

        if(ret_head == NULL) {
            ret_head = tmp_term;
            ret_head_walker = tmp_term;
        }else{
            ret_head_walker->next = tmp_term;
            ret_head_walker = ret_head_walker->next;
        }
        tmp_head = tmp_head->next;
    }
    return ret_head;
}

datalog_literal_t* dl_cli_wrap_literal(datalog_cli_literal_t* lit)
{
    if(lit == NULL) return NULL;
    datalog_literal_t* ret = datalog_literal_init(lit->predicate);
    if(ret == NULL) return NULL;

    if(lit->predicate != NULL){
        ret->predicate = (char*)malloc(sizeof(char) * (strlen(lit->predicate) + 1));
        if(ret->predicate == NULL) return NULL;
        strcpy(ret->predicate, lit->predicate);
    }

    ret->term_count = lit->term_count;
    ret->term_head = dl_cli_wrap_term_list((datalog_term_t*)lit->term_head);
    return ret;
}

datalog_query_t* dl_cli_wrap_query(datalog_cli_command_t* command)
{
    datalog_query_t* ret = 
        (datalog_query_t*)calloc(1, sizeof(datalog_query_t));

    if(ret == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: wrapping cli query  alloc failed\n");
#endif
    }

    ret->literal = dl_cli_wrap_literal(command->head);

#ifdef CLI_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG][CLI] Verbose: command wrapped into query\n");
#endif

    return ret;
}

datalog_clause_t* dl_cli_wrap_body(datalog_cli_command_t* command)
{
    datalog_clause_t* ret = 
        (datalog_clause_t*)calloc(1, sizeof(datalog_clause_t));

    if(ret == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: wrapping cli body clause alloc failed\n");
#endif
        return NULL;
    }

    ret->head = (datalog_literal_t*)calloc(1, sizeof(datalog_literal_t));
    
    if(ret->head == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: wrapping cli body head litteral alloc failed\n");
#endif
        return NULL;
    }

    ret->head = dl_cli_wrap_literal(command->head);
    
    if(command->body_count)  
        ret->body_list = 
            (datalog_literal_t**)calloc(command->body_count, sizeof(datalog_literal_t*));

    for(int i = 0; i < command->body_count; i++)
        ret->body_list[i] = dl_cli_wrap_literal(command->body[i]);
    ret->literal_count = command->body_count; 
    return ret;    
}

void datalog_cli_free_term(datalog_cli_term_t** term)
{
    if(*term != NULL){
        if((*term)->value != NULL) free ((*term)->value);
        free(*term);
        *term = NULL;
    }
}
 
void datalog_cli_free_term_list(datalog_cli_term_t** list_head)
{
    datalog_cli_term_t *head, *prev;
    if(*list_head != NULL){
        head = *list_head;
        while(head != NULL){
            prev = head;
            head = head->next;
            datalog_cli_free_term(&prev);
        }
        *list_head = NULL;
    }
}

void datalog_cli_free_literal(datalog_cli_literal_t** lit)
{
    if((*lit) != NULL){
        if((*lit)->predicate!=NULL) {
            free((*lit)->predicate);
            (*lit)->predicate = NULL;
        }
        if((*lit)->term_head != NULL) {
            datalog_cli_free_term_list(&(*lit)->term_head);
            (*lit)->term_head = NULL;
        }
    free(*lit);
    *lit = NULL;
    }
}

void dl_cli_free_command(datalog_cli_command_t** command)
{
    if((*command)->head != NULL) datalog_cli_free_literal(&(*command)->head);
    if((*command)->body != NULL)
        for(int i =0; i < (*command)->body_count; i++)
            if((*command)->body[i] != NULL)
                datalog_cli_free_literal(&(*command)->body[i]);
    free(*command);
    *command = NULL;
}

void dl_cli_assert_command(datalog_cli_command_t* command)
{
    //type of command
    switch(command->cmd_type){
        case DL_CLI_FACT:{
            datalog_literal_t* lit = dl_cli_wrap_literal(command->head);
            datalog_literal_create_and_assert(lit);
            lit->free(&lit);
            }
            break;
        case DL_CLI_RULE:{
            datalog_clause_t* clause = dl_cli_wrap_body(command);
            datalog_clause_create_and_assert(clause);
            datalog_free_clause(&clause);
            }
            break;
        case DL_CLI_QUERY:{
            datalog_query_t* query = dl_cli_wrap_query(command);
            datalog_query_ask(query);
            if(query->processed_answer != NULL) datalog_query_print_answers(query);
            datalog_free_query(&query);
            }
            break;
        case DL_CLI_RETRACTION:{
            datalog_clause_t* clause = dl_cli_wrap_body(command);
            datalog_clause_create_and_retract(clause);
            datalog_free_clause(&clause);
            }
            break;
        default:
            break;
    }
    dl_cli_free_command(&command);
}

char* dl_cli_assert_command_ret_str(datalog_cli_command_t* command)
{
    //type of command
    char* ret_str = NULL;
    switch(command->cmd_type){
        case DL_CLI_FACT:{
            datalog_literal_t* lit = dl_cli_wrap_literal(command->head);
            if(datalog_literal_create_and_assert(lit) == DATALOG_OK){
                ret_str = (char*)malloc(sizeof(char) * (strlen("Fact asserted") + 1));
                if(ret_str == NULL) return NULL;
                strcpy(ret_str, "Fact asserted");
            }else{
                ret_str = 
                    (char*)malloc(sizeof(char) * (strlen("Fact assertion failed") + 1));
                if(ret_str == NULL) return NULL;
                strcpy(ret_str, "Fact assertion failed");
            }
            lit->free(&lit);
            }
            break;
        case DL_CLI_RULE:{
            datalog_clause_t* clause = dl_cli_wrap_body(command);
            if(datalog_clause_create_and_assert(clause) != DATALOG_OK){
                ret_str = (char*)malloc(sizeof(char) * (strlen("Rule asserted") + 1));
                if(ret_str == NULL) return NULL;
                strcpy(ret_str, "Rule asserted");
            }else{
                ret_str = 
                    (char*)malloc(sizeof(char) * (strlen("Rule assertion failed") + 1));
                if(ret_str == NULL) return NULL;
                strcpy(ret_str, "Rule assertion failed");
            }
            datalog_free_clause(&clause);
            }
            break;
        case DL_CLI_QUERY:{
            datalog_query_t* query = dl_cli_wrap_query(command);
            datalog_query_ask(query);
            if(query->processed_answer != NULL)
                ret_str = datalog_query_return_answers(query);
            datalog_free_query(&query);
            }
            break;
        case DL_CLI_RETRACTION:{
            datalog_clause_t* clause = dl_cli_wrap_body(command);
            if(datalog_clause_create_and_retract(clause) != DATALOG_OK){
                ret_str = (char*)malloc(sizeof(char) * (strlen("Retraction asserted") + 1));
                if(ret_str == NULL) return NULL;
                strcpy(ret_str, "Retraction asserted");
            }else{
                ret_str = 
                    (char*)malloc(sizeof(char) * (strlen("Retraction assertion failed") + 1));
                if(ret_str == NULL) return NULL;
                strcpy(ret_str, "Retraction assertion failed");
            }
            datalog_free_clause(&clause);
            }
            break;
        default:
            break;
    }
    //TODO FIX CRASH ON THIS LINE
    //dl_cli_free_command(&command);
    return ret_str;
}
