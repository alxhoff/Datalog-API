/**
 * @file        datalog_api.c
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       API functions to use the LUA/C datalog implementation found
 *              found at http://datalog.sourceforge.net
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

#include "datalog_api.h"

datalog_query_answer_t* datalog_process_answer(dl_answers_t a)
{
#ifdef DATALOG_DEBUG 
    fprintf(stderr, "[DATALOG] DEBUG: processing query answers\n"); 
#endif
    //alloc return struct
    datalog_query_answer_t* ret_struct = (datalog_query_answer_t*)
        malloc(sizeof(datalog_query_answer_t));

    if(ret_struct == NULL) return NULL;

    ret_struct->predic = dl_getpred(a);

#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: predicate:                          %s\n", 
        ret_struct->predic);
#endif
   
    //alloc list pointer
    ret_struct->answer_pairs = (datalog_query_answer_pair_t**)
        malloc(sizeof(datalog_query_answer_pair_t*));

    //alloc first list item
    ret_struct->answer_pairs[0] = (datalog_query_answer_pair_t*)
        malloc(sizeof(datalog_query_answer_pair_t));

    int index = 0;
    int arg1_len;
    //get first term
    char* tmp1 = dl_getconst(a, index, 0); 
    arg1_len = dl_getconstlen(a, index, 0);
    //get second term
    char* tmp2 = tmp1 + arg1_len + 1;


    ret_struct->answer_pairs[0]->arg1 = tmp1;
    ret_struct->answer_pairs[0]->arg2 = tmp2;

#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: first constant pair: (%s, %s)\n", 
            tmp1, tmp2);
#endif
    while(tmp1 != NULL){
        index++;
        
        tmp1 = dl_getconst(a, index, 0);
        if(tmp1 == NULL) break;
        arg1_len = dl_getconstlen(a, index, 0);
        tmp2 = tmp1 + arg1_len + 1;
        if(tmp2 == NULL) break;

#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: index %d constant pair: (%s, %s)\n", 
            index, tmp1, tmp2);
#endif
        ret_struct->answer_pairs = (datalog_query_answer_pair_t**)
            realloc((ret_struct->answer_pairs),
                    sizeof(datalog_query_answer_pair_t*) * (index + 1));

        ret_struct->answer_pairs[index] = (datalog_query_answer_pair_t*)
            malloc(sizeof(datalog_query_answer_pair_t));

        ret_struct->answer_pairs[index]->arg1 = tmp1;
        ret_struct->answer_pairs[index]->arg2 = tmp2;
    }
    ret_struct->argc = index;
#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: found \"%d\" answers\n", 
            ret_struct->argc);
#endif
    return ret_struct;
}

/*
DATALOG_ERR_t datalog_print_clause(datalog_clause_t* clause)
{
    if(clause == NULL){
        fprintf(stderr, "Clause is NULL, cannot print");
        return DATALOG_MEM;
    }

    printf("%s(%s, %s) :- ", clause->head->predicate,
            clause->head->arg1, clause->head->arg2);
    for(int i = 0; i < clause->literal_count; i++){
        if(i != (clause->literal_count - 1))
            printf("%s(%s, %s), ", clause->body_list[i]->predicate,
                    clause->body_list[i]->arg1, 
                    clause->body_list[i]->arg2);
        else
            printf("%s(%s, %s) ", clause->body_list[i]->predicate,
                    clause->body_list[i]->arg1, 
                    clause->body_list[i]->arg2);
    }

    printf("\n");

    return DATALOG_OK;
}
*/

