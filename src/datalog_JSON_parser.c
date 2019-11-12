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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datalog_api.h"
#include "datalog_JSON_parser.h"

#define CMP_STRING(JSON_OBJ, STRING)    \
    if(!strcmp(JSON_OBJ->string, #STRING)) 

#define CMP_VALUESTRING(JSON_OBJ, VALUESTRING)    \
    if(!strcmp(JSON_OBJ->valuestring, #VALUESTRING)) 

#define CMP_NOT_VALUESTRING(JSON_OBJ, VALUESTRING)    \
    if(strcmp(JSON_OBJ->valuestring, #VALUESTRING)) 

JSON_PARSER_ERR_t json_parser_terms(cJSON* terms, dl_parser_literal_t* lit)
{
    int term_count = 0;
    cJSON* tmp_json_term = NULL;

    char* tmp_str;
    dl_parser_term_t *tmp_term, *new_term;

    cJSON_ArrayForEach(tmp_json_term, terms->child){
        CMP_STRING(tmp_json_term, variable){
            //process variable term
            new_term = (dl_parser_term_t*)calloc(1, sizeof(dl_parser_term_t));
            if(new_term == NULL) return JSON_PARSER_MEM;
            if(lit->term_head == NULL) lit->term_head = new_term;
            else{
                tmp_term = lit->term_head;
                while(tmp_term->next != NULL) tmp_term = tmp_term->next;
                tmp_term->next = new_term;
            }
            new_term->type = DL_PARSE_TERM_V; 
#ifdef PARSER_DEBUG_VERBOSE
            printf("Variable term found\n");
#endif
            goto parse_terms_set_contents;
        }else CMP_STRING(tmp_json_term, value){
            //process constant term
            new_term = (dl_parser_term_t*)calloc(1, sizeof(dl_parser_term_t));
            if(new_term == NULL) return JSON_PARSER_MEM;
            if(lit->term_head == NULL) lit->term_head = new_term;
            else{
                tmp_term = lit->term_head;
                while(tmp_term->next != NULL) tmp_term = tmp_term->next;
                tmp_term->next = new_term;
            }
            new_term->type = DL_PARSE_TERM_C; 
#ifdef PARSER_DEBUG_VERBOSE
            printf("Constant term found\n");
#endif
parse_terms_set_contents:
            new_term->value = (char*)malloc(sizeof(char) * 
                    (strlen((const char*)tmp_json_term->valuestring) + 1));
            if(new_term->value == NULL) return JSON_PARSER_MEM;
            strcpy(new_term->value, (const char*)tmp_json_term->valuestring);
            lit->term_count++;
        }
    }
    return JSON_PARSER_OK;
}

JSON_PARSER_ERR_t json_parser_rule_head(cJSON* head, dl_parser_rule_t* rule)
{
    rule->head = (dl_parser_literal_t*)calloc(1, sizeof(dl_parser_literal_t));
    if(rule->head == NULL) return JSON_PARSER_MEM;
  
    cJSON* head_type = NULL;
    head_type = cJSON_GetObjectItemCaseSensitive(head, "@type");
    if(cJSON_IsString(head_type) && (head_type->valuestring != NULL)){
        CMP_NOT_VALUESTRING(head_type, Literal) return JSON_PARSER_INVAL; 
    }

    cJSON* head_predicate = NULL;
    head_predicate = cJSON_GetObjectItemCaseSensitive(head, "predicate");
        
    if(cJSON_IsString(head_predicate) && (head_predicate->valuestring != NULL)){
        rule->head->predicate = 
            (char*)malloc(sizeof(char) * (strlen(head_predicate->valuestring) + 1));
        if(rule->head->predicate == NULL) return JSON_PARSER_MEM;
        strcpy(rule->head->predicate, head_predicate->valuestring);

#ifdef PARSER_DEBUG_VERBOSE
        printf("Rule head predicate: %s\n", head_predicate->valuestring);
#endif
    }

    cJSON* head_terms = NULL;
    head_terms = cJSON_GetObjectItemCaseSensitive(head, "terms");
    if(head_terms != NULL) json_parser_terms(head_terms, rule->head);

    return JSON_PARSER_OK;
}

JSON_PARSER_ERR_t json_parser_rule_body(cJSON* body, dl_parser_rule_t* rule)
{
    rule->body = 
        (dl_parser_clause_body_t*)calloc(1, sizeof(dl_parser_clause_body_t));
    if(rule->body == NULL) return JSON_PARSER_MEM;

    cJSON* body_literal = NULL;
    cJSON_ArrayForEach(body_literal, body){
        
        cJSON* body_type = NULL;
        body_type = cJSON_GetObjectItemCaseSensitive(body_literal, "@type");
        if(cJSON_IsString(body_type) && (body_type->valuestring != NULL)){
            CMP_NOT_VALUESTRING(body_type, Literal) return JSON_PARSER_INVAL;
            else{
                rule->body->literals = 
                    (dl_parser_literal_t**) realloc(rule->body->literals,
                            (rule->body->literal_count + 1) * sizeof(dl_parser_literal_t*));
                rule->body->literals[rule->body->literal_count] = 
                    (dl_parser_literal_t*) calloc(1, sizeof(dl_parser_literal_t));
            }
        }  
        
        cJSON* body_predicate = NULL;
        body_predicate = cJSON_GetObjectItemCaseSensitive(body_literal, "predicate");
        
        if(cJSON_IsString(body_predicate) && (body_predicate->valuestring != NULL)){
            rule->body->literals[rule->body->literal_count]->predicate = 
                (char*)malloc(sizeof(char) * (strlen(body_predicate->valuestring) +1));
            if(rule->body->literals[rule->body->literal_count] == NULL)
                return JSON_PARSER_MEM;
            strcpy(rule->body->literals[rule->body->literal_count]->predicate, 
                body_predicate->valuestring);
          
#ifdef PARSER_DEBUG_VERBOSE
            printf("body term predicate: %s\n", body_predicate->valuestring);
#endif
        }

        cJSON* body_terms = NULL;
        body_terms = cJSON_GetObjectItemCaseSensitive(body_literal, "terms");
        if(body_terms != NULL) 
            json_parser_terms(body_terms, rule->body->literals[rule->body->literal_count]);
        
        rule->body->literal_count++;
    }
    
    return JSON_PARSER_OK;
}

JSON_PARSER_ERR_t json_parser_wrap_literal(datalog_literal_t* dl_lit, 
    dl_parser_literal_t* parser_lit)
{
    dl_parser_term_t* tmp_term = NULL;
    if(parser_lit->term_count > 0) tmp_term = parser_lit->term_head;
    else return JSON_PARSER_INVAL;
    for(int i = 0; i < parser_lit->term_count; i++){
        dl_lit->add_term(dl_lit, tmp_term->value, tmp_term->type);
        tmp_term = tmp_term->next;
    }
    return JSON_PARSER_OK;
}

JSON_PARSER_ERR_t json_parser_assert_rule(dl_parser_rule_t* rule)
{
    //create clause head API literal
    datalog_literal_t* head_lit = datalog_literal_init(rule->head->predicate);
    //add terms
    json_parser_wrap_literal(head_lit, rule->head);
    
    //create API clause
    datalog_clause_t* tmp_clause = datalog_clause_init(head_lit);
    
    //add body literals
    datalog_literal_t* tmp_lit = NULL; 
    for(int i = 0; i < rule->body->literal_count; i++){
        tmp_lit = datalog_literal_init(rule->body->literals[i]->predicate);
        json_parser_wrap_literal(tmp_lit, rule->body->literals[i]);
        tmp_clause->add_literal(tmp_clause, tmp_lit);
    }
    
    //assert clause
    tmp_clause->assert(tmp_clause);
    //TODO CHECK FREE
    tmp_clause->free(&tmp_clause);

    return JSON_PARSER_OK;
}

JSON_PARSER_ERR_t json_parser_rule(cJSON* rule)
{
    cJSON* rule_type = NULL;
    cJSON* rule_head = NULL;
    cJSON* rule_body = NULL;
    cJSON* tmp_type = NULL;
    tmp_type = cJSON_GetObjectItemCaseSensitive(rule, "@type");
    if(cJSON_IsString(tmp_type) && (tmp_type->valuestring != NULL)){
        CMP_VALUESTRING(tmp_type, Rule){
            
            dl_parser_rule_t* tmp_dl_rule = 
                (dl_parser_rule_t*)calloc(1, sizeof(dl_parser_rule_t)); 
            if(tmp_dl_rule == NULL) return JSON_PARSER_MEM;

            cJSON* tmp_head = NULL;
            cJSON* tmp_body = NULL;

            tmp_head = cJSON_GetObjectItemCaseSensitive(rule, "head");
            if(tmp_head != NULL){
                json_parser_rule_head(tmp_head, tmp_dl_rule);
            }

            tmp_body = cJSON_GetObjectItemCaseSensitive(rule, "body");
            if(tmp_body != NULL){
                json_parser_rule_body(tmp_body, tmp_dl_rule);
            }

            json_parser_assert_rule(tmp_dl_rule);
            
        }
    }else{
        printf("rules entry not of rule type\n");
    }
    
    return JSON_PARSER_OK;
}

JSON_PARSER_ERR_t json_parser_fact_head(cJSON* head, dl_parser_fact_t* fact)
{
    fact->literal = (dl_parser_literal_t*)calloc(1, sizeof(dl_parser_literal_t));
    if(fact->literal == NULL) return JSON_PARSER_MEM;

    cJSON* head_predicate = NULL;
    head_predicate = cJSON_GetObjectItemCaseSensitive(head, "predicate");

    if(cJSON_IsString(head_predicate) && (head_predicate->valuestring != NULL)){
        fact->literal->predicate = 
            (char*)malloc(sizeof(char) * (strlen(head_predicate->valuestring) + 1));
        if(fact->literal->predicate == NULL) return JSON_PARSER_MEM;
        strcpy(fact->literal->predicate, head_predicate->valuestring);

#ifdef PARSER_DEBUG_VERBOSE
        printf("Fact head predicate: %s\n", head_predicate->valuestring);
#endif
    }

    cJSON* head_terms = NULL;
    head_terms = cJSON_GetObjectItemCaseSensitive(head, "terms");
    if(head_terms != NULL) json_parser_terms(head_terms, fact->literal);

    return JSON_PARSER_OK;
}

JSON_PARSER_ERR_t json_parser_assert_fact(dl_parser_fact_t* fact)
{
    datalog_literal_t* lit = datalog_literal_init(fact->literal->predicate);
    json_parser_wrap_literal(lit, fact->literal);

    lit->assert(lit);
    //TODO CHECK FREE
    lit->free(&lit);

    return JSON_PARSER_OK;
} 

JSON_PARSER_ERR_t json_parser_fact(cJSON* fact)
{
    cJSON* fact_type = NULL;
    cJSON* fact_head = NULL;
    cJSON* rule_body = NULL;
    cJSON* tmp_type = NULL;

    tmp_type = cJSON_GetObjectItemCaseSensitive(fact, "@type");
    if(cJSON_IsString(tmp_type) && (tmp_type->valuestring != NULL)){
        CMP_VALUESTRING(tmp_type, Fact){
            
            dl_parser_fact_t* tmp_dl_fact =
                (dl_parser_fact_t*)calloc(1, sizeof(dl_parser_fact_t));
            if(tmp_dl_fact == NULL) return JSON_PARSER_MEM;

            cJSON* tmp_head = NULL;

            tmp_head = cJSON_GetObjectItemCaseSensitive(fact, "head");
            if(tmp_head != NULL){
                json_parser_fact_head(tmp_head, tmp_dl_fact);
            }

            json_parser_assert_fact(tmp_dl_fact);
        }
    }

    return JSON_PARSER_OK;
}

JSON_PARSER_ERR_t json_parser_runtime(char* filename)
{
    FILE* file_to_parse = fopen(filename, "r");
    if(!file_to_parse){
        printf("File failed to open\n");
        exit(EXIT_FAILURE);
    }

#ifdef PARSER_DEBUG_VERBOSE
    printf("File loaded\n");
#endif

    fseek(file_to_parse, 0, SEEK_END);
    long input_file_size = ftell(file_to_parse);
    rewind(file_to_parse);
    char* file_to_parse_contents = 
        malloc(input_file_size * sizeof(char));
    if(file_to_parse == NULL) return -1;
    fread(file_to_parse_contents, sizeof(char), 
            input_file_size, file_to_parse);
    fclose(file_to_parse);

    int status = 0;
    cJSON* test_json = cJSON_Parse(file_to_parse_contents);
    if(test_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL){
#ifdef PARSER_ERR
            fprintf(stderr, "Error before: %s\n", error_ptr);
#endif
        }
        status = 0;
        exit(EXIT_FAILURE);
    }

    const cJSON* type = NULL;
    type = cJSON_GetObjectItemCaseSensitive(test_json, "@type");
    if(cJSON_IsString(type) && (type->valuestring != NULL)){
#ifdef PARSER_DEBUG_VERBOSE
        printf("Checking monitor \"%s\"\n", type->valuestring);
#endif
    }

    //RULES
    cJSON* rules = cJSON_GetObjectItemCaseSensitive(test_json, "rules");
    cJSON* tmp_rules = NULL;
    cJSON_ArrayForEach(tmp_rules, rules){
        json_parser_rule(tmp_rules);
    }
    
    //FACTS
    cJSON* facts = cJSON_GetObjectItemCaseSensitive(test_json, "facts");
    cJSON* tmp_facts = NULL;
    cJSON_ArrayForEach(tmp_facts, facts){
        json_parser_fact(tmp_facts);
    }

    return JSON_PARSER_OK;
}
