/**
 * @file        datalog_opcua_generator.c
 * @author      Alexander Hoffman
 * @email       alxhoff@gmail.com
 * @website     http://alexhoffman.info
 * @copyright   GNU GPL v3
 * @brief       OPCUA Server XML Generator
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
#include <string.h>

#include "datalog_opcua_generator.h"

opcua_document_t* opcua_document;

//DOCUMENT FUNCTIONS
void datalog_opcua_save_deinit_doc(void)
{
    xmlSaveFormatFileEnc( opcua_document->filename, opcua_document->document, 
            "UTF-8", 1);

    xmlFree(opcua_document->document);

    free(opcua_document);

    xmlCleanupParser();

    xmlMemoryDump();
}

opcua_document_t* datalog_opcua_create_document(char* filename, char* version)
{
    opcua_document_t* ret = (opcua_document_t*)malloc(sizeof(opcua_document_t));
    if(ret == NULL) return NULL;

    ret->filename = (char*)malloc(sizeof(char) * (strlen(filename) + 1));
    if(ret->filename == NULL) return NULL;
    strcpy(ret->filename, filename);

    ret->version = (char*)malloc(sizeof(char) * (strlen(version) + 1));
    if(ret->version == NULL) return NULL;
    strcpy(ret->version, version);

    ret->document = xmlNewDoc(BAD_CAST version);
    ret->root_node = xmlNewNode(NULL, BAD_CAST "UANodeSet");
    xmlDocSetRootElement(ret->document, ret->root_node);

    return ret;
}

DL_OPCUA_ERR_t datalog_opcua_init_doc(void)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    xmlNodePtr tmp_node = NULL, tmp_parent = NULL, tmp_child = NULL;
    char buff[256];
    int i = 0;

    opcua_document = datalog_opcua_create_document(XML_FILENAME, 
            XML_FILE_VERSION); 


    xmlCreateIntSubset(opcua_document->document, BAD_CAST "UANodeSet", 
            NULL, BAD_CAST "dtdIsHere");

    tmp_node = xmlNewChild(opcua_document->root_node, NULL, 
            BAD_CAST "NamespaceUris", NULL);
    xmlNewChild(tmp_node, NULL, BAD_CAST "Uri", BAD_CAST NAMESPACE_URI);

    //ALIAS
    tmp_parent = xmlNewChild(opcua_document->root_node, NULL, BAD_CAST "Aliases", NULL);

    while(alias_array[i].integer != 0){
        sprintf(buff, "i=%d", alias_array[i].integer);
        tmp_child = xmlNewChild(tmp_parent, NULL, BAD_CAST "Alias", BAD_CAST buff);
        xmlNewProp(tmp_child, BAD_CAST "Alias",  BAD_CAST alias_array[i].name);
        i++;
    }
    
    tmp_parent = xmlNewChild(opcua_document->root_node, NULL, 
            BAD_CAST "Extensions", NULL);
    tmp_parent = xmlNewChild(tmp_parent, NULL, BAD_CAST "Extension", NULL);
    tmp_child = xmlNewChild(tmp_parent, NULL, BAD_CAST "ModelInfo", NULL);
    xmlNewProp(tmp_child, BAD_CAST "Tool", BAD_CAST "UaModeler");
    xmlNewProp(tmp_child, BAD_CAST "Hash", BAD_CAST "T9MjgfInUChe45aJYm9rKw==");
    xmlNewProp(tmp_child, BAD_CAST "Version", BAD_CAST "1.4.0");

    return DL_OPCUA_OK;
}

//SET FUNCTIONS
//## OBJECT TYPE
DL_OPCUA_ERR_t self_set_object_type_parent_node_id_ns(opcua_object_type_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_parent_node_id_i(opcua_object_type_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_parent_node_id_s(opcua_object_type_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_node_id_ns(opcua_object_type_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->node_id, ns);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_node_id_i(opcua_object_type_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->node_id, i);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_node_id_s(opcua_object_type_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->node_id, s);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_browse_name(opcua_object_type_t* self, 
        char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self->attributes,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_display_name(opcua_object_type_t* self, 
        char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self->attributes,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_description(opcua_object_type_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self->attributes, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_is_abstract(opcua_object_type_t* self, 
        bool abstract)
{
    if(self->object_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->object_type_attributes->is_abstract = abstract;
    return DL_OPCUA_OK;
}

//## VARIABLE
DL_OPCUA_ERR_t self_set_variable_parent_node_id_ns(opcua_variable_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_parent_node_id_i(opcua_variable_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_parent_node_id_s(opcua_variable_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_node_id_ns(opcua_variable_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->node_id, ns);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_node_id_i(opcua_variable_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->node_id, i);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_node_id_s(opcua_variable_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->node_id, s);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_browse_name(opcua_variable_t* self, 
    char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self->attributes,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_display_name(opcua_variable_t* self, 
        char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self->attributes,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_description(opcua_variable_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self->attributes, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_user_access_level(opcua_variable_t* self, int ual)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_access_level(opcua_variable_t* self, int al)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->access_level = al;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_value(opcua_variable_t* self, void* value)
{
//TODO
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_data_type(opcua_variable_t* self, opcua_node_id_t* ID)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    if(ID == NULL) return DL_OPCUA_INVAL;
    memcpy(&self->variable_attributes->data_type, ID, sizeof(opcua_node_id_t));
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_array_dimensions(opcua_variable_t* self, int ad)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_attributes->array_dimensions = ad;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_value_rank(opcua_variable_t* self, int32_t vr)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_attributes->value_rank = vr;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_is_abstract(opcua_variable_t* self, bool abstract)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_attributes->is_abstract = abstract;
    return DL_OPCUA_OK;
}

//## METHOD
DL_OPCUA_ERR_t self_set_method_parent_node_id_ns(opcua_method_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_method_parent_node_id_i(opcua_method_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_method_parent_node_id_s(opcua_method_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_method_node_id_ns(opcua_method_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_method_node_id_i(opcua_method_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_method_node_id_s(opcua_method_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_method_declaration_id(opcua_method_t* self, int id)
{
    if(self->method_attributes == NULL) return DL_OPCUA_INVAL;
    self->method_attributes->method_declaration_id = id;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_method_browse_name(opcua_method_t* self, char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self->attributes,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_method_display_name(opcua_method_t* self, char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self->attributes,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_method_description(opcua_method_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self->attributes, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_method_user_access_level(opcua_method_t* self, int ual)
{
    if(self->method_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_method_access_level(opcua_method_t* self, int al)
{
    if(self->method_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->access_level = al;
    return DL_OPCUA_OK;
}

//## OBJECT
DL_OPCUA_ERR_t self_set_object_parent_node_id_ns(opcua_object_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_parent_node_id_i(opcua_object_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_parent_node_id_s(opcua_object_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_node_id_ns(opcua_object_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->node_id, ns);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_node_id_i(opcua_object_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->node_id, i);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_node_id_s(opcua_object_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->node_id, s);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_browse_name(opcua_object_t* self, char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self->attributes,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_display_name(opcua_object_t* self, char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self->attributes,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_description(opcua_object_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self->attributes, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_user_access_level(opcua_object_t* self, int ual)
{
    if(self->object_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_object_access_level(opcua_object_t* self, int al)
{
    if(self->object_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->access_level = al;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_object_event_notifier(opcua_object_t* self, 
        unsigned char event_notifier)
{
    if(self->object_attributes == NULL) return DL_OPCUA_INVAL;
    self->object_attributes->event_notifier = event_notifier;
    return DL_OPCUA_OK;
}

//## VARIABLE TYPE
DL_OPCUA_ERR_t self_set_variable_type_parent_node_id_ns(opcua_variable_type_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_parent_node_id_i(opcua_variable_type_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_parent_node_id_s(opcua_variable_type_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_node_id_ns(opcua_variable_type_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->node_id, ns);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_node_id_i(opcua_variable_type_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->node_id, i);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_node_id_s(opcua_variable_type_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->node_id, s);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_browse_name(opcua_variable_type_t* self, char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self->attributes,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_display_name(opcua_variable_type_t* self, char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self->attributes,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_description(opcua_variable_type_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self->attributes, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_type_user_access_level(opcua_variable_type_t* self, int ual)
{
    if(self->variable_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_type_access_level(opcua_variable_type_t* self, int al)
{
    if(self->variable_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->access_level = al;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_type_value(opcua_variable_type_t* self, void* value)
{
//TODO
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_type_data_type(opcua_variable_type_t* self, opcua_node_id_t* ID)
{
    if(self->variable_type_attributes == NULL) return DL_OPCUA_INVAL;
    if(ID == NULL) return DL_OPCUA_INVAL;
    memcpy(&self->variable_type_attributes->data_type, ID, sizeof(opcua_node_id_t));
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_type_array_dimensions(opcua_variable_type_t* self, uint32_t ad)
{
    if(self->variable_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_type_attributes->array_dimensions = ad;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_type_value_rank(opcua_variable_type_t* self, int32_t vr)
{
    if(self->variable_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_type_attributes->value_rank = vr;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_type_is_abstract(opcua_variable_type_t* self, bool abstract)
{
    if(self->variable_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_type_attributes->is_abstract = abstract;
    return DL_OPCUA_OK;
}

//## REFERENCE TYPE
DL_OPCUA_ERR_t self_set_reference_type_parent_node_id_ns(opcua_reference_type_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_parent_node_id_i(opcua_reference_type_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_parent_node_id_s(opcua_reference_type_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_node_id_ns(opcua_reference_type_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_node_id_i(opcua_reference_type_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_node_id_s(opcua_reference_type_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_browse_name(opcua_reference_type_t* self, 
        char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self->attributes,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_display_name(opcua_reference_type_t* self, 
        char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self->attributes,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_description(opcua_reference_type_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self->attributes, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_type_user_access_level(opcua_reference_type_t* self, int ual)
{
    if(self->reference_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_reference_type_access_level(opcua_reference_type_t* self, int al)
{
    if(self->reference_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->access_level = al;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_reference_type_is_abstract(opcua_reference_type_t* self, bool abstract)
{
    if(self->reference_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->reference_type_attributes->is_abstract = abstract;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_reference_type_symmetric(opcua_reference_type_t* self, bool symmetric)
{
    if(self->reference_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->reference_type_attributes->symmetric = symmetric;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_reference_type_inverse_name(opcua_reference_type_t* self, 
        char* inverse_name)
{
    if(self->reference_type_attributes == NULL) return DL_OPCUA_INVAL;
    if(inverse_name == NULL) return DL_OPCUA_INVAL;
    self->reference_type_attributes->inverse_name = 
        (char*)realloc(self->reference_type_attributes->inverse_name, 
                sizeof(char) * (strlen(inverse_name) + 1));
    if(self->reference_type_attributes == NULL) return DL_OPCUA_MEM;
    strcpy(self->reference_type_attributes->inverse_name, inverse_name);
    return DL_OPCUA_OK;
}

//## DATA TYPE
DL_OPCUA_ERR_t self_set_data_type_parent_node_id_ns(opcua_data_type_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_parent_node_id_i(opcua_data_type_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_parent_node_id_s(opcua_data_type_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_node_id_ns(opcua_data_type_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_node_id_i(opcua_data_type_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_node_id_s(opcua_data_type_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_browse_name(opcua_data_type_t* self, 
        char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self->attributes,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_display_name(opcua_data_type_t* self, 
        char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self->attributes,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_description(opcua_data_type_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self->attributes, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_data_type_user_access_level(opcua_data_type_t* self, int ual)
{
    if(self->data_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_data_type_access_level(opcua_data_type_t* self, int al)
{
    if(self->data_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->access_level = al;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_data_type_is_abstract(opcua_data_type_t* self, bool abstract)
{
    if(self->data_type_attributes == NULL) return DL_OPCUA_INVAL;
    self->data_type_attributes->is_abstract = abstract;
    return DL_OPCUA_OK;
}

//## VIEW
DL_OPCUA_ERR_t self_set_view_parent_node_id_ns(opcua_view_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_view_parent_node_id_i(opcua_view_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_view_parent_node_id_s(opcua_view_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_view_node_id_ns(opcua_view_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->attributes->node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_view_node_id_i(opcua_view_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->attributes->node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_view_node_id_s(opcua_view_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->attributes->node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_view_browse_name(opcua_view_t* self, char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self->attributes,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_view_display_name(opcua_view_t* self, char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self->attributes,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_view_description(opcua_view_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self->attributes, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_view_user_access_level(opcua_view_t* self, int ual)
{
    if(self->view_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_view_access_level(opcua_view_t* self, int al)
{
    if(self->view_attributes == NULL) return DL_OPCUA_INVAL;
    self->attributes->access_level = al;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_view_contains_no_loops(opcua_view_t* self, bool val)
{
    if(self->view_attributes == NULL) return DL_OPCUA_INVAL;
    self->view_attributes->contains_no_loops = val;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_view_event_notifier(opcua_view_t* self, 
        unsigned char mask)
{
    if(self->view_attributes == NULL) return DL_OPCUA_INVAL;
    self->view_attributes->event_notifier = mask;
    return DL_OPCUA_OK;
}

//## REFERENCE
DL_OPCUA_ERR_t self_set_reference_type(opcua_reference_t* self, char* type)
{
    self->type = (char*)realloc(self->type, 
        sizeof(char) * (strlen(type) + 1));
    if(self->type == NULL) return DL_OPCUA_MEM;

    strcpy(self->type, type);

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_reference_id_ns(opcua_reference_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_id_i(opcua_reference_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_id_s(opcua_reference_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_reference_is_forward(opcua_reference_t* self, bool val)
{
    self->is_forward = val;

    return DL_OPCUA_OK;
}

//##ATTRIBUTE
DL_OPCUA_ERR_t self_set_attribute_parent_node_id_ns(
        opcua_node_attributes_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->parent_node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_parent_node_id_i(opcua_node_attributes_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->parent_node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_parent_node_id_s(opcua_node_attributes_t* self, 
        char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->parent_node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_node_id_ns(opcua_node_attributes_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_node_id_i(opcua_node_attributes_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_node_id_s(opcua_node_attributes_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_browse_name(opcua_node_attributes_t* self, 
        char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self, browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_display_name(opcua_node_attributes_t* self, 
        char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self, display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_description(opcua_node_attributes_t* self,
        char* description)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_description(self, description);
    return ret;
}

DL_OPCUA_ERR_t self_set_attribute_user_access_level(opcua_node_attributes_t* self, 
        int ual)
{
    self->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_attribute_access_level(opcua_node_attributes_t* self, int al)
{
    self->access_level = al;
    return DL_OPCUA_OK;
}

//CREATE REFERENCES
DL_OPCUA_ERR_t self_create_node_object_type_references(
        opcua_object_type_t* object)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(object, 
            DL_OPC_OBJ_TYPE);

    return ret;
}

DL_OPCUA_ERR_t self_create_node_method_references(
        opcua_method_t* method)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(method, 
            DL_OPC_METHOD);

    return ret;
}

DL_OPCUA_ERR_t self_create_node_variable_references(
        opcua_variable_t* variable)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(variable, 
            DL_OPC_VARIABLE);

    return ret;
}

DL_OPCUA_ERR_t self_create_node_object_references(
        opcua_object_t* object)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(object, 
            DL_OPC_OBJ);

    return ret;
}

DL_OPCUA_ERR_t self_create_node_variable_type_references(
        opcua_variable_type_t* variable_type)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(variable_type, 
            DL_OPC_VAR_TYPE);

    return ret;
}

DL_OPCUA_ERR_t self_create_node_reference_type_references(
        opcua_reference_type_t* reference_type)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(reference_type, 
            DL_OPC_REF_TYPE);

    return ret;
}

DL_OPCUA_ERR_t self_create_node_data_type_references(
        opcua_data_type_t* data_type)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(data_type, 
            DL_OPC_DATA_TYPE);

    return ret;
}

DL_OPCUA_ERR_t self_create_node_view_references(
        opcua_view_t* view)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(view, 
            DL_OPC_VIEW);

    return ret;
}

//SELF ADD REF 
DL_OPCUA_ERR_t self_add_ref_object_type(opcua_object_type_t* object_t, 
        opcua_reference_t* reference)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    ret = datalog_opcua_add_reference(reference, object_t, DL_OPC_OBJ_TYPE);
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_add_ref_method(opcua_method_t* method,
        opcua_reference_t* reference)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    ret = datalog_opcua_add_reference(reference, method, DL_OPC_METHOD);
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_add_ref_variable(opcua_variable_t* variable,
        opcua_reference_t* reference)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    ret = datalog_opcua_add_reference(reference, variable, DL_OPC_VARIABLE);
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_add_ref_object(opcua_object_t* object,
        opcua_reference_t* reference)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    ret = datalog_opcua_add_reference(reference, object, DL_OPC_OBJ);
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_add_ref_variable_type(opcua_variable_type_t* variable_type,
        opcua_reference_t* reference)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    ret = datalog_opcua_add_reference(reference, variable_type, DL_OPC_VAR_TYPE);
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_add_ref_reference_type(opcua_reference_type_t* reference_type,
        opcua_reference_t* reference)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    ret = datalog_opcua_add_reference(reference, reference_type, DL_OPC_REF_TYPE);
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_add_ref_data_type(opcua_data_type_t* data_type,
        opcua_reference_t* reference)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    ret = datalog_opcua_add_reference(reference, data_type, DL_OPC_DATA_TYPE);
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_add_ref_view(opcua_view_t* view,
        opcua_reference_t* reference)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    ret = datalog_opcua_add_reference(reference, view, DL_OPC_VIEW);
    return DL_OPCUA_OK;
}
//## GENERALISED FUNCTIONS
DL_OPCUA_ERR_t datalog_opcua_set_browse_name(opcua_node_attributes_t* attribute, 
        char* browse_name)
{
    attribute->browse_name = 
        (char*)realloc(attribute->browse_name, 
                sizeof(char) * (strlen(browse_name) + 1));
    if(attribute->browse_name == NULL) 
        return DL_OPCUA_MEM;
    strcpy(attribute->browse_name, browse_name);

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_set_display_name(opcua_node_attributes_t* attribute, 
        char* display_name)
{
    attribute->display_name = 
        (char*)realloc(attribute->display_name, 
                sizeof(char) * (strlen(display_name) + 1));
    if(attribute->display_name == NULL) 
        return DL_OPCUA_MEM;
    strcpy(attribute->display_name, display_name);
    
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_set_id_s(opcua_node_id_t* id, char* value)
{
    if(value != NULL){
        id->s = (char*)realloc(id->s, 
                sizeof(char) * (strlen(value) + 1));
        if(id->s == NULL) return DL_OPCUA_MEM;
        strcpy(id->s, value);
        return DL_OPCUA_OK;
    }
    return DL_OPCUA_INVAL;
}

DL_OPCUA_ERR_t datalog_opcua_set_id_ns(opcua_node_id_t* id, int ns)
{
    id->ns = ns;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_set_id_i(opcua_node_id_t* id, int i)
{
    id->i = i;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_set_id_o(opcua_node_id_t* id, char* o)
{
    if(o != NULL){
        id->o = (char*)realloc(id->o, 
                sizeof(char) * (strlen(o) + 1));
        if(id->o == NULL) return DL_OPCUA_MEM;
        strcpy(id->o, o);
        return DL_OPCUA_OK;
    }
    return DL_OPCUA_INVAL;
}

DL_OPCUA_ERR_t datalog_opcua_set_id_g(opcua_node_id_t* id, char* g)
{
    if(g != NULL){
        id->g = (char*)realloc(id->g, 
                sizeof(char) * (strlen(g) + 1));
        if(id->g == NULL) return DL_OPCUA_MEM;
        strcpy(id->g, g);
        return DL_OPCUA_OK;
    }
    return DL_OPCUA_INVAL;
}

DL_OPCUA_ERR_t datalog_opcua_set_description(opcua_node_attributes_t* attribute,
        char* description)
{
    attribute->description = (char*)realloc(attribute->description,
            sizeof(char) * (strlen(description) + 1));
    if(attribute->description == NULL) return DL_OPCUA_MEM;
    strcpy(attribute->description, description);
    return DL_OPCUA_OK;
}

//CREATE OBJECT NODES
//ATTRIBUTES
DL_OPCUA_ERR_t datalog_opcua_create_node_attributes(xmlNodePtr node, 
        xmlNodePtr display_name_node, opcua_node_attributes_t* attributes)
{
    char buffer[32];
    
    DL_OPCUA_ERR_t ret = datalog_opcua_add_id_attribute(node, "ParentNodeId",
        &attributes->parent_node_id);            
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    ret = datalog_opcua_add_id_attribute(node, "NodeId",
        &attributes->node_id);            
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;
    
    if(attributes->browse_name != NULL)
        xmlNewProp(node, BAD_CAST "BrowseName",
                BAD_CAST attributes->browse_name);
    
    if(display_name_node == NULL){
        if(attributes->display_name != NULL)
            display_name_node = xmlNewChild(node, NULL,
                BAD_CAST "DisplayName", BAD_CAST attributes->display_name);
    }else
        xmlNodeSetContent(display_name_node, 
                BAD_CAST attributes->display_name);
    
    if(attributes->user_access_level != 0){
        sprintf(buffer, "%d", attributes->user_access_level);
        xmlNewProp(node, BAD_CAST "UserAccessLevel", BAD_CAST buffer);
    }
    
    if(attributes->access_level != 0){
        sprintf(buffer, "%d", attributes->access_level);
        xmlNewProp(node, BAD_CAST "AccessLevel", BAD_CAST buffer);
    }

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_object_type_attributes(
        opcua_object_type_t* object_t)
{
    char buffer[32];

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;

    if(object_t->object_type_attributes->is_abstract != -1){
        sprintf(buffer, "%s", 
            (object_t->object_type_attributes->is_abstract == true ? "true" : "false"));
        xmlNewProp(object_t->node, BAD_CAST "IsAbstract", BAD_CAST buffer);
    }
    
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_method_attributes(opcua_method_t* method)
{
    char buffer[32];

    if(method->method_attributes->method_declaration_id != 0){
        sprintf(buffer, "i=%d", method->method_attributes->method_declaration_id);
        xmlNewProp(method->node, BAD_CAST "MethodDeclarationId", BAD_CAST buffer);
    }

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_variable_attributes(opcua_variable_t* variable)
{
    char buffer[32];

    //TODO value

    DL_OPCUA_ERR_t ret = datalog_opcua_add_id_attribute(variable->node, "DataType",
            &variable->variable_attributes->data_type);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    if(variable->variable_attributes->array_dimensions != 0){
        sprintf(buffer, "%d", variable->variable_attributes->array_dimensions);
        xmlNewProp(variable->node, BAD_CAST "ArrayDimensions", BAD_CAST buffer);
    }
    if(variable->variable_attributes->value_rank != 0){
        sprintf(buffer, "%d", variable->variable_attributes->value_rank);
        xmlNewProp(variable->node, BAD_CAST "ValueRank", BAD_CAST buffer);
    }
    
    if(variable->variable_attributes->is_abstract != -1){
        sprintf(buffer, "%s", 
            (variable->variable_attributes->is_abstract == true ? "true" : "false"));
        xmlNewProp(variable->node, BAD_CAST "IsAbstract", BAD_CAST buffer);
    }

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_object_attributes(opcua_object_t* object)
{
    char buffer[32]; 
    
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;

    if(object->object_attributes->event_notifier != 0){
        sprintf(buffer, "0x%02X", object->object_attributes->event_notifier);
        xmlNewProp(object->node, BAD_CAST "EventNotifier", BAD_CAST buffer);
    }
        
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_variable_type_attributes(
        opcua_variable_type_t* variable_type)
{
    char buffer[32];

    //TODO value
    
    DL_OPCUA_ERR_t ret = datalog_opcua_add_id_attribute(variable_type->node, "DataType",
            &variable_type->variable_type_attributes->data_type);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    if(variable_type->variable_type_attributes->array_dimensions != 0){
        sprintf(buffer, "%d", variable_type->variable_type_attributes->array_dimensions);
        xmlNewProp(variable_type->node, BAD_CAST "ArrayDimensions", BAD_CAST buffer);
    }
    if(variable_type->variable_type_attributes->value_rank != 0){
        sprintf(buffer, "%d", variable_type->variable_type_attributes->value_rank);
        xmlNewProp(variable_type->node, BAD_CAST "ValueRank", BAD_CAST buffer);
    }
    
    if(variable_type->variable_type_attributes->is_abstract != -1){
        sprintf(buffer, "%s", 
            (variable_type->variable_type_attributes->is_abstract == true ? "true" : "false"));
        xmlNewProp(variable_type->node, BAD_CAST "IsAbstract", BAD_CAST buffer);
    }

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_reference_type_attributes(
        opcua_reference_type_t* reference_type)
{
    char buffer[32];

    if(reference_type->reference_type_attributes->is_abstract != -1){
        sprintf(buffer, "%s", 
            (reference_type->reference_type_attributes->is_abstract == true ? "true" : "false"));
        xmlNewProp(reference_type->node, BAD_CAST "IsAbstract", BAD_CAST buffer);
    }
    
    if(reference_type->reference_type_attributes->symmetric != -1){
        sprintf(buffer, "%s", 
            (reference_type->reference_type_attributes->symmetric == true ? "true" : "false"));
        xmlNewProp(reference_type->node, BAD_CAST "Symmetric", BAD_CAST buffer);
    }
 
    if(reference_type->reference_type_attributes->inverse_name != NULL)
        xmlSetProp(reference_type->node, BAD_CAST "InverseName", 
                BAD_CAST reference_type->reference_type_attributes->inverse_name);

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_data_type_attributes(
        opcua_data_type_t* data_type)
{
    char buffer[32];

    if(data_type->data_type_attributes->is_abstract != -1){
        sprintf(buffer, "%s", 
            (data_type->data_type_attributes->is_abstract == true ? "true" : "false"));
        xmlNewProp(data_type->node, BAD_CAST "IsAbstract", BAD_CAST buffer);
    }
    
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_view_attributes(
        opcua_view_t* view)
{
    char buffer[32];

    if(view->view_attributes->contains_no_loops != -1){
        sprintf(buffer, "%s", 
            (view->view_attributes->contains_no_loops == true ? "true" : "false"));
        xmlNewProp(view->node, BAD_CAST "IsAbstract", BAD_CAST buffer);
    }
    
    if(view->view_attributes->event_notifier != 0){
        sprintf(buffer, "0x%02X", view->view_attributes->event_notifier);
        xmlNewProp(view->node, BAD_CAST "EventNotifier", BAD_CAST buffer);
    }
        
    return DL_OPCUA_OK;
}

//OBJECTS
DL_OPCUA_ERR_t datalog_opcua_create_node_object_type_from_array(
        opcua_reference_t* array, opcua_node_attributes_t* attributes)
{
    int i = 0;
    opcua_reference_t* tmp_ref;
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    
    opcua_object_type_t* tmp_obj_type = datalog_opcua_create_object_type();
    if(attributes->browse_name != NULL)
        tmp_obj_type->set_browse_name(tmp_obj_type, attributes->browse_name);
    if(attributes->display_name != NULL)
        tmp_obj_type->set_display_name(tmp_obj_type, attributes->display_name);
    if(tmp_obj_type->attributes != NULL){
        memcpy(&tmp_obj_type->attributes->node_id,
                &attributes->node_id, sizeof(opcua_node_id_t));
        memcpy(&tmp_obj_type->attributes->parent_node_id, 
                &attributes->parent_node_id, sizeof(opcua_node_id_t));
    }
    datalog_opcua_create_node_object_type(tmp_obj_type);

    while(array[i].type != NULL){
        tmp_ref = datalog_opcua_create_reference();
        tmp_ref->set_id_i(tmp_ref, array[i].id.i);
        tmp_ref->set_id_ns(tmp_ref, array[i].id.ns);
        tmp_ref->set_is_forward(tmp_ref, array[i].is_forward);
        if(array[i].type != NULL) 
            tmp_ref->set_type(tmp_ref, array[i].type);
        if(array[i].id.s != NULL)
            tmp_ref->set_id_s(tmp_ref, array[i].id.s);

        datalog_opcua_add_reference(tmp_ref, tmp_obj_type, DL_OPC_OBJ_TYPE);
        i++;
   }
    ret = datalog_opcua_create_node_references(tmp_obj_type, DL_OPC_OBJ_TYPE);

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_object_type(opcua_object_type_t* object)
{
    if(object == NULL) return DL_OPCUA_INVAL;
    if(object->attributes == NULL) return DL_OPCUA_INVAL;
    if(object->object_type_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    
    object->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UAObjectType", NULL);

    ret = datalog_opcua_create_node_attributes(object->node, object->display_name_node,
            object->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    ret = datalog_opcua_create_node_object_type_attributes(object);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_method(opcua_method_t* method)
{
    if(method == NULL) return DL_OPCUA_INVAL;
    if(method->attributes == NULL) return DL_OPCUA_INVAL;
    if(method->method_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;

    method->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UAMethod", NULL);

    ret = datalog_opcua_create_node_attributes(method->node, method->display_name_node,
            method->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    ret = datalog_opcua_create_node_method_attributes(method);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_variable(opcua_variable_t* variable)
{
    if(variable == NULL) return DL_OPCUA_INVAL;
    if(variable->attributes == NULL) return DL_OPCUA_INVAL;
    if(variable->variable_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
   
    variable->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UAVariable", NULL);

    ret = datalog_opcua_create_node_attributes(variable->node, variable->display_name_node,
            variable->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;
    
    ret = datalog_opcua_create_node_variable_attributes(variable);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_object(opcua_object_t* object)
{
    if(object == NULL) return DL_OPCUA_INVAL;
    if(object->attributes == NULL) return DL_OPCUA_INVAL;
    if(object->object_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    
    object->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UAObject", NULL);

    ret = datalog_opcua_create_node_attributes(object->node, object->display_name_node, 
            object->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;
    
    ret = datalog_opcua_create_node_object_attributes(object);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_variable_type(opcua_variable_type_t* variable_type)
{
    if(variable_type == NULL) return DL_OPCUA_INVAL;
    if(variable_type->attributes == NULL) return DL_OPCUA_INVAL;
    if(variable_type->variable_type_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    
    variable_type->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UAVariableType", NULL);

    ret = datalog_opcua_create_node_attributes(variable_type->node, variable_type->display_name_node, 
            variable_type->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;
    
    ret = datalog_opcua_create_node_variable_type_attributes(variable_type);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_reference_type(opcua_reference_type_t* reference_type)
{
    if(reference_type == NULL) return DL_OPCUA_INVAL;
    if(reference_type->attributes == NULL) return DL_OPCUA_INVAL;
    if(reference_type->reference_type_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    
    reference_type->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UAReferenceType", NULL);

    ret = datalog_opcua_create_node_attributes(reference_type->node, reference_type->display_name_node, 
            reference_type->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;
    
    ret = datalog_opcua_create_node_reference_type_attributes(reference_type);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_data_type(opcua_data_type_t* data_type)
{
    if(data_type == NULL) return DL_OPCUA_INVAL;
    if(data_type->attributes == NULL) return DL_OPCUA_INVAL;
    if(data_type->data_type_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    
    data_type->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UADataType", NULL);

    ret = datalog_opcua_create_node_attributes(data_type->node, data_type->display_name_node, 
            data_type->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;
    
    ret = datalog_opcua_create_node_data_type_attributes(data_type);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_view(opcua_view_t* view)
{
    if(view == NULL) return DL_OPCUA_INVAL;
    if(view->attributes == NULL) return DL_OPCUA_INVAL;
    if(view->view_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    
    view->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UAView", NULL);

    ret = datalog_opcua_create_node_attributes(view->node, view->display_name_node,
            view->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;
    
    ret = datalog_opcua_create_node_view_attributes(view);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

//REFERENCE
opcua_reference_t* datalog_opcua_find_reference_i(void* object,
        DL_OPCUA_TYPE_t type, int i)
{
    opcua_reference_t *ref_head;
    
    switch(type){
    case DL_OPC_OBJ_TYPE:
        ref_head = ((opcua_object_type_t*)object)->reference_head;
        break;
    case DL_OPC_VARIABLE:
        ref_head = ((opcua_variable_t*)object)->reference_head;
        break;
    case DL_OPC_METHOD:
        ref_head = ((opcua_method_t*)object)->reference_head;
        break;
    case DL_OPC_OBJ:
        ref_head = ((opcua_object_t*)object)->reference_head;
        break;
    case DL_OPC_VAR_TYPE:
        ref_head = ((opcua_variable_type_t*)object)->reference_head;
        break;
    case DL_OPC_REF_TYPE:
        ref_head = ((opcua_reference_type_t*)object)->reference_head;
        break;
    case DL_OPC_DATA_TYPE:
        ref_head = ((opcua_data_type_t*)object)->reference_head;
        break;
    case DL_OPC_VIEW:
        ref_head = ((opcua_view_t*)object)->reference_head;
        break;
    default:
        break;
    }
    
    while(ref_head->id.i != i){
        if(ref_head == NULL) return NULL;
        ref_head = ref_head->next;
    }

    return ref_head;
}

opcua_reference_t* datalog_opcua_find_reference_s(void* object,
        DL_OPCUA_TYPE_t type, char* s)
{
    opcua_reference_t *ref_head;
    
    switch(type){
    case DL_OPC_OBJ_TYPE:
        ref_head = ((opcua_object_type_t*)object)->reference_head;
        break;
    case DL_OPC_VARIABLE:
        ref_head = ((opcua_variable_t*)object)->reference_head;
        break;
    case DL_OPC_METHOD:
        ref_head = ((opcua_method_t*)object)->reference_head;
        break;
    case DL_OPC_OBJ:
        ref_head = ((opcua_object_t*)object)->reference_head;
        break;
    case DL_OPC_VAR_TYPE:
        ref_head = ((opcua_variable_type_t*)object)->reference_head;
        break;
    case DL_OPC_REF_TYPE:
        ref_head = ((opcua_reference_type_t*)object)->reference_head;
        break;
    case DL_OPC_DATA_TYPE:
        ref_head = ((opcua_data_type_t*)object)->reference_head;
        break;
    case DL_OPC_VIEW:
        ref_head = ((opcua_view_t*)object)->reference_head;
        break;
    default:
        break;
    }
   
    while(ref_head != NULL){
        if(!strcmp(((ref_head->id.s == NULL) ? "" : ref_head->id.s), s))
            return ref_head;

        if(ref_head == NULL) return NULL;
        ref_head = ref_head->next;
    }
    
    return NULL;
}

opcua_reference_t* datalog_opcua_find_reference_type(void* object,
        DL_OPCUA_TYPE_t type, char* type_string)
{
    opcua_reference_t *ref_head;
    
    switch(type){
    case DL_OPC_OBJ_TYPE:
        ref_head = ((opcua_object_type_t*)object)->reference_head;
        break;
    case DL_OPC_VARIABLE:
        ref_head = ((opcua_variable_t*)object)->reference_head;
        break;
    case DL_OPC_METHOD:
        ref_head = ((opcua_method_t*)object)->reference_head;
        break;
    case DL_OPC_OBJ:
        ref_head = ((opcua_object_t*)object)->reference_head;
        break;
    case DL_OPC_VAR_TYPE:
        ref_head = ((opcua_variable_type_t*)object)->reference_head;
        break;
    case DL_OPC_REF_TYPE:
        ref_head = ((opcua_reference_type_t*)object)->reference_head;
        break;
    case DL_OPC_DATA_TYPE:
        ref_head = ((opcua_data_type_t*)object)->reference_head;
        break;
    case DL_OPC_VIEW:
        ref_head = ((opcua_view_t*)object)->reference_head;
        break;
    default:
        break;
    }
   
    while(ref_head != NULL){
        if(!strcmp(((ref_head->type == NULL) ? "" : ref_head->type), type_string))
            return ref_head;

        if(ref_head == NULL) return NULL;
        ref_head = ref_head->next;
    }
    
    return NULL;
}

opcua_reference_t* datalog_opcua_find_reference(void* object,
        DL_OPCUA_TYPE_t type, opcua_node_id_t* ID)
{
    opcua_reference_t *ref_head;

    switch(type){
    case DL_OPC_OBJ_TYPE:
        ref_head = ((opcua_object_type_t*)object)->reference_head;
        break;
    case DL_OPC_VARIABLE:
        ref_head = ((opcua_variable_t*)object)->reference_head;
        break;
    case DL_OPC_METHOD:
        ref_head = ((opcua_method_t*)object)->reference_head;
        break;
    case DL_OPC_OBJ:
        ref_head = ((opcua_object_t*)object)->reference_head;
        break;
    case DL_OPC_VAR_TYPE:
        ref_head = ((opcua_variable_type_t*)object)->reference_head;
        break;
    case DL_OPC_REF_TYPE:
        ref_head = ((opcua_reference_type_t*)object)->reference_head;
        break;
    case DL_OPC_DATA_TYPE:
        ref_head = ((opcua_data_type_t*)object)->reference_head;
        break;
    case DL_OPC_VIEW:
        ref_head = ((opcua_view_t*)object)->reference_head;
        break;
    default:
        break;
    }
    
    while(memcmp(&ref_head->id, ID, sizeof(opcua_node_id_t))){
        if(ref_head == NULL) return NULL;
        ref_head = ref_head->next;
    }

    return ref_head;
}

DL_OPCUA_ERR_t datalog_opcua_free_reference_list(void** object, 
        DL_OPCUA_TYPE_t type)
{
    opcua_reference_t **ref_head, *next;

    switch(type){
    case DL_OPC_OBJ_TYPE:
        ref_head = &(*(opcua_object_type_t**)object)->reference_head;
        break;
    case DL_OPC_VARIABLE:
        ref_head = &(*(opcua_variable_t**)object)->reference_head;
        break;
    case DL_OPC_METHOD:
        ref_head = &(*(opcua_method_t**)object)->reference_head;
        break;
    case DL_OPC_OBJ:
        ref_head = &(*(opcua_object_t**)object)->reference_head;
        break;
    case DL_OPC_VAR_TYPE:
        ref_head = &(*(opcua_variable_type_t**)object)->reference_head;
        break;
    case DL_OPC_REF_TYPE:
        ref_head = &(*(opcua_reference_type_t**)object)->reference_head;
        break;
    case DL_OPC_DATA_TYPE:
        ref_head = &(*(opcua_data_type_t**)object)->reference_head;
        break;
    case DL_OPC_VIEW:
        ref_head = &(*(opcua_view_t**)object)->reference_head;
        break;
    default:
        break;
    }
    while((*ref_head) != NULL){
        next = (*ref_head)->next;
        free(*ref_head);
        (*ref_head) = next;
    }

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_free_reference(opcua_reference_t** ref)
{
    free(*ref);
    *ref = NULL;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_clear_reference(opcua_reference_t* ref)
{
    if(ref == NULL) return DL_OPCUA_INVAL;
    ref->id.i = 0;
    ref->id.ns = 0;
    if(ref->id.s != NULL) free(ref->id.s);
    ref->is_forward = -1;
    if(ref->type != NULL) free(ref->type);

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_add_reference(opcua_reference_t* reference,
        void* object, DL_OPCUA_TYPE_t object_type)
{
    switch(object_type){
        case DL_OPC_VARIABLE:
            if(((opcua_variable_t*)object)->reference_head == NULL){
                ((opcua_variable_t*)object)->reference_head = reference;
                return DL_OPCUA_OK;
            }else{
                opcua_reference_t* ref_head = 
                    ((opcua_variable_t*)object)->reference_head;
                while(ref_head->next != NULL) ref_head = ref_head->next;
                ref_head->next = reference;
            }
            break;
        case DL_OPC_METHOD:
            if(((opcua_method_t*)object)->reference_head == NULL){
                ((opcua_method_t*)object)->reference_head = reference;
                return DL_OPCUA_OK;
            }else{
                opcua_reference_t* ref_head = 
                    ((opcua_method_t*)object)->reference_head;
                while(ref_head->next != NULL) ref_head = ref_head->next;
                ref_head->next = reference;
            }
            break;
        case DL_OPC_OBJ:
            if(((opcua_object_t*)object)->reference_head == NULL){
                ((opcua_object_t*)object)->reference_head = reference;
                return DL_OPCUA_OK;
            }else{
                opcua_reference_t* ref_head = 
                    ((opcua_object_t*)object)->reference_head;
                while(ref_head->next != NULL) ref_head = ref_head->next;
                ref_head->next = reference;
            }
            break;
        case DL_OPC_OBJ_TYPE:
            if(((opcua_object_type_t*)object)->reference_head == NULL){
                ((opcua_object_type_t*)object)->reference_head = reference;
                return DL_OPCUA_OK;
            }else{
                opcua_reference_t* ref_head = 
                    ((opcua_object_type_t*)object)->reference_head;
                while(ref_head->next != NULL) ref_head = ref_head->next;
                ref_head->next = reference;
            }
            break;
        case DL_OPC_VAR_TYPE:
            if(((opcua_variable_type_t*)object)->reference_head == NULL){
                ((opcua_variable_type_t*)object)->reference_head = reference;
                return DL_OPCUA_OK;
            }else{
                opcua_reference_t* ref_head = 
                    ((opcua_variable_type_t*)object)->reference_head;
                while(ref_head->next != NULL) ref_head = ref_head->next;
                ref_head->next = reference;
            }
            break;
        case DL_OPC_REF_TYPE:
            if(((opcua_reference_type_t*)object)->reference_head == NULL){
                ((opcua_reference_type_t*)object)->reference_head = reference;
                return DL_OPCUA_OK;
            }else{
                opcua_reference_t* ref_head = 
                    ((opcua_reference_type_t*)object)->reference_head;
                while(ref_head->next != NULL) ref_head = ref_head->next;
                ref_head->next = reference;
            }
            break;
        case DL_OPC_DATA_TYPE:
            if(((opcua_data_type_t*)object)->reference_head == NULL){
                ((opcua_data_type_t*)object)->reference_head = reference;
                return DL_OPCUA_OK;
            }else{
                opcua_reference_t* ref_head = 
                    ((opcua_data_type_t*)object)->reference_head;
                while(ref_head->next != NULL) ref_head = ref_head->next;
                ref_head->next = reference;
            }
            break;
        case DL_OPC_VIEW:
            if(((opcua_view_t*)object)->reference_head == NULL){
                ((opcua_view_t*)object)->reference_head = reference;
                return DL_OPCUA_OK;
            }else{
                opcua_reference_t* ref_head = 
                    ((opcua_view_t*)object)->reference_head;
                while(ref_head->next != NULL) ref_head = ref_head->next;
                ref_head->next = reference;
            }
            break;
        default:
            break;
    }
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_add_reference_attributes(xmlNodePtr reference_node,
        opcua_reference_t* reference)
{
    char buffer[32];

    if(reference->type != NULL)
        xmlNewProp(reference_node, BAD_CAST "ReferenceType",
                BAD_CAST reference->type);

    DL_OPCUA_ERR_t ret = datalog_opcua_add_id_contents(reference_node, &reference->id);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    if((reference->is_forward == true) || (reference->is_forward == false)){
        sprintf(buffer, "%s", ((reference->is_forward == 1) ? "true" : "false"));
        xmlNewProp(reference_node, BAD_CAST "IsForward", BAD_CAST buffer);
    }

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_references(void* object, 
        DL_OPCUA_TYPE_t object_type)
{
    char buffer[32];

    switch(object_type){
        case DL_OPC_VARIABLE:
        if(((opcua_variable_t*)object)->reference_head == NULL) break;
        if(((opcua_variable_t*)object)->references_node == NULL)
            ((opcua_variable_t*)object)->references_node 
                = xmlNewChild(((opcua_variable_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_variable_t*)object)->references_node != NULL){
            if(((opcua_variable_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = 
                    ((opcua_variable_t*)object)->reference_head;
                while(ref_head != NULL){
                    xmlNodePtr tmp_node = 
                        xmlNewChild(((opcua_variable_t*)object)->references_node,
                        NULL, BAD_CAST "Reference", NULL);
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);
                    ref_head = ref_head->next;
                }
            }else return DL_OPCUA_INVAL;
        }else return DL_OPCUA_INVAL;
            break;
        case DL_OPC_METHOD:
        if(((opcua_method_t*)object)->reference_head == NULL) break;
        if(((opcua_method_t*)object)->references_node == NULL)
            ((opcua_method_t*)object)->references_node 
                = xmlNewChild(((opcua_method_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_method_t*)object)->references_node != NULL){
            if(((opcua_method_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = ((opcua_method_t*)object)->reference_head;
                while(ref_head != NULL){
                    xmlNodePtr tmp_node = 
                        xmlNewChild(((opcua_method_t*)object)->references_node,
                        NULL, BAD_CAST "Reference", NULL);
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);
                    ref_head = ref_head->next;
                }
            }else return DL_OPCUA_INVAL;
        }else return DL_OPCUA_INVAL;
            break;
        case DL_OPC_OBJ:
        if(((opcua_object_t*)object)->reference_head == NULL) break;
        if(((opcua_object_t*)object)->references_node == NULL)
            ((opcua_object_t*)object)->references_node 
                = xmlNewChild(((opcua_object_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_object_t*)object)->references_node != NULL){
            if(((opcua_object_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = ((opcua_object_t*)object)->reference_head;
                while(ref_head != NULL){
                    xmlNodePtr tmp_node = 
                        xmlNewChild(((opcua_object_t*)object)->references_node,
                        NULL, BAD_CAST "Reference", NULL);
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);
                    ref_head = ref_head->next;
                }
            }else return DL_OPCUA_INVAL;
        }else return DL_OPCUA_INVAL;
            break;
        case DL_OPC_OBJ_TYPE:
        if(((opcua_object_type_t*)object)->reference_head == NULL) break;
        if(((opcua_object_type_t*)object)->references_node == NULL)
            ((opcua_object_type_t*)object)->references_node 
                = xmlNewChild(((opcua_object_type_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_object_type_t*)object)->references_node != NULL){
            if(((opcua_object_type_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = 
                    ((opcua_object_type_t*)object)->reference_head;
                while(ref_head != NULL){
                    xmlNodePtr tmp_node = 
                        xmlNewChild(((opcua_object_type_t*)object)->references_node,
                        NULL, BAD_CAST "Reference", NULL);
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);
                    ref_head = ref_head->next;
                }
            }else return DL_OPCUA_INVAL;
        }else return DL_OPCUA_INVAL;
            break;
        case DL_OPC_VAR_TYPE:
        if(((opcua_variable_type_t*)object)->reference_head == NULL) break;
        if(((opcua_variable_type_t*)object)->references_node == NULL)
            ((opcua_variable_type_t*)object)->references_node 
                = xmlNewChild(((opcua_variable_type_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_variable_type_t*)object)->references_node != NULL){
            if(((opcua_variable_type_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = 
                    ((opcua_variable_type_t*)object)->reference_head;
                while(ref_head != NULL){
                    xmlNodePtr tmp_node = 
                        xmlNewChild(((opcua_variable_type_t*)object)->references_node,
                        NULL, BAD_CAST "Reference", NULL);
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);
                    ref_head = ref_head->next;
                }
            }else return DL_OPCUA_INVAL;
        }else return DL_OPCUA_INVAL;
            break;
        case DL_OPC_REF_TYPE:
        if(((opcua_reference_type_t*)object)->reference_head == NULL) break;
        if(((opcua_reference_type_t*)object)->references_node == NULL)
            ((opcua_reference_type_t*)object)->references_node 
                = xmlNewChild(((opcua_reference_type_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_reference_type_t*)object)->references_node != NULL){
            if(((opcua_reference_type_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = 
                    ((opcua_reference_type_t*)object)->reference_head;
                while(ref_head != NULL){
                    xmlNodePtr tmp_node = 
                        xmlNewChild(((opcua_reference_type_t*)object)->references_node,
                        NULL, BAD_CAST "Reference", NULL);
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);
                    ref_head = ref_head->next;
                }
            }else return DL_OPCUA_INVAL;
        }else return DL_OPCUA_INVAL;
            break;
        case DL_OPC_DATA_TYPE:
        if(((opcua_data_type_t*)object)->reference_head == NULL) break;
        if(((opcua_data_type_t*)object)->references_node == NULL)
            ((opcua_data_type_t*)object)->references_node 
                = xmlNewChild(((opcua_data_type_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_data_type_t*)object)->references_node != NULL){
            if(((opcua_data_type_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = 
                    ((opcua_data_type_t*)object)->reference_head;
                while(ref_head != NULL){
                    xmlNodePtr tmp_node = 
                        xmlNewChild(((opcua_data_type_t*)object)->references_node,
                        NULL, BAD_CAST "Reference", NULL);
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);
                    ref_head = ref_head->next;
                }
            }else return DL_OPCUA_INVAL;
        }else return DL_OPCUA_INVAL;
            break;
        case DL_OPC_VIEW:
        if(((opcua_view_t*)object)->reference_head == NULL) break;
        if(((opcua_view_t*)object)->references_node == NULL)
            ((opcua_view_t*)object)->references_node 
                = xmlNewChild(((opcua_view_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_view_t*)object)->references_node != NULL){
            if(((opcua_view_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = 
                    ((opcua_view_t*)object)->reference_head;
                while(ref_head != NULL){
                    xmlNodePtr tmp_node = 
                        xmlNewChild(((opcua_view_t*)object)->references_node,
                        NULL, BAD_CAST "Reference", NULL);
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);
                    ref_head = ref_head->next;
                }
            }else return DL_OPCUA_INVAL;
        }else return DL_OPCUA_INVAL;
            break;
        default:
            break;
    }

    return DL_OPCUA_OK;
}

//FREE
DL_OPCUA_ERR_t datalog_opcua_free_variable(opcua_variable_t** variable)
{
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;

    ret = datalog_opcua_free_reference_list((void**)variable, DL_OPC_VARIABLE);
    free((*variable)->variable_attributes);
    free((*variable)->attributes);

    free(*variable); 

    return DL_OPCUA_OK;
}

//ID
DL_OPCUA_ERR_t datalog_opcua_add_id_contents(xmlNodePtr node, opcua_node_id_t* id)
{
    char buffer[32]; 

    if((id->i) != 0 && (id->ns == 0)){
        sprintf(buffer, "i=%d", id->i);                
        xmlNodeSetContent(node, BAD_CAST buffer);
    }else if((id->i) == 0 && (id->ns != 0)){
        sprintf(buffer, "ns=%d", id->ns);
        xmlNodeSetContent(node, BAD_CAST buffer);
    }else if((id->i) != 0 && (id->ns != 0)){
        sprintf(buffer, "ns=%d;i=%d", id->ns, id->i);
        xmlNodeSetContent(node, BAD_CAST buffer);
    }

    if(id->s != NULL){
        char* node_value = (char*)xmlNodeGetContent(node);
        if(node_value == NULL){
            sprintf(buffer, "s=%s", id->s);
            xmlNodeSetContent(node, BAD_CAST buffer);
        }else{
            char second_buffer[strlen(node_value) + strlen(id->s) + 4];
            strcpy(second_buffer, node_value);
            strcpy(second_buffer + strlen(node_value), ";s=");
            strcpy(second_buffer + strlen(node_value) + 3, id->s);
            xmlNodeSetContent(node, BAD_CAST second_buffer);
        }
    }
    
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_add_id_attribute(xmlNodePtr node, char* attribute,
        opcua_node_id_t* id)
{
    char buffer[32]; 

    xmlAttrPtr tmp_attr = NULL; 

    if((id->i) != 0 && (id->ns == 0)){
        tmp_attr = xmlNewProp(node, BAD_CAST attribute, NULL);
        sprintf(buffer, "i=%d", id->i);                
        xmlSetProp(node, BAD_CAST attribute, BAD_CAST buffer);
    }else if((id->i) == 0 && (id->ns != 0)){
        tmp_attr = xmlNewProp(node, BAD_CAST attribute, NULL);
        sprintf(buffer, "ns=%d", id->ns);
        xmlSetProp(node, BAD_CAST attribute, BAD_CAST buffer);
    }else if((id->i) != 0 && (id->ns != 0)){
        tmp_attr = xmlNewProp(node, BAD_CAST attribute, NULL);
        sprintf(buffer, "ns=%d;i=%d", id->ns, id->i);
        xmlSetProp(node, BAD_CAST attribute, BAD_CAST buffer);
    }

    if(id->s != NULL){
        if(tmp_attr == NULL){
            sprintf(buffer, "s=%s", id->s);
            tmp_attr = xmlNewProp(node, BAD_CAST attribute, BAD_CAST buffer);
        }else{
            char* node_value = (char*)xmlGetProp(node, BAD_CAST attribute);
            char second_buffer[strlen(node_value) + strlen(id->s) + 4];
            strcpy(buffer, node_value);
            strcpy(buffer + strlen(node_value), ";s=");
            strcpy(buffer + strlen(node_value) + 3, id->s);
            xmlSetProp(node, BAD_CAST attribute, BAD_CAST buffer);
        }
    }
    
    return DL_OPCUA_OK;
}

//ATTRIBUTES
opcua_node_attributes_t* datalog_opcua_create_attributes(void)
{
    opcua_node_attributes_t* ret = (opcua_node_attributes_t*)
        calloc(1, sizeof(opcua_node_attributes_t));
    if(ret == NULL) return NULL;

    ret->set_parent_id_ns = &self_set_attribute_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_attribute_parent_node_id_i;
    ret->set_parent_id_s = &self_set_attribute_parent_node_id_s;
    ret->set_node_id_ns = &self_set_attribute_node_id_ns;
    ret->set_node_id_i = &self_set_attribute_node_id_i;
    ret->set_node_id_s = &self_set_attribute_node_id_s;
    ret->set_user_access_level = &self_set_attribute_user_access_level;
    ret->set_access_level = &self_set_attribute_access_level;
    ret->set_browse_name = &self_set_attribute_browse_name;
    ret->set_display_name = &self_set_attribute_display_name;
    ret->set_description = &self_set_attribute_description;

    return ret;
}

opcua_object_type_attributes_t* datalog_opcua_create_object_type_attributes(void)
{
    opcua_object_type_attributes_t* ret = (opcua_object_type_attributes_t*)
        calloc(1, sizeof(opcua_object_type_attributes_t));
    if(ret == NULL) return NULL;


    return ret;
}

opcua_method_attributes_t* datalog_opcua_create_method_attributes(void)
{
    opcua_method_attributes_t* ret = (opcua_method_attributes_t*)
        calloc(1, sizeof(opcua_method_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

opcua_variable_attributes_t* datalog_opcua_create_variable_attributes(void)
{
    opcua_variable_attributes_t* ret = (opcua_variable_attributes_t*)
        calloc(1, sizeof(opcua_variable_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

opcua_object_attributes_t* datalog_opcua_create_object_attributes(void)
{
    opcua_object_attributes_t* ret = (opcua_object_attributes_t*)
        calloc(1, sizeof(opcua_object_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

//VAR TYPE
opcua_variable_type_attributes_t* datalog_opcua_create_variable_type_attributes(void)
{
    opcua_variable_type_attributes_t* ret = (opcua_variable_type_attributes_t*)
        calloc(1, sizeof(opcua_variable_type_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

//REF TYPE
opcua_reference_type_attributes_t* datalog_opcua_create_reference_type_attributes(void)
{
    opcua_reference_type_attributes_t* ret = (opcua_reference_type_attributes_t*)
        calloc(1, sizeof(opcua_reference_type_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

//DAT TYPE
opcua_data_type_attributes_t* datalog_opcua_create_data_type_attributes(void)
{
    opcua_data_type_attributes_t* ret = (opcua_data_type_attributes_t*)
        calloc(1, sizeof(opcua_data_type_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

//VIEW
opcua_view_attributes_t* datalog_opcua_create_view_attributes(void)
{
    opcua_view_attributes_t* ret = (opcua_view_attributes_t*)
        calloc(1, sizeof(opcua_view_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

//STRUCT OBJECTS
opcua_node_id_t* datalog_opcua_create_node_id(void)
{
    opcua_node_id_t* ret = (opcua_node_id_t*)calloc(1, sizeof(opcua_node_id_t));
    if(ret == NULL) return NULL;

    ret->set_ns = &datalog_opcua_set_id_ns;
    ret->set_i = &datalog_opcua_set_id_i;
    ret->set_s = &datalog_opcua_set_id_s;
    ret->set_o = &datalog_opcua_set_id_o;
    ret->set_g = &datalog_opcua_set_id_g;

    return ret;
}

opcua_object_type_t* datalog_opcua_create_object_type(void)
{
    opcua_object_type_t* ret = (opcua_object_type_t*)
        calloc(1, sizeof(opcua_object_type_t));

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->object_type_attributes = datalog_opcua_create_object_type_attributes();
    if(ret->object_type_attributes == NULL) return NULL;

    ret->object_type_attributes->is_abstract = -1;

    ret->set_parent_id_ns = &self_set_object_type_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_object_type_parent_node_id_i;
    ret->set_parent_id_s = &self_set_object_type_parent_node_id_s;
    ret->set_node_id_ns = &self_set_object_type_node_id_ns;
    ret->set_node_id_i = &self_set_object_type_node_id_i;
    ret->set_node_id_s = &self_set_object_type_node_id_s;
    ret->set_browse_name = &self_set_object_type_browse_name;
    ret->set_display_name = &self_set_object_type_display_name;
    ret->set_description = &self_set_object_type_description;
    ret->create_references = &self_create_node_object_type_references;
    ret->add_reference = &self_add_ref_object_type;
    ret->create_node = &datalog_opcua_create_node_object_type;
    ret->set_is_abstract = &self_set_object_type_is_abstract;

    return ret;
}

opcua_method_t* datalog_opcua_create_method(void)
{
    opcua_method_t* ret = (opcua_method_t*)calloc(1, sizeof(opcua_method_t));
    if(ret == NULL) return NULL;

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->method_attributes = datalog_opcua_create_method_attributes();
    if(ret->method_attributes == NULL) return NULL;

    ret->set_parent_id_ns = &self_set_method_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_method_parent_node_id_i;
    ret->set_parent_id_s = &self_set_method_parent_node_id_s;
    ret->set_node_id_ns = &self_set_method_node_id_ns;
    ret->set_node_id_i = &self_set_method_node_id_i;
    ret->set_node_id_s = &self_set_method_node_id_s;
    ret->set_browse_name = &self_set_method_browse_name;
    ret->set_display_name = &self_set_method_display_name;
    ret->set_description = &self_set_method_description;
    ret->create_references = &self_create_node_method_references;
    ret->add_reference = &self_add_ref_method;
    ret->create_node = &datalog_opcua_create_node_method;
    ret->set_declaration_id = &self_set_method_declaration_id;

    return ret;
}

opcua_variable_t* datalog_opcua_create_variable(void)
{
    opcua_variable_t* ret = (opcua_variable_t*) 
        calloc(1, sizeof(opcua_variable_t));
    if(ret == NULL) return NULL;

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->variable_attributes = datalog_opcua_create_variable_attributes();
    if(ret->variable_attributes == NULL) return NULL;

    ret->variable_attributes->is_abstract = -1;

    ret->set_parent_id_ns = &self_set_variable_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_variable_parent_node_id_i;
    ret->set_parent_id_s = &self_set_variable_parent_node_id_s;
    ret->set_node_id_ns = &self_set_variable_node_id_ns;
    ret->set_node_id_i = &self_set_variable_node_id_i;
    ret->set_node_id_s = &self_set_variable_node_id_s;
    ret->set_browse_name = &self_set_variable_browse_name;
    ret->set_display_name = &self_set_variable_display_name;
    ret->set_description = &self_set_variable_description;
    ret->set_user_access_level = &self_set_variable_user_access_level;
    ret->set_access_level = &self_set_variable_access_level;
    ret->create_references = &self_create_node_variable_references;
    ret->add_reference = &self_add_ref_variable;
    ret->create_node = &datalog_opcua_create_node_variable;
    ret->set_value = &self_set_variable_value;
    ret->set_data_type = &self_set_variable_data_type;
    ret->set_array_dimensions = &self_set_variable_array_dimensions;
    ret->set_value_rank = &self_set_variable_value_rank;
    ret->set_is_abstract = &self_set_variable_is_abstract;

    return ret;
}

opcua_object_t* datalog_opcua_create_object(void)
{
    opcua_object_t* ret = (opcua_object_t*)
        calloc(1, sizeof(opcua_object_t));
    if(ret == NULL) return NULL;

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->object_attributes = datalog_opcua_create_object_attributes();
    if(ret->object_attributes == NULL) return NULL;

    ret->object_attributes->event_notifier = -1;

    ret->set_parent_id_ns = &self_set_object_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_object_parent_node_id_i;
    ret->set_parent_id_s = &self_set_object_parent_node_id_s;
    ret->set_node_id_ns = &self_set_object_node_id_ns;
    ret->set_node_id_i = &self_set_object_node_id_i;
    ret->set_node_id_s = &self_set_object_node_id_s;
    ret->set_browse_name = &self_set_object_browse_name;
    ret->set_display_name = &self_set_object_display_name;
    ret->set_description = &self_set_object_description;
    ret->create_references = &self_create_node_object_references;
    ret->add_reference = &self_add_ref_object;
    ret->create_node = &datalog_opcua_create_node_object;
    ret->set_event_notifier = &self_set_object_event_notifier;

    return ret;
}

//VARIABLE TYPE
opcua_variable_type_t* datalog_opcua_create_variable_type(void)
{
    opcua_variable_type_t* ret = (opcua_variable_type_t*)
        calloc(1, sizeof(opcua_variable_type_t));
    if(ret == NULL) return NULL;

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->variable_type_attributes = datalog_opcua_create_variable_type_attributes();
    if(ret->variable_type_attributes == NULL) return NULL;

    ret->variable_type_attributes->is_abstract = -1;

    ret->set_parent_id_ns = &self_set_variable_type_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_variable_type_parent_node_id_i;
    ret->set_parent_id_s = &self_set_variable_type_parent_node_id_s;
    ret->set_node_id_ns = &self_set_variable_type_node_id_ns;
    ret->set_node_id_i = &self_set_variable_type_node_id_i;
    ret->set_node_id_s = &self_set_variable_type_node_id_s;
    ret->set_browse_name = &self_set_variable_type_browse_name;
    ret->set_display_name = &self_set_variable_type_display_name;
    ret->set_description = &self_set_variable_type_description;
    ret->set_user_access_level = &self_set_variable_type_user_access_level;
    ret->set_access_level = &self_set_variable_type_access_level;
    ret->create_references = &self_create_node_variable_type_references;
    ret->add_reference = &self_add_ref_variable_type;
    ret->create_node = &datalog_opcua_create_node_variable_type;
    ret->set_value = &self_set_variable_type_value;
    ret->set_data_type = &self_set_variable_type_data_type;
    ret->set_array_dimensions = &self_set_variable_type_array_dimensions;
    ret->set_value_rank = &self_set_variable_type_value_rank;
    ret->set_is_abstract = &self_set_variable_type_is_abstract;

    return ret;
}
//REFERENCE TYPE
opcua_reference_type_t* datalog_opcua_create_reference_type(void)
{
    opcua_reference_type_t* ret = (opcua_reference_type_t*)
        calloc(1, sizeof(opcua_reference_type_t));
    if(ret == NULL) return NULL;

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->reference_type_attributes = datalog_opcua_create_reference_type_attributes();
    if(ret->reference_type_attributes == NULL) return NULL;

    ret->reference_type_attributes->is_abstract = -1;
    ret->reference_type_attributes->symmetric = -1;

    ret->set_parent_id_ns = &self_set_reference_type_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_reference_type_parent_node_id_i;
    ret->set_parent_id_s = &self_set_reference_type_parent_node_id_s;
    ret->set_node_id_ns = &self_set_reference_type_node_id_ns;
    ret->set_node_id_i = &self_set_reference_type_node_id_i;
    ret->set_node_id_s = &self_set_reference_type_node_id_s;
    ret->set_browse_name = &self_set_reference_type_browse_name;
    ret->set_display_name = &self_set_reference_type_display_name;
    ret->set_description = &self_set_reference_type_description;
    ret->set_user_access_level = &self_set_reference_type_user_access_level;
    ret->set_access_level = &self_set_reference_type_access_level;
    ret->create_references = &self_create_node_reference_type_references;
    ret->add_reference = &self_add_ref_reference_type;
    ret->create_node = &datalog_opcua_create_node_reference_type;
    ret->set_is_abstract = &self_set_reference_type_is_abstract;
    ret->set_symmetric = &self_set_reference_type_symmetric;
    ret->set_inverse_name = &self_set_reference_type_inverse_name;

    return ret;

}
//DATA TYPE
opcua_data_type_t* datalog_opcua_create_data_type(void)
{
    opcua_data_type_t* ret = (opcua_data_type_t*)
        calloc(1, sizeof(opcua_data_type_t));
    if(ret == NULL) return NULL;

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->data_type_attributes = datalog_opcua_create_data_type_attributes();
    if(ret->data_type_attributes == NULL) return NULL;

    ret->data_type_attributes->is_abstract = -1;

    ret->set_parent_id_ns = &self_set_data_type_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_data_type_parent_node_id_i;
    ret->set_parent_id_s = &self_set_data_type_parent_node_id_s;
    ret->set_node_id_ns = &self_set_data_type_node_id_ns;
    ret->set_node_id_i = &self_set_data_type_node_id_i;
    ret->set_node_id_s = &self_set_data_type_node_id_s;
    ret->set_browse_name = &self_set_data_type_browse_name;
    ret->set_display_name = &self_set_data_type_display_name;
    ret->set_description = &self_set_data_type_description;
    ret->set_user_access_level = &self_set_data_type_user_access_level;
    ret->set_access_level = &self_set_data_type_access_level;
    ret->create_references = &self_create_node_data_type_references;
    ret->add_reference = &self_add_ref_data_type;
    ret->create_node = &datalog_opcua_create_node_data_type;
    ret->set_is_abstract = &self_set_data_type_is_abstract;

    return ret;

}
//VIEW
opcua_view_t* datalog_opcua_create_view(void)
{
    opcua_view_t* ret = (opcua_view_t*)
        calloc(1, sizeof(opcua_view_t));
    if(ret == NULL) return NULL;

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->view_attributes = datalog_opcua_create_view_attributes();
    if(ret->view_attributes == NULL) return NULL;

    ret->view_attributes->contains_no_loops = -1;

    ret->set_parent_id_ns = &self_set_view_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_view_parent_node_id_i;
    ret->set_parent_id_s = &self_set_view_parent_node_id_s;
    ret->set_node_id_ns = &self_set_view_node_id_ns;
    ret->set_node_id_i = &self_set_view_node_id_i;
    ret->set_node_id_s = &self_set_view_node_id_s;
    ret->set_browse_name = &self_set_view_browse_name;
    ret->set_display_name = &self_set_view_display_name;
    ret->set_description = &self_set_view_description;
    ret->set_user_access_level = &self_set_view_user_access_level;
    ret->set_access_level = &self_set_view_access_level;
    ret->create_references = &self_create_node_view_references;
    ret->add_reference = &self_add_ref_view;
    ret->create_node = &datalog_opcua_create_node_view;
    ret->set_contains_no_loops = &self_set_view_contains_no_loops;
    ret->set_event_notifier = &self_set_view_event_notifier;

    return ret;
}

opcua_reference_t* datalog_opcua_create_reference(void)
{
    opcua_reference_t* ret = (opcua_reference_t*)
        calloc(1, sizeof(opcua_reference_t));
    if(ret == NULL) return NULL;

    //undef state
    ret->is_forward = -1;

    ret->set_id_ns = &self_set_reference_id_ns;
    ret->set_id_i = &self_set_reference_id_i;
    ret->set_id_s = &self_set_reference_id_s;
    ret->set_type = &self_set_reference_type;
    ret->set_is_forward = &self_set_reference_is_forward;
    ret->add_reference = &datalog_opcua_add_reference;
    ret->free_reference = &datalog_opcua_free_reference;

    return ret;
}

//RUNTIME
void datalog_opcua_runtime(void)
{
    printf("opcua runtime\n");

    DL_OPCUA_ERR_t ret  = datalog_opcua_init_doc();

    if(ret != DL_OPCUA_OK) return;

//TEST CODE
    opcua_node_id_t* test_node_id = datalog_opcua_create_node_id();
    test_node_id->set_ns(test_node_id, 51);
    test_node_id->set_i(test_node_id, 52);
    test_node_id->set_s(test_node_id, "test node id");

    //object type
    opcua_node_attributes_t* test_attributes = datalog_opcua_create_attributes();

    test_attributes->set_node_id_ns(test_attributes, 1);
    test_attributes->set_node_id_i(test_attributes, 1001);
    test_attributes->set_browse_name(test_attributes, "1:ControllerTypeXml");
    test_attributes->set_display_name(test_attributes, "ControllerTypeXml");

    datalog_opcua_create_node_object_type_from_array(object_type_array, 
            test_attributes);

    //variable
    opcua_variable_t* test_variable = datalog_opcua_create_variable();

    test_variable->set_parent_id_i(test_variable, 1001);
    test_variable->set_parent_id_ns(test_variable, 1);
    test_variable->set_node_id_i(test_variable, 6001);
    test_variable->set_node_id_ns(test_variable, 1);
    test_variable->set_browse_name(test_variable, "1:PowerConsumption");
    test_variable->set_display_name(test_variable, "PowerConsumption");
    test_variable->set_user_access_level(test_variable, 3);
    test_variable->set_access_level(test_variable, 3);
    test_variable->set_description(test_variable, "test description of the variable nod:e");
    test_variable->set_value_rank(test_variable, 1111);
    test_variable->set_array_dimensions(test_variable, 11);
    test_variable->set_is_abstract(test_variable, true);
    test_variable->set_data_type(test_variable, test_node_id);

    datalog_opcua_create_node_variable(test_variable); 
    opcua_reference_t* test_ref = datalog_opcua_create_reference();
    test_ref->set_type(test_ref, "HasSubtype");
    test_ref->set_is_forward(test_ref, false);
    test_ref->set_id_i(test_ref, 58);
    test_variable->add_reference(test_variable, test_ref);

    test_ref = datalog_opcua_create_reference();
    test_ref->set_type(test_ref, "HasComponent");
    test_ref->set_id_i(test_ref, 6001);
    test_ref->set_id_ns(test_ref, 1);
    test_variable->add_reference(test_variable, test_ref);

    test_variable->create_references(test_variable);

    datalog_opcua_free_variable(&test_variable);

    //method
    opcua_method_t* test_method = datalog_opcua_create_method();

    test_method->set_node_id_i(test_method, 1);
    test_method->set_node_id_ns(test_method, 7001);
    test_method->set_parent_id_i(test_method, 1001);
    test_method->set_parent_id_ns(test_method, 1);
    test_method->set_parent_id_s(test_method, "testParentS");
    test_method->set_browse_name(test_method, "1:Start");
    test_method->set_display_name(test_method, "Start");
    test_method->set_declaration_id(test_method, 666);

    ret = datalog_opcua_create_node_method(test_method);
    
    test_ref = datalog_opcua_create_reference();
    test_ref->set_id_i(test_ref, 78);
    test_ref->set_type(test_ref, "HasModellingRule");
    test_method->add_reference(test_method, test_ref);

    test_ref = datalog_opcua_create_reference();
    test_ref->set_id_i(test_ref, 1001);
    test_ref->set_id_ns(test_ref, 1);
    test_ref->set_is_forward(test_ref, false);
    test_ref->set_type(test_ref, "HasComponent");
    test_method->add_reference(test_method, test_ref);

    test_method->create_references(test_method);

    //object
    opcua_object_t* test_object = datalog_opcua_create_object();

    test_object->set_node_id_i(test_object, 5001);
    test_object->set_node_id_s(test_object, "TESTS");
    test_object->set_node_id_ns(test_object, 1);
    test_object->set_parent_id_i(test_object, 1001);
    test_object->set_parent_id_ns(test_object, 1);
    test_object->set_browse_name(test_object, "1:StateCondition");
    test_object->set_display_name(test_object, "StateCondition");
    test_object->set_event_notifier(test_object, 10);

    ret = datalog_opcua_create_node_object(test_object);

    test_ref = datalog_opcua_create_reference();
    test_ref->set_id_ns(test_ref, 1);
    test_ref->set_id_i(test_ref, 6010);
    test_ref->set_type(test_ref, "HasComponent");
    test_object->add_reference(test_object, test_ref);

    test_ref = datalog_opcua_create_reference();
    test_ref->set_id_i(test_ref, 6014);
    test_ref->set_id_ns(test_ref, 1);
    test_ref->set_type(test_ref, "HasProperty");
    test_object->add_reference(test_object, test_ref);

    test_object->create_references(test_object);

    if(ret != DL_OPCUA_OK) return;

    //variable type
    opcua_variable_type_t* test_variable_type = datalog_opcua_create_variable_type(); 

    test_variable_type->set_node_id_i(test_variable_type, 1);
    test_variable_type->set_node_id_ns(test_variable_type, 7001);
    test_variable_type->set_parent_id_i(test_variable_type, 1001);
    test_variable_type->set_parent_id_ns(test_variable_type, 1);
    test_variable_type->set_parent_id_s(test_variable_type, "testParentS");
    test_variable_type->set_browse_name(test_variable_type, "1:Start");
    test_variable_type->set_display_name(test_variable_type, "Start");
    test_variable_type->set_access_level(test_variable_type, 10);
    test_variable_type->set_user_access_level(test_variable_type, 12);
    test_variable_type->set_value_rank(test_variable_type, 1111);
    test_variable_type->set_array_dimensions(test_variable_type, 11);
    test_variable_type->set_is_abstract(test_variable_type, true);
    test_variable_type->set_data_type(test_variable_type, test_node_id);

    ret = test_variable_type->create_node(test_variable_type);

    test_variable_type->add_reference(test_variable_type, test_ref);
    test_variable_type->create_references(test_variable_type);
    
    //reference type
    opcua_reference_type_t* test_reference_type = datalog_opcua_create_reference_type();  
   
    test_reference_type->set_node_id_i(test_reference_type, 1);
    test_reference_type->set_node_id_ns(test_reference_type, 7001);
    test_reference_type->set_parent_id_i(test_reference_type, 1001);
    test_reference_type->set_parent_id_ns(test_reference_type, 1);
    test_reference_type->set_parent_id_s(test_reference_type, "testParentS");
    test_reference_type->set_browse_name(test_reference_type, "1:Start");
    test_reference_type->set_display_name(test_reference_type, "Start");
    test_reference_type->set_access_level(test_reference_type, 10);
    test_reference_type->set_user_access_level(test_reference_type, 12);
    test_reference_type->set_is_abstract(test_reference_type, true);
    test_reference_type->set_symmetric(test_reference_type, false);
    test_reference_type->set_inverse_name(test_reference_type, "inverse name test");

    ret = test_reference_type->create_node(test_reference_type);

    test_reference_type->add_reference(test_reference_type, test_ref);
    test_reference_type->create_references(test_reference_type);
    
    //data type
    opcua_data_type_t* test_data_type = datalog_opcua_create_data_type(); 
   
    test_data_type->set_node_id_i(test_data_type, 1);
    test_data_type->set_node_id_ns(test_data_type, 7001);
    test_data_type->set_parent_id_i(test_data_type, 1001);
    test_data_type->set_parent_id_ns(test_data_type, 1);
    test_data_type->set_parent_id_s(test_data_type, "testParentS");
    test_data_type->set_browse_name(test_data_type, "1:Start");
    test_data_type->set_display_name(test_data_type, "Start");
    test_data_type->set_access_level(test_data_type, 10);
    test_data_type->set_user_access_level(test_data_type, 12);
    test_data_type->set_is_abstract(test_data_type, true);

    ret = test_data_type->create_node(test_data_type);

    test_data_type->add_reference(test_data_type, test_ref);
    test_data_type->create_references(test_data_type);
    
    //view
    opcua_view_t* test_view = datalog_opcua_create_view();
    
    test_view->set_node_id_i(test_view, 1);
    test_view->set_node_id_ns(test_view, 7001);
    test_view->set_parent_id_i(test_view, 1001);
    test_view->set_parent_id_ns(test_view, 1);
    test_view->set_parent_id_s(test_view, "testParentS");
    test_view->set_browse_name(test_view, "1:Start");
    test_view->set_display_name(test_view, "Start");
    test_view->set_access_level(test_view, 10);
    test_view->set_user_access_level(test_view, 12);
    test_view->set_contains_no_loops(test_view, false);
    test_view->set_event_notifier(test_view, 60);

    ret = test_view->create_node(test_view);

    test_view->add_reference(test_view, test_ref);
    test_view->create_references(test_view);

//TEST CODE END

    datalog_opcua_save_deinit_doc();
}