/*
datalog_clause_t* datalog_init_clause(char* head_predicate,
        char* head_arg1, char* head_arg2, DATALOG_LIT_t head_lit_type)
{
    datalog_clause_t* ret_clause = 
        (datalog_clause_t*)malloc(sizeof(datalog_clause_t));

    if(ret_clause == NULL) return NULL;
    
    ret_clause->literal_count = 0;

    //body literal array
    ret_clause->body_list = 
        (datalog_literal_t**)malloc(sizeof(datalog_literal_t*));

    //head literal
    ret_clause->head = 
        (datalog_literal_t*)malloc(sizeof(datalog_literal_t));   

    ret_clause->head->lit_type = head_lit_type;

    ret_clause->head->predicate = 
        (char*)malloc(sizeof(char) * strlen(head_predicate));
    if(ret_clause->head->predicate == NULL) return NULL;
    strcpy(ret_clause->head->predicate, head_predicate);

    ret_clause->head->arg1 = 
        (char*)malloc(sizeof(char) * strlen(head_arg1));
    if(ret_clause->head->arg1 == NULL) return NULL;
    strcpy(ret_clause->head->arg1, head_arg1);
    
    ret_clause->head->arg2 = 
        (char*)malloc(sizeof(char) * strlen(head_arg2));
    if(ret_clause->head->arg2 == NULL) return NULL;
    strcpy(ret_clause->head->arg2, head_arg2);

    return ret_clause;
}
*/

/*
DATALOG_ERR_t datalog_clause_add_literal(datalog_clause_t* clause,
        char* predicate, char* arg1, char* arg2, DATALOG_LIT_t lit_type)
{
    clause->body_list = (datalog_literal_t**)realloc(clause->body_list,
            sizeof(datalog_literal_t*)*(clause->literal_count + 1)); 
    
    if(clause->body_list[clause->literal_count] != NULL)
        clause->body_list[clause->literal_count] = 
            (datalog_literal_t*)malloc(sizeof(datalog_literal_t));
    else
        return DATALOG_MEM;

    clause->body_list[clause->literal_count]->lit_type = lit_type;

    clause->body_list[clause->literal_count]->predicate = 
        (char*)malloc(sizeof(char) * strlen(predicate));
    if(clause->body_list[clause->literal_count]->predicate == NULL)
        return DATALOG_MEM;
    strcpy(clause->body_list[clause->literal_count]->predicate, predicate);
    
    clause->body_list[clause->literal_count]->arg1 = 
        (char*)malloc(sizeof(char) * strlen(arg1));
    if(clause->body_list[clause->literal_count]->arg1 == NULL)
        return DATALOG_MEM;
    strcpy(clause->body_list[clause->literal_count]->arg1, arg1);
     
    clause->body_list[clause->literal_count]->arg2 = 
        (char*)malloc(sizeof(char) * strlen(arg2));
    if(clause->body_list[clause->literal_count]->arg2 == NULL)
        return DATALOG_MEM;
    strcpy(clause->body_list[clause->literal_count]->arg2, arg2);

    clause->literal_count++;

    return DATALOG_OK;
}
*/

DATALOG_ERR_t datalog_clause_add_literal_s(datalog_clause_t* clause,
        datalog_literal_t* literal) 
{
    clause->body_list = (datalog_literal_t**)realloc(clause->body_list,
            sizeof(datalog_literal_t*)*(clause->literal_count + 1)); 
    
    if(clause->body_list[clause->literal_count] != NULL)
        clause->body_list[clause->literal_count] = literal;
    else
        return DATALOG_MEM;
    
    clause->literal_count++;

    return DATALOG_OK;
}

DATALOG_ERR_t datalog_clause_add_literal_s_copy(datalog_clause_t* clause,
        datalog_literal_t* literal) 
{
    clause->body_list = (datalog_literal_t**)realloc(clause->body_list,
            sizeof(datalog_literal_t*)*(clause->literal_count + 1)); 
    
    if(clause->body_list[clause->literal_count] != NULL)
        clause->body_list[clause->literal_count] = 
            (datalog_literal_t*)malloc(sizeof(datalog_literal_t));
    else
        return DATALOG_MEM;
    
    memcpy(clause->body_list[clause->literal_count],
            literal, sizeof(datalog_literal_t));

    clause->literal_count++;

    return DATALOG_OK;
}

