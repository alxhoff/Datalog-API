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

    //OBJECT TYPES
    opcua_object_type_t* tmp_obj_type = datalog_opcua_create_object_type();
    tmp_obj_type->set_browse_name(tmp_obj_type, "testObjectTypeBrowseName");
    tmp_obj_type->set_node_id_s(tmp_obj_type, "testS");
    datalog_opcua_create_node_object_type(tmp_obj_type);

    opcua_reference_t* tmp_ref = datalog_opcua_create_reference();
    i = 0;
    while(object_type_array[i].type != NULL){
        datalog_opcua_clear_reference(tmp_ref);
//        if(object_type_array[i].type != NULL) 
//            tmp_ref->set_type(tmp_ref, object_type_array[i].type);
/*        
        tmp_ref->set_id_i(tmp_ref, object_type_array[i].id.i);
        tmp_ref->set_id_ns(tmp_ref, object_type_array[i].id.ns);
        if(object_type_array[i].id.s != NULL)
            tmp_ref->set_id_s(tmp_ref, object_type_array[i].id.s);
        tmp_ref->set_is_forward(tmp_ref, object_type_array[i].is_forward);

        datalog_opcua_add_reference(tmp_obj_type->node, DL_OPC_OBJ_TYPE, tmp_ref);
*/
        
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
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self, DL_OPC_VARIABLE,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_type_display_name(opcua_object_type_t* self, char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self, DL_OPC_VARIABLE,
            display_name);
    return ret;
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

DL_OPCUA_ERR_t self_set_variable_data_type(opcua_variable_t* var,
        char* data_type)
{
    var->variable_attributes->data_type = 
        (char*)realloc(var->variable_attributes->data_type, sizeof(char) * (strlen(data_type) + 1));
    if(var->variable_attributes->data_type == NULL) return DL_OPCUA_MEM;
    strcpy(var->variable_attributes->data_type, data_type);
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_user_access_level(opcua_variable_t* self, int ual)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_attributes->user_access_level = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_access_level(opcua_variable_t* self, int al)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_attributes->access_level = al;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_array_dimensions(opcua_variable_t* self, int ad)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_attributes->array_dimensions = ad;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_value_rank(opcua_variable_t* self, int vr)
{
    if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;
    self->variable_attributes->value_rank = vr;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_variable_browse_name(opcua_variable_t* self, char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self, DL_OPC_VARIABLE,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_display_name(opcua_variable_t* self, char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self, DL_OPC_VARIABLE,
            display_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_variable_description(opcua_variable_t* variable,
        char* description)
{
    variable->description = (char*)realloc(variable->description,
            sizeof(char) * (strlen(description) + 1));
    if(variable->description == NULL) return DL_OPCUA_MEM;
    strcpy(variable->description, description);
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
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self, DL_OPC_METHOD,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_method_display_name(opcua_method_t* self, char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self, DL_OPC_METHOD,
            display_name);
    return ret;
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
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self, DL_OPC_OBJ,
            browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_object_display_name(opcua_object_t* self, char* display_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_display_name(self, DL_OPC_OBJ,
            display_name);
    return ret;
}

//## REFERENCE
DL_OPCUA_ERR_t self_set_reference_type(opcua_reference_t* reference,
        char* type)
{
    reference->type = (char*)realloc(reference->type, 
        sizeof(char) * (strlen(type) + 1));
    if(reference->type == NULL) return DL_OPCUA_MEM;

    strcpy(reference->type, type);

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

//## GENERALISED FUNCTIONS
DL_OPCUA_ERR_t datalog_opcua_set_browse_name(void* object, DL_OPCUA_TYPE_t type,
        char* browse_name)
{
    switch(type){
    case DL_OPC_VARIABLE:
        ((opcua_variable_t*)object)->attributes->browse_name = 
            (char*)realloc(((opcua_variable_t*)object)->attributes->browse_name, 
                    sizeof(char) * (strlen(browse_name) + 1));
        if(((opcua_variable_t*)object)->attributes->browse_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_variable_t*)object)->attributes->browse_name, browse_name);
        break;
    case DL_OPC_METHOD:
        ((opcua_method_t*)object)->attributes->browse_name = 
            (char*)realloc(((opcua_method_t*)object)->attributes->browse_name, 
                    sizeof(char) * (strlen(browse_name) + 1));
        if(((opcua_method_t*)object)->attributes->browse_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_method_t*)object)->attributes->browse_name, browse_name);
        break;
    case DL_OPC_OBJ:
        ((opcua_object_t*)object)->attributes->browse_name = 
            (char*)realloc(((opcua_object_t*)object)->attributes->browse_name, 
                    sizeof(char) * (strlen(browse_name) + 1));
        if(((opcua_object_t*)object)->attributes->browse_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_object_t*)object)->attributes->browse_name, browse_name);
        break;
    default:
        break;
    }

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_set_display_name(void* object, DL_OPCUA_TYPE_t type, 
        char* display_name)
{
    switch(type){
    case DL_OPC_VARIABLE:
        ((opcua_variable_t*)object)->attributes->display_name = 
            (char*)realloc(((opcua_variable_t*)object)->attributes->display_name, 
                    sizeof(char) * (strlen(display_name) + 1));
        if(((opcua_variable_t*)object)->attributes->display_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_variable_t*)object)->attributes->display_name, display_name);
        break;
    case DL_OPC_METHOD:
        ((opcua_method_t*)object)->attributes->display_name = 
            (char*)realloc(((opcua_method_t*)object)->attributes->display_name, 
                    sizeof(char) * (strlen(display_name) + 1));
        if(((opcua_method_t*)object)->attributes->display_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_method_t*)object)->attributes->display_name, display_name);
        break;
    case DL_OPC_OBJ:
        ((opcua_object_t*)object)->attributes->display_name = 
            (char*)realloc(((opcua_object_t*)object)->attributes->display_name, 
                    sizeof(char) * (strlen(display_name) + 1));
        if(((opcua_object_t*)object)->attributes->display_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_object_t*)object)->attributes->display_name, display_name);
        break;
    default:
        break;
    }
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_set_id_s(opcua_ns_id_t* id, char* value)
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

