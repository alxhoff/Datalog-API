/**
 * @file        datalog_api_parser.c
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

#include <stdlib.h>
#include <string.h>

#include "datalog_api_parser.h"

DATALOG_ERR_t datalog_parser_assert_doc(dl_parser_return_doc_t* doc)
{
    DATALOG_ERR_t ret = DATALOG_OK;

    ret = datalog_assert_fact_list(doc);
    if(ret != DATALOG_OK) return DATALOG_ASRT;

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Debug: fact list asserted\n");
#endif
     
    ret = datalog_assert_rule_list(doc);
    if(ret != DATALOG_OK) return DATALOG_ASRT;

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Debug: rule list asserted\n");
#endif

    return DATALOG_OK;
}

DATALOG_ERR_t datalog_assert_rule_list(dl_parser_return_doc_t* doc)
{
    DATALOG_ERR_t ret = DATALOG_OK;

    dl_parser_rule_t* dl_p_rule_tmp = doc->rules_head;
    dl_parser_rule_t* rule_to_free;
    datalog_clause_t* clause_tmp;

    while(dl_p_rule_tmp != NULL){
        
        clause_tmp = datalog_wrap_rule(dl_p_rule_tmp);
       
#ifdef PARSER_DEBUG_VERBOSE
        fprintf(stderr, "[DATALOG][PARSER] Verbose: rule clause wrapped\n");
#endif

        ret = datalog_clause_create_and_assert(clause_tmp);
        
        if(ret != DATALOG_OK){
#ifdef DATALOG_ERR
            fprintf(stderr, "[DATALOG][API] Err: failed to assert"
                    " clause from rule list\n");
#endif
            return DATALOG_ASRT;
        }

#ifdef PARSER_DEBUG_VERBOSE
        fprintf(stderr, "[DATALOG][PARSER] Verbose: rule asserted\n");
#endif
        rule_to_free = dl_p_rule_tmp;
        dl_p_rule_tmp = dl_p_rule_tmp->next;
    }
    return DATALOG_OK;
}

DATALOG_ERR_t datalog_assert_fact_list(dl_parser_return_doc_t* doc)
{
    DATALOG_ERR_t ret = DATALOG_OK;

    dl_parser_fact_t* dl_p_fact_tmp = doc->facts_head;
    datalog_literal_t* lit_tmp;

    while(dl_p_fact_tmp != NULL){
        
        lit_tmp = datalog_wrap_fact(dl_p_fact_tmp);

        if(lit_tmp == NULL) return DATALOG_WRAP; 
        ret = (DATALOG_ERR_t)datalog_literal_create_and_assert(lit_tmp);

        if(ret != DATALOG_OK){

#ifdef DATALOG_ERR
            fprintf(stderr, "[DATALOG][API] Err: failed to assert"
                    " literal from fact list\n");
#endif
        }   
            dl_p_fact_tmp = dl_p_fact_tmp->next;
    }

    return DATALOG_OK;
}

// wrap data types
datalog_literal_t* datalog_wrap_literal(dl_parser_literal_t* literal)
{
    datalog_literal_t* ret = 
        (datalog_literal_t*)calloc(1, sizeof(datalog_literal_t));

    if(ret == NULL){
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][API] Err: wraping literal alloc failed\n"); 
#endif
        return NULL;
    }

    ret->predicate = 
        (char*)malloc(sizeof(char)*strlen(literal->predicate));
    if(ret->predicate == NULL){
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][WRAP] Err: wrapping literal"
            " predicate failed\n");
#endif
        return NULL;
    }
    
    strcpy(ret->predicate, literal->predicate);
    ret->term_count = literal->term_count;

    if(literal->term_head == NULL) return ret;
    dl_parser_term_t* term_to_wrap = literal->term_head;
    
    datalog_term_t *head, *prev;

    for(int i = 0; i < literal->term_count; i++){
        if(term_to_wrap == NULL) return ret;
        head = (datalog_term_t*)calloc(1,sizeof(datalog_term_t));
        if(head == NULL) return NULL;
        memcpy(head, term_to_wrap, sizeof(datalog_term_t));
        if(i > 0) prev->next = head;
        else ret->term_head = head;
        prev = head;
        term_to_wrap = term_to_wrap->next; 
    }
    return ret;
}

datalog_clause_t* datalog_wrap_rule(dl_parser_rule_t* rule)
{
    datalog_clause_t* ret =
        (datalog_clause_t*)calloc(1, sizeof(datalog_clause_t));

    if(ret == NULL){
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][API] Err: wraping rule alloc failed\n"); 
#endif
        return NULL;
    }

    //wrap head
    ret->head = datalog_wrap_literal(rule->head);
    
    if(ret->head == NULL){
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][API] Err: wraping rule head"
                " failed\n");
#endif
        return NULL;
    }

#ifdef PARSER_VERBOSE
    fprintf(stderr, "[DATALOG][PARSER] Verbose: rule head wrapped for assertion\n");
#endif

    ret->body_list = (datalog_literal_t**)calloc(
            rule->body->literal_count,
            sizeof(datalog_literal_t*));

    if(ret->body_list == NULL){
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][API] Err: rule body alloc"
                " failed\n");
#endif
        return NULL;
    }

    //wrap body
    for(int i = 0; i < rule->body->literal_count; i++){
        ret->body_list[i] = datalog_wrap_literal(
                rule->body->literals[i]);
    }

    ret->literal_count = rule->body->literal_count;

    return ret;
}

datalog_literal_t* datalog_wrap_fact(dl_parser_fact_t* fact)
{
    datalog_literal_t* ret = datalog_wrap_literal(fact->literal); 

    if(ret == NULL){
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][API] Err: wraping fact alloc failed\n"); 
#endif
        return NULL;
    }
  
    return ret;
}