/*
DATALOG_ERR_t datalog_create_and_assert_clause_s(datalog_clause_t* clause)
{
    DATALOG_ERR_t ret = 0;
    
    //create head on the stack
    if(datalog_create_literal_s(clause->head) != DATALOG_OK){
#ifdef DATALOG_ERR
            fprintf(stderr, "[DATALOG][API] Err: failed to assert clause head \n");
#endif
        return DATALOG_LIT;
    }
#ifdef DATALOG_DEBUG_VERBOSE
        fprintf(stderr, "[DATALOG][API] Verbose: create and assert clause, "
                "head literal created\n");
#endif

    ret = dl_pushhead(datalog_db);

    if(ret){
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][API] Err: asserting clause head failed\n");
#endif
        return DATALOG_ASRT;
    }
        
    //Create clause literals 
    for(int i = 0; i < clause->literal_count; i++){
        ret = datalog_create_literal_s(clause->body_list[i]);
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][API] Err: creating clause literal #%d:        %s\n"
                , i, (ret == DATALOG_OK ? "SUCCSESS" : "FAIL"));
#endif
        if(ret != DATALOG_OK) return DATALOG_LIT;
        
        ret = dl_addliteral(datalog_db);

#ifdef PARSER_DEBUG_VERBOSE
        fprintf(stderr, "[DATALOG][API] Verbose: asserting clause literal #%d    %s\n"
                , i, (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif
        if(ret) return DATALOG_ASRT;
    }

    ret = dl_makeclause(datalog_db);

#ifdef PARSER_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG][PARSER] Verbose: asserting clause:           %s\n",
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif
    if(ret) return DATALOG_ASRT;

    return DATALOG_OK;
}
*/

DATALOG_ERR_t datalog_assert_clause(int literal_count)
{
    int ret = 0;

    //create empty clause
    ret = dl_pushhead(datalog_db);
    
#ifdef DATALOG_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG] VERBOSE: empty clause created:               %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

    for(int i = 0; i < literal_count; i++){
        ret = dl_addliteral(datalog_db);

#ifdef DATALOG_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG] VERBOSE: literal %d added to clause:         %s\n",
        i, (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif
    }

    //finalise the clause
    ret = dl_makeclause(datalog_db);

#ifdef DATALOG_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG] VERBOSE: clause finalised:                   %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

    //assert clause
    ret = dl_assert(datalog_db);

#ifdef DATALOG_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG] VERBOSE: clause asserted:                    %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif
    
    return DATALOG_OK;
}

/*
DATALOG_ERR_t datalog_create_and_retract_clause_s(datalog_clause_t* clause)
{
    //create head on the stack
    if(datalog_create_literal_s(clause->head) != DATALOG_OK){
#ifdef DATALOG_ERR
            fprintf(stderr, "[DATALOG][API] Err: failed to assert clause head \n");
#endif
        return DATALOG_LIT;
    }

    //assert clause
    if(datalog_retract_clause() != DATALOG_OK){
#ifdef DATALOG_ERR
        fprintf(stderr, "[DATALOG][API] Err: failed to retract "
                "clause with #%d literals\n", clause->literal_count +1);
#endif
        return DATALOG_ASRT;
    }
    return DATALOG_OK;
}
*/

DATALOG_ERR_t datalog_retract_clause(void)
{
    int ret = 0;

    //create clause from complete literal
    ret = dl_pushhead(datalog_db);
    
#ifdef DATALOG_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG] VERBOSE: empty clause created:               %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

    //finalise the clause
    ret = dl_makeclause(datalog_db);

#ifdef DATALOG_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG] VERBOSE: clause finalised:                   %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

    //retract clause
    ret = dl_retract(datalog_db);

#ifdef DATALOG_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG] VERBOSE: clause retracted:                   %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

    return DATALOG_OK;
}

void datalog_print_answers(datalog_query_answer_t* a)
{
#ifdef DATALOG_DEBUG 
    fprintf(stderr, "[DATALOG] DEBUG: printing query answers\n"); 
#endif
    printf("!!-----------DATALOG ANSWERS-----------!!\n");
    printf(" Predicate: %s\n", a->predic);
    printf(" Answer contains \"%d\" answers\n", a->argc);
    for(int i = 0; i < a->argc; i++)
        printf("Answer #%d: %s( %s, %s)\n", i, a->predic,
                a->answer_pairs[i]->arg1, a->answer_pairs[i]->arg2);
}

datalog_query_t* datalog_query_init(datalog_literal_t* lit)
{
    datalog_query_t* query = (datalog_query_t*)malloc(sizeof(datalog_query_t));

    if(query == NULL) return NULL;

    query->literal = lit;

    return query;
}

DATALOG_ERR_t datalog_query_print(datalog_query_t* query)
{
    printf("!!=========Query=========!!\n");
    datalog_literal_print(query->literal);
    //TODO print answers
    return DATALOG_OK;
}

DATALOG_ERR_t datalog_query_ask(datalog_query_t* query)  
{

    
#ifdef DATALOG_DEBUG 
    fprintf(stderr, "[DATALOG] DEBUG: query clause for predicate: %s\n", query->literal->predicate); 
#endif
   
    DATALOG_ERR_t ret = DATALOG_OK;

    ret = datalog_literal_create(query->literal);
  
#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: query literal created:              %s\n", 
        (ret == DATALOG_OK ? "SUCCSESS" : "FAIL"));
#endif

    dl_answers_t a;
    
    ret = dl_ask(datalog_db, &a);

#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: query sent:                         %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif
#ifdef DATALOG_DEBUG 
    fprintf(stderr, "[DATALOG] VERBOSE: query resulted in answer:           %s\n", 
        (a != NULL ? "SUCCSESS" : "FAIL"));
#endif
  
    if(a != NULL){
        datalog_query_answer_t* processed = 
            datalog_process_answer(a);
    }

    return DATALOG_OK;
}