DL_OPCUA_ERR_t datalog_opcua_set_id_ns(opcua_ns_id_t* id, int ns)
{
    id->ns = ns;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_set_id_i(opcua_ns_id_t* id, int i)
{
    id->i = i;
    return DL_OPCUA_OK;
}

//CREATE OBJECT NODES
//ATTRIBUTES
DL_OPCUA_ERR_t datalog_opcua_create_node_attributes(xmlNodePtr parent_node, 
        opcua_node_attributes_t* attributes)
{
    char buffer[32];
    
    if(attributes->browse_name != NULL)
        xmlNewProp(parent_node, BAD_CAST "BrowseName",
                BAD_CAST attributes->browse_name);
    
    DL_OPCUA_ERR_t ret = datalog_opcua_add_id_attribute(parent_node, "ParentNodeId",
        &attributes->parent_node_id);            
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    ret = datalog_opcua_add_id_attribute(parent_node, "NodeId",
        &attributes->node_id);            
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_object_type_attributes(
        opcua_object_type_t* object_t)
{
    if(object_t->attributes->display_name != NULL)
        xmlNewProp(object_t->node, BAD_CAST "DisplayName", 
                BAD_CAST object_t->attributes->display_name);

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_method_attributes(opcua_method_t* method)
{
    char buffer[32];

    if(method->method_attributes->method_declaration_id != 0){
        sprintf(buffer, "i=%d", method->method_attributes->method_declaration_id);
        xmlNewProp(method->node, BAD_CAST "MethodDeclarationId", BAD_CAST buffer);
    }

    if(method->attributes->display_name != NULL)
        xmlNewProp(method->node, BAD_CAST "DisplayName", 
                BAD_CAST method->attributes->display_name);

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_variable_attributes(opcua_variable_t* variable)
{
    char buffer[32];

    if(variable->variable_attributes->data_type != NULL)
        xmlNewProp(variable->node, BAD_CAST "DataType",
                BAD_CAST variable->variable_attributes->data_type);
    if(variable->variable_attributes->user_access_level != 0){
        sprintf(buffer, "%d", variable->variable_attributes->user_access_level);
        xmlNewProp(variable->node, BAD_CAST "UserAccessLevel", BAD_CAST buffer);
    }
    if(variable->variable_attributes->access_level != 0){
        sprintf(buffer, "%d", variable->variable_attributes->access_level);
        xmlNewProp(variable->node, BAD_CAST "AccessLevel", BAD_CAST buffer);
    }
    if(variable->variable_attributes->array_dimensions != 0){
        sprintf(buffer, "%d", variable->variable_attributes->array_dimensions);
        xmlNewProp(variable->node, BAD_CAST "ArrayDimensions", BAD_CAST buffer);
    }
    if(variable->variable_attributes->value_rank != 0){
        sprintf(buffer, "%d", variable->variable_attributes->array_dimensions);
        xmlNewProp(variable->node, BAD_CAST "ValueRank", BAD_CAST buffer);
    }
    
    if(variable->attributes->display_name != NULL)
        xmlNewProp(variable->node, BAD_CAST "DisplayName", 
                BAD_CAST variable->attributes->display_name);

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_object_attributes(opcua_object_t* object)
{
    if(object->attributes->display_name != NULL)
        xmlNewProp(object->node, BAD_CAST "DisplayName", 
                BAD_CAST object->attributes->display_name);
    
    return DL_OPCUA_OK;
}

//OBJECTS
DL_OPCUA_ERR_t datalog_opcua_create_node_object_type(opcua_object_type_t* object)
{
    if(object == NULL) return DL_OPCUA_INVAL;
    if(object->attributes == NULL) return DL_OPCUA_INVAL;
    if(object->object_type_attributes == NULL) return DL_OPCUA_INVAL;

    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;
    
    object->node = xmlNewChild(opcua_document->root_node,
            NULL, BAD_CAST "UAObjectType", NULL);

    ret = datalog_opcua_create_node_attributes(object->node, object->attributes);
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
            NULL, BAD_CAST "UAObject", NULL);

    ret = datalog_opcua_create_node_attributes(method->node, method->attributes);
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

    ret = datalog_opcua_create_node_attributes(variable->node, 
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

    ret = datalog_opcua_create_node_attributes(object->node, 
            object->attributes);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;
    
    ret = datalog_opcua_create_node_object_attributes(object);
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;

    return DL_OPCUA_OK;
}

//REFERENCE
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

DL_OPCUA_ERR_t datalog_opcua_add_reference(void* object, 
        DL_OPCUA_TYPE_t object_type, opcua_reference_t* reference)
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
        if(((opcua_variable_t*)object)->references_node == NULL)
            ((opcua_variable_t*)object)->references_node 
                = xmlNewChild(((opcua_variable_t*)object)->node,
                    NULL, BAD_CAST "References", NULL);
        if(((opcua_variable_t*)object)->references_node != NULL){
            if(((opcua_variable_t*)object)->reference_head != NULL){
                opcua_reference_t* ref_head = ((opcua_variable_t*)object)->reference_head;
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
        default:
            break;
    }

    return DL_OPCUA_OK;
}

//ID
DL_OPCUA_ERR_t datalog_opcua_add_id_contents(xmlNodePtr node, opcua_ns_id_t* id)
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
        opcua_ns_id_t* id)
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

//STRUCT OBJECTS
opcua_object_type_t* datalog_opcua_create_object_type(void)
{
    opcua_object_type_t* ret = (opcua_object_type_t*)
        calloc(1, sizeof(opcua_object_type_t));

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->object_type_attributes = datalog_opcua_create_object_type_attributes();
    if(ret->object_type_attributes == NULL) return NULL;

    //set function pointers
    ret->set_parent_id_ns = &self_set_object_type_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_object_type_parent_node_id_i;
    ret->set_parent_id_s = &self_set_object_type_parent_node_id_s;
    ret->set_node_id_ns = &self_set_object_type_node_id_ns;
    ret->set_node_id_i = &self_set_object_type_node_id_i;
    ret->set_node_id_s = &self_set_object_type_node_id_s;
    ret->set_browse_name = &self_set_object_type_browse_name;
    ret->set_display_name = &self_set_object_type_display_name;

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

    //set function pointers
    ret->set_parent_id_ns = &self_set_method_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_method_parent_node_id_i;
    ret->set_parent_id_s = &self_set_method_parent_node_id_s;
    ret->set_node_id_ns = &self_set_method_node_id_ns;
    ret->set_node_id_i = &self_set_method_node_id_i;
    ret->set_node_id_s = &self_set_method_node_id_s;
    ret->set_browse_name = &self_set_method_browse_name;
    ret->set_display_name = &self_set_method_display_name;
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

    ret->set_parent_id_ns = &self_set_variable_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_variable_parent_node_id_i;
    ret->set_parent_id_s = &self_set_variable_parent_node_id_s;
    ret->set_node_id_ns = &self_set_variable_node_id_ns;
    ret->set_node_id_i = &self_set_variable_node_id_i;
    ret->set_node_id_s = &self_set_variable_node_id_s;
    ret->set_browse_name = &self_set_variable_browse_name;
    ret->set_display_name = &self_set_variable_display_name;

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

    ret->set_parent_id_ns = &self_set_object_parent_node_id_ns;
    ret->set_parent_id_i = &self_set_object_parent_node_id_i;
    ret->set_parent_id_s = &self_set_object_parent_node_id_s;
    ret->set_node_id_ns = &self_set_object_node_id_ns;
    ret->set_node_id_i = &self_set_object_node_id_i;
    ret->set_node_id_s = &self_set_object_node_id_s;
    ret->set_browse_name = &self_set_object_browse_name;
    ret->set_display_name = &self_set_object_display_name;

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

    return ret;
}

//RUNTIME
void datalog_opcua_runtime(void)
{
    printf("opcua runtime\n");

    DL_OPCUA_ERR_t ret  = datalog_opcua_init_doc();

    if(ret != DL_OPCUA_OK) return;

//TEST CODE
    opcua_method_t* test_method = datalog_opcua_create_method();

    test_method->set_display_name(test_method, "testDisplayName");

    test_method->set_browse_name(test_method, "testBrowseName");

    test_method->set_node_id_i(test_method, 1);
    test_method->set_node_id_ns(test_method, 2);
    test_method->set_node_id_s(test_method, "testS");
    test_method->set_parent_id_i(test_method, 3);
    test_method->set_parent_id_ns(test_method, 4);
    test_method->set_parent_id_s(test_method, "testParentS");
    test_method->set_browse_name(test_method, "testBrowseName");
    test_method->set_display_name(test_method, "testDisplayName");
    test_method->set_declaration_id(test_method, 5);

    ret = datalog_opcua_create_node_method(test_method);
    
    opcua_reference_t* test_ref = datalog_opcua_create_reference();
    
    test_ref->set_id_i(test_ref, 6);
    
    datalog_opcua_add_reference(test_method, DL_OPC_METHOD, test_ref);

    test_ref = datalog_opcua_create_reference();

    test_ref->set_id_i(test_ref, 7);
    test_ref->set_id_ns(test_ref, 8);
    test_ref->set_is_forward(test_ref, false);
    test_ref->set_id_s(test_ref, "testRefS"); 
    test_ref->set_type(test_ref, "testType");
    
    datalog_opcua_add_reference(test_method, DL_OPC_METHOD, test_ref);

    test_ref = datalog_opcua_create_reference();

    test_ref->set_id_ns(test_ref, 9);
    test_ref->set_is_forward(test_ref, true);
    
    datalog_opcua_add_reference(test_method, DL_OPC_METHOD, test_ref);
//TODO OBJECT FUNCTION
    ret = datalog_opcua_create_node_references(test_method, DL_OPC_METHOD);

    if(ret != DL_OPCUA_OK) return;
//TEST CODE END

    datalog_opcua_save_deinit_doc();
}

