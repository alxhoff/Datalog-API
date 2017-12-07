/**
 * @file        datalog_cli.c
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "datalog_cli.h"
#include "datalog_cli_asserter.h"

char* dl_cli_remove_spaces(char* input)                                         
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<(int)strlen(input); i++,j++){
        if (input[i]!=' ')                           
            output[j]=input[i];                     
        else
            j--;                                     
    }
    output[j]=0;
    return output;
}

DATALOG_CLI_ERR_t dl_cli_split_line(char* line, char** head, char** body)
{
    *head = strtok(line, ":-");

    if(*head == NULL){
#ifdef CLI_DEBUG
        fprintf(stderr,"[DATALOG][CLI] Err: input string split failed, head is NULL\n");
#endif
        return DATALOG_CLI_SPLIT;
    }

    *body = strtok(NULL, ":-");

    if(*body == NULL){
#ifdef CLI_DEBUG
        fprintf(stderr,"[DATALOG][CLI] Err: input string split failed, body is NULL\n");
#endif
    }

    return DATALOG_CLI_OK;
}

datalog_cli_unprocessed_body_t* dl_cli_get_body_literals(
        char* body)
{
    datalog_cli_unprocessed_body_t* ret = 
        (datalog_cli_unprocessed_body_t*)malloc(sizeof(datalog_cli_unprocessed_body_t));
   
    if(ret == NULL){
#ifdef CLI_ERR
        fprintf(stderr,"[DATALOG][CLI] Err: unprocessed body alloc failed\n");
#endif
        return NULL;
    }

    char* tmp;
    int body_lit_count = 0;

    ret->body_literals = (char**)malloc(sizeof(char*));
   
    if(ret->body_literals == NULL){
#ifdef CLI_ERR
        fprintf(stderr,"[DATALOG][CLI] Err: unprocessed body list alloc failed\n");
#endif
        return NULL;
    }

    tmp = strtok(body, ".");

    while(tmp != NULL){
        printf("%s\n",tmp);
        
        ret->body_literals[body_lit_count] = tmp;
        body_lit_count++;
       
        
        ret->body_literals = (char**)realloc(ret->body_literals,
                sizeof(char*) * (body_lit_count + 1));
       
        if(ret->body_literals == NULL){
#ifdef CLI_ERR
            fprintf(stderr,"[DATALOG][CLI] Err: unprocessed body list realloc failed, literal #%d\n",
                body_lit_count);
#endif
            return NULL;
        }

        tmp = strtok(NULL, ".");
    }
    ret->lit_count = body_lit_count;

    return ret;
}

DATALOG_TERM_TYPE_t dl_cli_process_string_type(char* term)
{
    if(term[0] >= 'A' && term[0] <= 'Z')
        return DL_CLI_VARIABLE;
    else if(term[0] >= 'a' && term[0] <= 'z')
        return DL_CLI_CONSTANT;
    else return (DATALOG_TERM_TYPE_t) -1;
}

datalog_cli_literal_t* dl_cli_process_literal(char* lit_string)
{
    datalog_cli_literal_t* ret = (datalog_cli_literal_t*)
        calloc(1, sizeof(datalog_cli_literal_t));

    if(ret == NULL){
#ifdef CLI_ERR
        fprintf(stderr,"[DATALOG][CLI] Err: unprocessed body list alloc failed\n");
#endif
        return NULL;
    }
    
    char* tmp;
    tmp = strtok(lit_string, "(,).");
    
    if(tmp == NULL){
#ifdef CLI_DEBUG
        fprintf(stderr,"[DATALOG][CLI] Err: process literal failed, predicate is NULL\n");
#endif
    }
    ret->predicate = (char*)malloc(sizeof(char) * strlen(tmp));

    if(ret->predicate == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: process literal predicate alloc failed\n");
#endif
        return NULL;
    }

    strcpy(ret->predicate, tmp);

    tmp = strtok(NULL, "(,).");
    datalog_cli_term_t *tmp_term, *prev_term;
    while(tmp != NULL){
        if(tmp != NULL){
#ifdef CLI_DEBUG
            fprintf(stderr,"[DATALOG][CLI] DEBUG: process literal term found: %s \n", tmp);
#endif
        }
        tmp_term = (datalog_cli_term_t*)calloc(1,sizeof(datalog_cli_term_t));
        if(tmp_term == NULL) return NULL;
        
        if(ret->term_head == NULL) ret->term_head = tmp_term;
        else prev_term->next = tmp_term;      
        
        
        tmp_term->value = (char*)malloc(sizeof(char) * (strlen(tmp) + 1));
        if(tmp_term->value == NULL) return NULL;
        strcpy(tmp_term->value, tmp);
        
        tmp_term->type = dl_cli_process_string_type(tmp);
        
        prev_term = tmp_term;
        tmp = strtok(NULL, "(,).");
    }
    return ret;
}

DATALOG_CLI_ERR_t dl_cli_process_body_list(
        datalog_cli_unprocessed_body_t* body_list, datalog_cli_command_t* command)
{
    command->body = (datalog_cli_literal_t**)
        calloc(command->body_count, sizeof(datalog_cli_literal_t*));

    if(command->body == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: process body list list calloc failed\n");
#endif
        return DATALOG_CLI_MEM;
    }

    for(int i = 0; i < body_list->lit_count; i++){
        command->body[i] = dl_cli_process_literal(body_list->body_literals[i]); 
        if(command->body[i] == NULL){
#ifdef CLI_DEBUG
            fprintf(stderr, "[DATALOG][CLI] Debug: process body list, process literal #%d failed\n",
                    i);
#endif
            return DATALOG_CLI_LIT; 
        }
    }
    
    return DATALOG_CLI_OK;
}

datalog_cli_command_t* dl_cli_process_head(char* head_string)
{
    datalog_cli_command_t* ret = 
        (datalog_cli_command_t*)malloc(sizeof(datalog_cli_command_t));
   
    if(ret == NULL){
#ifdef CLI_ERR
        fprintf(stderr, "[DATALOG][CLI] Err: process head command alloc failed\n");
#endif
        return NULL;
    }

    //get end characters
    int end_char_pos = strlen(head_string) - 1;
#ifdef CLI_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG][CLI] last character of head detected as: %c\n"
            , head_string[end_char_pos]);
#endif
     
    if(head_string[end_char_pos] == '?'){
#if CLI_DEBUG_VERBOSE
        fprintf(stderr, "[DATALOG][CLI] Verbose: last character of head "
            "signifies query\n");
#endif
        ret->head_type = DL_CLI_HEAD_QUERY;
    }else if(head_string[end_char_pos] == '.'){
#if CLI_DEBUG_VERBOSE
        fprintf(stderr, "[DATALOG][CLI] Verbose: last character of head "
            "signifies statement\n");
#endif
        ret->head_type = DL_CLI_HEAD_STATEMENT;
    }else if(head_string[end_char_pos] == '~'){
#if CLI_DEBUG_VERBOSE
        fprintf(stderr, "[DATALOG][CLI] Verbose: last character of head "
            "signifies retraction\n");
#endif
        ret->head_type = DL_CLI_HEAD_RETRACTION;
    }else{
        fprintf(stderr, "[DATALOG][CLI] Debug: Syntax error, a literal "
                "is missing a terminating symbol\n");
        return NULL;
    }
    ret->head = dl_cli_process_literal(head_string);

    return ret;
}

DATALOG_CLI_ERR_t dl_cli_parse_line(char* line)
{
    if(!strcmp(line, "exit")) return DATALOG_CLI_EXIT;
    if(!strcmp(line, "help")) return DATALOG_CLI_HELP;

    char* no_spaces = dl_cli_remove_spaces(line);

#ifdef CLI_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG][CLI] Verbose: line wo spaces \"%s\"\n", no_spaces);
#endif

    char* head = NULL;
    char* body = NULL;
    
    if(dl_cli_split_line(no_spaces, &head, &body) != DATALOG_CLI_OK){
#ifdef CLI_DEBUG
        fprintf(stderr, "[DATALOG][CLI] Debug: split line failed\n");
#endif
        return DATALOG_CLI_SPLIT; 
    }

    datalog_cli_command_t* test_cmd = dl_cli_process_head(head);

    if(test_cmd == NULL){
#ifdef CLI_DEGBUG
        fprintf(stderr, "[DATALOG][CLI] Debug: process head failed\n");
#endif
        return DATALOG_CLI_INVAL; 
    }

#ifdef CLI_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG][CLI] Verbose: head is %s\n", head);
    fprintf(stderr, "[DATALOG][CLI] Verbose: body is %s\n", body);
#endif

    if(body == NULL){
        if(test_cmd->head_type == DL_CLI_HEAD_QUERY){
#ifdef CLI_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][CLI] Verbose: query created\n");
#endif
            test_cmd->cmd_type = DL_CLI_QUERY;
        }
        else if(test_cmd->head_type == DL_CLI_HEAD_STATEMENT){
#ifdef CLI_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][CLI] Verbose: fact created\n");
#endif
            test_cmd->cmd_type = DL_CLI_FACT;
        }
        else if(test_cmd->head_type == DL_CLI_HEAD_RETRACTION){
#ifdef CLI_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][CLI] Verbose: retraction created\n");
#endif
            test_cmd->cmd_type = DL_CLI_RETRACTION;
        }
    }else{
        if(test_cmd->head_type == DL_CLI_HEAD_QUERY){
            fprintf(stderr, "[DATALOG][CLI] Syntax error: head literal cannot be a query and"
                    " have a body\n");
            return DATALOG_CLI_SYNTAX;
        }
        else if(test_cmd->head_type == DL_CLI_HEAD_STATEMENT){
#ifdef CLI_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][CLI] Verbose: rule created\n");
#endif
            test_cmd->cmd_type = DL_CLI_RULE;
        }
        else if(test_cmd->head_type == DL_CLI_HEAD_RETRACTION){
#ifdef CLI_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][CLI] Verbose: retraction created\n");
#endif
            test_cmd->cmd_type = DL_CLI_RETRACTION;
        }
        
        datalog_cli_unprocessed_body_t* body_list
            = dl_cli_get_body_literals(body);

        if(body_list == NULL){
#ifdef CLI_DEBUG 
            fprintf(stderr, "[DATALOG][CLI] Debug: getting unprocessed body failed\n");
#endif
            return DATALOG_CLI_INVAL; 
        }

#ifdef CLI_DEBUG_VERBOSE
        if(body_list->body_literals[0] != NULL)
            fprintf(stderr, "[DATALOG][CLI] Verbose: first body literal is \"%s\"\n", 
                body_list->body_literals[0]);
#endif
    
        if(dl_cli_process_body_list(body_list, test_cmd) != DATALOG_CLI_OK)
        {
#ifdef CLI_DEBUG
            fprintf(stderr, "[DATALOG][CLI] Debug: process body list failed\n");
#endif
            return DATALOG_CLI_INVAL; 
        }
    }

    //ASSERT CMD STRUCT
    dl_cli_assert_command(test_cmd);

    return DATALOG_CLI_OK;
}

void dl_cli_print_help(void)
{
        printf("Usage:\n");
        printf("\n");
        printf("Clause structure: \n");
        printf("        [literal] :- [literal].[literal].[literal]...\n");
        printf("\n");
        printf("Literal structure: \n");
        printf("        [term]([term], [term]).\n");
        printf("\n");
        printf("Queries: literals followed by a ? are treated as queries\n");
        printf("        [term]([term], [term])?\n");
        printf("\n");
        printf("Term: a term can be either a variable or a constant\n");
        printf("Variable: a term starting with a capital letter\n");
        printf("Constant: a term starting with a lower case letter\n");
        printf("\n");
        printf("Options: \n");
        printf("    help,       print this message\n");
        printf("    clear,      clears the screen\n");
        printf("    exit,       exits the CLI interface\n");
        printf("\n");
}