/*
DATALOG_ERR_t datalog_query_s(datalog_query_t* query)
{
#ifdef DATALOG_DEBUG 
    fprintf(stderr, "[DATALOG] DEBUG: asserting query %s(%s, %s)\n", 
            query->literal->predicate, query->literal->arg1, query->literal->arg2); 
#endif
   
    DATALOG_ERR_t ret = DATALOG_OK;

    ret = datalog_create_literal_s(query->literal);
  
#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: query literal created:              %s\n", 
        (ret == DATALOG_OK ? "SUCCSESS" : "FAIL"));
#endif

    dl_answers_t a;
    
    ret = dl_ask(datalog_db, &a);

#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: query sent:                         %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif
#ifdef DATALOG_DEBUG 
    fprintf(stderr, "[DATALOG] VERBOSE: query resulted in answer:           %s\n", 
        (a != NULL ? "SUCCSESS" : "FAIL"));
#endif
  
    query->answer = (dl_answers_t*)calloc(1, sizeof(dl_answers_t));
    
    if(a!= NULL){
        memcpy(query->answer, &a, sizeof(dl_answers_t));
  
        query->processed_answer = datalog_process_answer(a); 
    }

    return DATALOG_OK;
}
*/

datalog_term_t* datalog_literal_get_term_index(datalog_literal_t* lit,
        int index)
{
    datalog_term_t* term_head = lit->term_head;
    if(index == 0) return term_head;
    for(int i=0; i<index; i++){
        if(term_head->next != NULL) term_head = term_head->next;
        else return NULL;
    }
    return term_head;
}

datalog_term_t* datalog_literal_get_last_term(datalog_literal_t* lit)
{
    if(lit->term_head == NULL) return NULL;
    datalog_term_t* term_head = lit->term_head;
    while(term_head->next != NULL) term_head = term_head->next;
    return term_head;
}

DATALOG_ERR_t datalog_literal_add_term(datalog_literal_t* lit, char* value, 
        DATALOG_TERM_t type)
{
    datalog_term_t* term = (datalog_term_t*)calloc(1, sizeof(datalog_term_t));
    if(term == NULL) return DATALOG_MEM;
    term->type = type;
    term->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
    if(term->value == NULL){
        free(term);
        return DATALOG_MEM;
    }
    strcpy(term->value, value);
    datalog_term_t* list_tail = datalog_literal_get_last_term(lit);
    if(list_tail == NULL) lit->term_head = term;
    else list_tail->next = term;
    lit->term_count++;
    return DATALOG_OK;
}

datalog_literal_t* datalog_literal_init(char* predicate)
{
    datalog_literal_t* lit = 
        (datalog_literal_t*)calloc(1, sizeof(datalog_literal_t));
    if(lit == NULL) return NULL;
    lit->predicate = (char*)malloc(sizeof(char) * (strlen(predicate) + 1));
    if(lit->predicate == NULL){
        free(lit);
        return NULL;
    }
    strcpy(lit->predicate, predicate);
    return lit;
}

