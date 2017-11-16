/**
 * @file        datalog_parser.c
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       A Datalog parser to extract Datalog rules and facts from
 *              XML documents.
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

#include "libxml/parser.h"

#include "config.h"

#include "datalog_parser.h"

void dl_parser_print_root(dl_parser_doc_t* doc)
{
    printf("*==========================================* \n");
    printf("  Root node for XML file \"%s\"\n", doc->filename);
    printf(" \n");
    printf(" Name: %s\n", doc->root_node->name);
    printf(" Type: %i\n", doc->root_node->type);
    printf("*==========================================* \n");
}

void dl_parser_print_metadata(dl_parser_doc_t* doc)
{
    printf("*==========================================* \n");
    printf("  Metadata for XML file \"%s\"\n", doc->filename);
    printf(" \n");
    printf(" Description: %s\n", doc->metadata->description);
    printf(" Author: %s\n", doc->metadata->author);
    printf(" Device Name: %s\n", doc->metadata->device_name);
    printf(" Device Type: %s\n", doc->metadata->device_type);
    printf(" Manufacturer: %s\n", doc->metadata->device_manufacturer);
    printf(" Contact: %s\n", doc->metadata->device_contact);
    printf(" Model: %s\n", doc->metadata->device_model);
    printf(" Serial No#: %s\n", doc->metadata->device_serial);
    printf("*==========================================* \n");
}

void dl_parser_print_literal(dl_parser_literal_t* literal)
{
    printf("%s(%s,%s)", literal->predicate, literal->arg1,
            literal->arg2);
}

void dl_parser_print_clause_body(dl_parser_clause_body_t* clause_body)
{
    printf("Body: ");
    for(int i = 0; i < clause_body->literal_count; i++){
        dl_parser_print_literal(clause_body->literals[i]);
        if(i != clause_body->literal_count - 1 )
            printf(", ");
    }
}

void dl_parser_print_rule(dl_parser_rule_t* rule)
{
    printf("Rule \n");
    printf("Head: ");
    dl_parser_print_literal(rule->head);
    printf("\n");
    dl_parser_print_clause_body(rule->body);
    printf("\n");
}

void dl_parser_print_rule_list(dl_parser_doc_t* doc)
{
    if(doc->rules_head == NULL){
#ifdef PARSER_ERR
        fprintf(stderr, "[DATALOG][PARSER] Err: cannot print rule list, list doesn't"
                "exsist\n");
#endif
        return;
    }
    
    dl_parser_rule_t* rule_head = doc->rules_head;

    while(rule_head != NULL){

        dl_parser_print_rule(rule_head);

        rule_head = rule_head->next;
    }
}

void dl_parser_print_fact(dl_parser_fact_t* fact)
{
    printf("Fact: ");
    dl_parser_print_literal(fact->literal);
    printf("\n");
}

void dl_parser_print_fact_list(dl_parser_doc_t* doc)
{
    if(doc->facts_head == NULL){
#ifdef PARSER_ERR
        fprintf(stderr, "[DATALOG][PARSER] Err: cannot print fact list, list doesn't"
                "exsist\n");
#endif
        return;
    }
    
    dl_parser_fact_t* fact_head = doc->facts_head;

    while(fact_head != NULL){

        dl_parser_print_fact(fact_head);

        fact_head = fact_head->next;
    }
}

dl_parser_rule_t* dl_parser_get_last_rule(dl_parser_doc_t* doc)
{
    if(doc->rules_head == NULL) return NULL;

    dl_parser_rule_t* head = doc->rules_head;

    while(head->next != NULL)
        head = head->next;
    
    return head;
}

DL_PARSER_ERR_t dl_parser_add_rule(dl_parser_doc_t* doc, dl_parser_rule_t* rule)
{
    if(doc->rules_head == NULL)
        doc->rules_head = rule;
    else{
        dl_parser_rule_t* tmp = dl_parser_get_last_rule(doc);
        tmp->next = rule;
    }
    
    return DL_PARSER_OK;
}

dl_parser_fact_t* dl_parser_get_last_fact(dl_parser_doc_t* doc)
{
    if(doc->facts_head == NULL) return NULL;

    dl_parser_fact_t* head = doc->facts_head;

    while(head->next != NULL)
        head = head->next;
    
    return head;
}

DL_PARSER_ERR_t dl_parser_add_fact(dl_parser_doc_t* doc, dl_parser_fact_t* fact)
{
    if(doc->facts_head == NULL)
        doc->facts_head = fact;
    else{
        dl_parser_fact_t* tmp = dl_parser_get_last_fact(doc);
        tmp->next = fact;
    }

    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_terms(dl_parser_doc_t* doc, xmlNode* terms_node, 
        dl_parser_literal_t* literal)
{
    xmlChar* contents;
    xmlNode* tmp; 
    DL_PARSER_ARG_TYPE_t arg1_type = 0; 
    
    tmp = terms_node->xmlChildrenNode;

    while(tmp != NULL){

        if(!xmlStrcmp(tmp->name, (const xmlChar*) "constant")){
            contents = xmlNodeListGetString(doc->document, tmp->xmlChildrenNode, 1);
            if(!arg1_type){
                literal->arg1 = (char*)malloc(sizeof(char) * strlen((const char*)contents));
                strcpy(literal->arg1, (const char*)contents);
                arg1_type = DL_PARSER_CONST;
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: constant found for arg1 in process"
                    " literal, value is \"%s\"\n", literal->arg1);
#endif
            }else if(arg1_type == DL_PARSER_CONST){
                literal->arg2 = (char*)malloc(sizeof(char) * strlen((const char*)contents));
                strcpy(literal->arg2, (const char*)contents);
                literal->lit_type = DL_CC;
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: constant found for arg2 in process"
                    " literal, value is \"%s\"\n", literal->arg2);
#endif
            }else if(arg1_type == DL_PARSER_VAR){
                literal->arg2 = (char*)malloc(sizeof(char) * strlen((const char*)contents));
                strcpy(literal->arg2, (const char*)contents);
                literal->lit_type = DL_VC;
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: constant found for arg2 in process"
                    " literal, value is \"%s\"\n", literal->arg2);
#endif
            }
        }else if(!xmlStrcmp(tmp->name, (const xmlChar*) "variable")){
            contents = xmlNodeListGetString(doc->document, tmp->xmlChildrenNode, 1);
            if(!arg1_type){
                literal->arg1 = (char*)malloc(sizeof(char) * strlen((const char*)contents));
                strcpy(literal->arg1, (const char*)contents);
                arg1_type = DL_PARSER_VAR;
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: variable found for arg1 in process"
                    " literal, value is \"%s\"\n", literal->arg1);
#endif
            }else if(arg1_type == DL_PARSER_CONST){
                literal->arg2 = (char*)malloc(sizeof(char) * strlen((const char*)contents));
                strcpy(literal->arg2, (const char*)contents);
                literal->lit_type = DL_CV;
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: variable found for arg2 in process"
                    " literal, value is \"%s\"\n", literal->arg2);
#endif
            }else if(arg1_type == DL_PARSER_VAR){
                literal->arg2 = (char*)malloc(sizeof(char) * strlen((const char*)contents));
                strcpy(literal->arg2, (const char*)contents);
                literal->lit_type = DL_VV;
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: variable found for arg2 in process"
                    " literal, value is \"%s\"\n", literal->arg2);
#endif
            }
        }
        tmp = tmp->next;
    }
    return DL_PARSER_OK;
}

dl_parser_literal_t* dl_parser_process_literal(dl_parser_doc_t* doc, xmlNode* literal_node)
{
    dl_parser_literal_t* ret = (dl_parser_literal_t*)malloc(sizeof(dl_parser_literal_t));

    if(ret == NULL){
#ifdef PARSER_ERR 
        fprintf(stderr, "[DATALOG][PARSER] Err: process literal alloc of literal failed\n");
#endif
        return NULL;
    }

    xmlNode *tmp;
    xmlChar* contents;

    tmp = literal_node->xmlChildrenNode;

    while(tmp != NULL){
        
        if(!xmlStrcmp(tmp->name, (const xmlChar*) "predicate")){
            contents = xmlNodeListGetString(doc->document, tmp->xmlChildrenNode, 1);
            ret->predicate = (char*)malloc(sizeof(char) * strlen((const char*)contents));
            strcpy(ret->predicate, (const char*)contents);
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: predicate found in process literal,"
                    "value is \"%s\"\n", ret->predicate);
#endif
        }else if(!xmlStrcmp(tmp->name, (const xmlChar*) "terms")){
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: terms found in process literal\n");
#endif
            dl_parser_terms(doc, tmp, ret);
        }
        tmp = tmp->next;
    }
    return ret;
}

xmlNode* dl_parser_get_head_literal_node(xmlNode* head_node)
{
    xmlNode* node = head_node->xmlChildrenNode;
    
    while(node != NULL){
        if(!xmlStrcmp(node->name, (const xmlChar*)"literal")){
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: literal node found for fact\n");
#endif
            break;
        }
    node = node->next;
    }
    return node;
}

dl_parser_clause_body_t* dl_parser_body_get_literal_nodes(dl_parser_doc_t* doc,
        xmlNode* body_node)
{
    dl_parser_clause_body_t* ret = 
        (dl_parser_clause_body_t*)calloc(1, sizeof(dl_parser_clause_body_t));

    if(ret == NULL){
#ifdef PARSER_ERR 
        fprintf(stderr, "[DATALOG][PARSER] Err: clause body alloc failed\n");
#endif
        return NULL;
    }
   
    xmlNode* node = body_node->xmlChildrenNode;
    dl_parser_literal_t* temp_literal;
        
    while(node != NULL){
        if(!xmlStrcmp(node->name, (const xmlChar*)"literal")){
            temp_literal = (dl_parser_literal_t*)calloc(1, sizeof(dl_parser_literal_t));
            if(temp_literal == NULL){
#ifdef PARSER_ERR
                fprintf(stderr, "[DATALOG][PARSER] Err: temp literal alloc failed\n");
#endif
                return NULL;
            }
            //TODO checks
            temp_literal = dl_parser_process_literal(doc, node);

#ifdef PARSER_DEBUG 
            fprintf(stderr, "[DATALOG][PARSER] Debug: literal found in body \"%s(%s, %s)\"\n"
                    , temp_literal->predicate, temp_literal->arg1, temp_literal->arg2);
#endif
            
            ret->literals = (dl_parser_literal_t**)realloc(ret->literals,
                    sizeof(dl_parser_literal_t*) * (ret->literal_count + 1));

            if(ret->literals[ret->literal_count] == NULL){
#ifdef PARSER_ERR
                fprintf(stderr, "[DATALOG][PARSER] Err: realloc of literal list failed\n");
#endif
                return NULL;
            }

            ret->literals[ret->literal_count] = temp_literal;

            ret->literal_count++;
        }
        node = node->next;
    }

    return ret;
}

DL_PARSER_ERR_t dl_parser_process_rule(dl_parser_doc_t* doc, dl_parser_rule_t* rule)
{
    xmlNode* node = rule->node->xmlChildrenNode;
    xmlChar* contents;

    while(node != NULL){
        if(!xmlStrcmp(node->name, (const xmlChar*)"head")){
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: head node found for rule\n");
#endif
            //function for processing head
            xmlNode* head_node = dl_parser_get_head_literal_node(node);
            rule->head = dl_parser_process_literal(doc, head_node);
        }else if(!xmlStrcmp(node->name, (const xmlChar*)"body")){
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: body node found for rule\n");
#endif
            //function for processing body
            rule->body = dl_parser_body_get_literal_nodes(doc, node);
        }
        node = node->next;
    }

    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_process_fact(dl_parser_doc_t* doc, dl_parser_fact_t* fact)
{
    xmlNode* node = fact->node->xmlChildrenNode;
    xmlChar* contents;

    while(node != NULL){
        if(!xmlStrcmp(node->name, (const xmlChar*)"head")){
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: head node found for fact\n");
#endif
            break;
        }
        node = node->next;
    }

    node = dl_parser_get_head_literal_node(node);

    fact->literal = dl_parser_process_literal(doc, node);

    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_mappings(dl_parser_doc_t* doc)
{
    doc->mappings_node = doc->root_node->xmlChildrenNode;
    while(doc->mappings_node != NULL){
        if(!xmlStrcmp(doc->mappings_node->name, (const xmlChar*) "mappings")){
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: mappings node found\n");
#endif
            break;
        }
    doc->mappings_node = doc->mappings_node->next;
    }

    //iterate through the next tier tags and push pointers to all
    //facts and rules into lists
    xmlNode* node = doc->mappings_node->xmlChildrenNode;
    
    while(node != NULL){
        if(!xmlStrcmp(node->name, (const xmlChar*) "fact")){
            //create fact
            dl_parser_fact_t* tmp = (dl_parser_fact_t*)calloc(1, sizeof(dl_parser_fact_t));
            tmp->node = node;
            dl_parser_add_fact(doc, tmp);
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: fact found whilst parsing mappings\n");
#endif
        }else if(!xmlStrcmp(node->name, (const xmlChar*) "rule")){
            dl_parser_rule_t* tmp = (dl_parser_rule_t*)calloc(1, sizeof(dl_parser_rule_t));
            tmp->node = node;
            dl_parser_add_rule(doc, tmp);
#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: rule found whilst parsing mappings\n");
#endif
        }
        node = node->next;
    }

    //process lists
    dl_parser_fact_t* fact_head = doc->facts_head;

    while(fact_head != NULL){
        dl_parser_process_fact(doc, fact_head);    
        fact_head = fact_head->next;
    }
    
    dl_parser_rule_t* rule_head = doc->rules_head;

    while(rule_head != NULL){
        dl_parser_process_rule(doc, rule_head);
        rule_head = rule_head->next;
    }

#ifdef PARSER_DEBUG_VERBOSE
            fprintf(stderr, "[DATALOG][PARSER] Verbose: facts processed in parse mappings\n");
#endif
    
/*    //print
    dl_parser_fact_t* fact_tmp = doc->facts_head;
    while(fact_tmp != NULL){
        dl_parser_print_fact(fact_tmp);
        fact_tmp = fact_tmp->next;
    }
*/
    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_script(dl_parser_doc_t* doc)
{
    for(doc->script_node = doc->root_node->xmlChildrenNode;
            doc->script_node; doc->script_node = doc->script_node->next){
        if(!xmlStrcmp(doc->script_node->name, (const xmlChar*) "script"))
            break;
    }

    //TODO maybe
        
    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_alloc_xml_string(char** pointer, xmlChar* string)
{
    *pointer = (char*)malloc(sizeof(char) * strlen((const char*) string));

    if(*pointer == NULL) return DL_PARSER_MEM;

    strcpy(*pointer, (const char*)string);

    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_metadata_device(dl_parser_doc_t* doc,
        xmlNode* dev_node)
{
    xmlNode* tmp_node = dev_node->xmlChildrenNode;
    xmlChar* contents;
    DL_PARSER_ERR_t ret;

    while(tmp_node != NULL){
    
        if(!xmlStrcmp(tmp_node->name, (const xmlChar*)"name")){
            contents = xmlNodeListGetString(doc->document, tmp_node->xmlChildrenNode, 1);
            if(dl_parser_alloc_xml_string(&doc->metadata->device_name, contents) != DL_PARSER_OK)
                return DL_PARSER_MEM;
            xmlFree(contents);
#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)tmp_node->name, doc->metadata->device_name);
#endif
        }else if(!xmlStrcmp(tmp_node->name, (const xmlChar*)"type")){
            contents = xmlNodeListGetString(doc->document, tmp_node->xmlChildrenNode, 1);
            if(dl_parser_alloc_xml_string(&doc->metadata->device_type, contents) != DL_PARSER_OK)
                return DL_PARSER_MEM;
            xmlFree(contents);
#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)tmp_node->name, doc->metadata->device_type);
#endif
        }else if(!xmlStrcmp(tmp_node->name, (const xmlChar*)"manufacturer")){
            contents = xmlNodeListGetString(doc->document, tmp_node->xmlChildrenNode, 1);
            if(dl_parser_alloc_xml_string(&doc->metadata->device_manufacturer, contents) != DL_PARSER_OK)
                return DL_PARSER_MEM;
            xmlFree(contents);
#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)tmp_node->name, doc->metadata->device_manufacturer);
#endif
        }else if(!xmlStrcmp(tmp_node->name, (const xmlChar*)"contact")){
            contents = xmlNodeListGetString(doc->document, tmp_node->xmlChildrenNode, 1);
            if(dl_parser_alloc_xml_string(&doc->metadata->device_contact, contents) != DL_PARSER_OK)
                return DL_PARSER_MEM;
            xmlFree(contents);
#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)tmp_node->name, doc->metadata->device_contact);
#endif
        }else if(!xmlStrcmp(tmp_node->name, (const xmlChar*)"model")){
            contents = xmlNodeListGetString(doc->document, tmp_node->xmlChildrenNode, 1);
            if(dl_parser_alloc_xml_string(&doc->metadata->device_model, contents) != DL_PARSER_OK)
                return DL_PARSER_MEM;
            xmlFree(contents);
#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)tmp_node->name, doc->metadata->device_model);
#endif
        }else if(!xmlStrcmp(tmp_node->name, (const xmlChar*)"serial")){
            contents = xmlNodeListGetString(doc->document, tmp_node->xmlChildrenNode, 1);
            if(dl_parser_alloc_xml_string(&doc->metadata->device_serial, contents) != DL_PARSER_OK)
                return DL_PARSER_MEM;
            xmlFree(contents);
#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)tmp_node->name, doc->metadata->device_serial);
#endif
        }else if(!xmlStrcmp(tmp_node->name, (const xmlChar*)"year")){
            contents = xmlNodeListGetString(doc->document, tmp_node->xmlChildrenNode, 1);
            if(dl_parser_alloc_xml_string(&doc->metadata->device_year, contents) != DL_PARSER_OK)
                return DL_PARSER_MEM;
            xmlFree(contents);
#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)tmp_node->name, doc->metadata->device_year);
#endif
        }
        
        tmp_node = tmp_node->next;
    }

    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_metadata(dl_parser_doc_t* doc)
{
    xmlNode* node;
    
    for(doc->metadata_root = doc->root_node->xmlChildrenNode; 
            doc->metadata_root; doc->metadata_root = doc->metadata_root->next){
        if(!xmlStrcmp(doc->metadata_root->name, (const xmlChar*) "metadata"))
            break;   
    }
   
    if(doc->metadata_root == NULL){
#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node not found\n");
#endif
        return DL_PARSER_NO_NODE; 
    }

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata root node found with tag \"%s\"\n",
            (const char*)doc->metadata_root->name);
#endif
    
    xmlChar* contents;

    for(node = doc->metadata_root->xmlChildrenNode; node; node = node->next){
        if(!xmlStrcmp(node->name, (const xmlChar*) "description")){

            contents = xmlNodeListGetString(doc->document, node->xmlChildrenNode, 1);
            doc->metadata->description = 
                (char*)malloc(sizeof(char) * strlen((const char*) contents));
     
            if(doc->metadata->description == NULL) return DL_PARSER_MEM;
            strcpy(doc->metadata->description, (const char*)contents);
            xmlFree(contents);

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)node->name, doc->metadata->description);
#endif
        }else if(!xmlStrcmp(node->name, (const xmlChar*) "author")){
            
            contents = xmlNodeListGetString(doc->document, node->xmlChildrenNode, 1);
            doc->metadata->author = 
                (char*)malloc(sizeof(char) * strlen((const char*)contents));
            if(doc->metadata->author == NULL) return DL_PARSER_MEM;
            strcpy(doc->metadata->author, (const char*)contents);
            xmlFree(contents);

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata node found with tag \"%s\" "
            "and content \"%s\"\n", (const char*)node->name, doc->metadata->author);
#endif
        }else if(!xmlStrcmp(node->name, (const xmlChar*) "device")){
            dl_parser_metadata_device(doc, node);

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata device node found\n");
#endif
        }
    }

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Metadata found \n");
#endif
    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_deinit_lit(dl_parser_literal_t* lit_head)
{
    dl_parser_literal_t* lit_tmp = lit_head;
    dl_parser_literal_t* prev_lit;

    while(lit_tmp != NULL){
        prev_lit = lit_tmp->next;
        free(lit_tmp);
        lit_tmp = prev_lit;
    }
    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_deinit_rule(dl_parser_rule_t* rules_head)
{
    dl_parser_rule_t* rule_tmp = rules_head;
    dl_parser_rule_t* prev_rule;

    while(rule_tmp != NULL){
        prev_rule = rule_tmp->next;
        free(rule_tmp);
        rule_tmp = prev_rule;
    }
    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_deinit_facts(dl_parser_fact_t* facts_head)
{
    dl_parser_fact_t* fact_tmp = facts_head;
    dl_parser_fact_t* prev_fact;

    while(fact_tmp != NULL){
        prev_fact = fact_tmp->next;
        free(fact_tmp);
        fact_tmp = prev_fact;
    }
    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_deinit_wo_data(dl_parser_doc_t* doc)
{
    free(doc);
    return DL_PARSER_OK;
}

DL_PARSER_ERR_t dl_parser_deinit_w_data(dl_parser_doc_t* doc)
{
    //free lists.
    dl_parser_deinit_lit(doc->literals_head);
    dl_parser_deinit_rule(doc->rules_head);
    dl_parser_deinit_facts(doc->facts_head);

    //free metadata
    free(doc->metadata);

    //free filename
    free(doc->filename);

    //free struct
    free(doc);
        
    return DL_PARSER_OK;
}

dl_parser_doc_t* dl_parser_init(char* filename)
{
    dl_parser_doc_t* doc = (dl_parser_doc_t*)calloc(1, sizeof(dl_parser_doc_t));

    if(doc == NULL){
#ifdef PARSER_ERR
        fprintf(stderr, "[DATALOG][PARSER] ERR: Couldn't alloc dl_parser_doc");
#endif
        return NULL;
}

    doc->metadata = (dl_parser_metadata_t*)calloc(1, sizeof(dl_parser_metadata_t));

    if(doc->metadata == NULL){
#ifdef PARSER_ERR
        fprintf(stderr, "[DATALOG][PARSER] ERR: Couldn't alloc metadata");
#endif
        return NULL;
    }

    if(filename == NULL){
#ifdef PARSER_ERR
    fprintf(stderr, "[DATALOG][PARSER] ERR: XML filename invalid \"%s\" \n", filename);
#endif
        return NULL;
    }

    doc->filename = (char*)malloc(sizeof(char)*strlen(filename));
    if(doc->filename == NULL){
#ifdef PARSER_ERR
        fprintf(stderr, "[DATALOG][PARSER] ERR: Couldn't alloc doc filename");
#endif
        return NULL;
    }
    strcpy(doc->filename, filename);

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Debug: XML filname loaded as \"%s\"\n", filename);
#endif

    doc->document = xmlReadFile(filename, NULL, 0);

#ifdef PARSER_DEBUG_VERBOSE
    fprintf(stderr, "[DATALOG][PARSER] Verbose: XML document read\n");
#endif 

    doc->root_node = xmlDocGetRootElement(doc->document);

    dl_parser_print_root(doc);

#ifdef PARSER_DEBUG
    if(doc->root_node == NULL)
        fprintf(stderr, "[DATALOG][PARSER] Debug: Root node is NULL");
#endif
    if(doc->root_node == NULL){
        xmlFreeDoc(doc->document);
        return NULL;
    }    
    
    if(xmlStrcmp(doc->root_node->name, (const xmlChar*) "datalog")){
#ifdef PARSER_ERR
        fprintf(stderr, "XML file is of the wrong type \n");
        fprintf(stderr, "type is of \"%s\" when type datalog is needed\n",
                doc->root_node->name);
#endif
        xmlFreeDoc(doc->document);
        free(doc);
        return NULL;
    }
    return doc;
}

DL_PARSER_ERR_t dl_parser_copy_string(char* pointer, char* string)
{
    pointer = (char*)malloc(sizeof(char)*strlen(string));
    if(pointer == NULL) return DL_PARSER_MEM;
    strcpy(pointer, string);
    return DL_PARSER_OK;
}

dl_parser_return_doc_t* dl_parser_prepare_return_doc(dl_parser_doc_t* doc)
{
    dl_parser_return_doc_t* ret = 
        (dl_parser_return_doc_t*)calloc(1, sizeof(dl_parser_return_doc_t));

    if(ret == NULL) return NULL;

    memcpy(ret, doc, sizeof(dl_parser_return_doc_t)); 

    return ret;
}

dl_parser_return_doc_t* dl_parser_runtime(char* filename)
{
    dl_parser_doc_t* dl_doc = dl_parser_init(filename);
    DL_PARSER_ERR_t ret = DL_PARSER_OK;

#ifdef PARSER_DEBUG
    fprintf(stderr, "[DATALOG][PARSER] Init finished:              %s\n",
            (ret == DL_PARSER_OK)?("SUCCESS"):("FAIL"));
#endif

    //metadata
    ret = dl_parser_metadata(dl_doc); 

    dl_parser_print_metadata(dl_doc);

    //mappings
    dl_parser_mappings(dl_doc);

    dl_parser_print_fact_list(dl_doc);

    dl_parser_print_rule_list(dl_doc);

    //return results
    dl_parser_return_doc_t* ret_doc = dl_parser_prepare_return_doc(dl_doc);

    dl_parser_deinit_wo_data(dl_doc);

//TODO script parsing
    
    return ret_doc;
}