DATALOG_ERR_t datalog_literal_print(datalog_literal_t* lit)
{
    printf("!!========LITERAL========!!\n");
    printf("  Predicate: %s\n", (lit->predicate != NULL) ? lit->predicate : "NULL");
    if(lit->term_head == NULL){
        printf("  No terms\n");
        goto print_literal_return;
    }
    
    datalog_term_t* term_head = lit->term_head;
    
    for(int i = 0; i<lit->term_count; i++){
        printf("  Term #%d: %s\n", i, term_head->value); 
        term_head = term_head->next;
    }

print_literal_return: printf("!!========LITERAL========!!\n");
    return DATALOG_OK;
}

DATALOG_ERR_t datalog_literal_create(datalog_literal_t* lit)
{
    DATALOG_ERR_t ret = DATALOG_OK;
    
    //start literal, push empty literal onto stack
    ret = dl_pushliteral(datalog_db);

#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: empty literal pushed onto stack:    %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

    if(ret) return DATALOG_LIT;

    //push predicate symbol onto the stack via string
    ret = dl_pushlstring(datalog_db, lit->predicate, 
            (size_t)strlen(lit->predicate));
        
#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: predicate string pushed onto stack: %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

    if(ret) return DATALOG_LIT;
   
    ret = dl_addpred(datalog_db); 

#ifdef DATALOG_DEBUG_VERBOSE 
    fprintf(stderr, "[DATALOG] VERBOSE: predicate created:                  %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

    if(ret) return DATALOG_LIT;

    //push args onto the stack
    datalog_term_t* tmp;
    for(int i = 0; i < lit->term_count; i++){
        tmp = datalog_literal_get_term_index(lit, i);
        if(tmp != NULL) ret = dl_pushlstring(datalog_db, tmp->value,
                (size_t)strlen(tmp->value));

#ifdef DATALOG_DEBUG_VERBOSE 
        fprintf(stderr, "[DATALOG] VERBOSE: term #%d string pushed onto stack:   %s\n" 
            , i, (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

        if(ret) return DATALOG_LIT;

        if(tmp->type == DL_TERM_C){
            ret = dl_addconst(datalog_db);
#ifdef DATALOG_DEBUG_VERBOSE 
            fprintf(stderr, "[DATALOG] VERBOSE: term #%d added as constant:          %s\n" 
                , i, (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif
        }else if(tmp->type == DL_TERM_V){
            ret = dl_addvar(datalog_db);
#ifdef DATALOG_DEBUG_VERBOSE 
            fprintf(stderr, "[DATALOG] VERBOSE: term #%d added as variable:          %s\n" 
                , i, (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif
        }else return DATALOG_TERM;
        
#ifdef DATALOG_DEBUG_VERBOSE
        fprintf(stderr, "[DATALOG] VERBOSE: term %d created:                     %s\n", 
            i, (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

        if(ret) return DATALOG_LIT;
    }
    
    //finish creating literal
    ret = dl_makeliteral(datalog_db);

#ifdef DATALOG_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG] VERBOSE: literal created:                    %s\n", 
        (ret == 0 ? "SUCCSESS" : "FAIL"));
#endif

   if(ret) return DATALOG_LIT;

   return DATALOG_OK;
}


DATALOG_ERR_t datalog_literal_create_and_assert(datalog_literal_t* lit)
{
    if(datalog_literal_create(lit) != DATALOG_OK) return DATALOG_LIT;
    if(datalog_assert_clause(0) != DATALOG_OK) return DATALOG_ASRT;

    return DATALOG_OK;
}

DATALOG_ERR_t datalog_push_string(char* string)
{
    
    return DATALOG_OK;
}

DATALOG_ERR_t datalog_engine_db_init(void)
{
   datalog_db = dl_open();
   
#ifdef DATALOG_ERR
    if(datalog_db == NULL){
        fprintf(stderr, "[DATALOG] ERR: dl_open() failed\n");
        return DATALOG_MEM;
    }
#ifdef DATALOG_DEBUG
    if(datalog_db != NULL)
        fprintf(stderr, "[DATALOG] DEBUG: database opened \n");
#endif
#endif

    return DATALOG_OK;
}

DATALOG_ERR_t datalog_engine_db_deinit(void)
{
    dl_close(datalog_db);

#ifdef DATALOG_DEBUG
    printf("[DATALOG] DEBUG: database closed\n");
#endif

    return DATALOG_OK;
}
