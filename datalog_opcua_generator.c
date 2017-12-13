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

#define object_type_ATTS    SET_IS_ABSTRACT(object_type)
        
#define variable_ATTS       SET_VALUE(variable)         \
                            SET_DATA_TYPE(variable)     \
                            SET_VALUE_RANK(variable)    \
                            SET_ARRAY_DIM(variable)     \
                            SET_ACCESS_LEVEL(variable)  \
                            SET_U_ACCESS_LEVEL(variable)\
                            SET_MIN_SAMP_INT(variable)  \
                            SET_HISTORIZING(variable)   

#define method_ATTS         SET_DEC_ID(method)          \
                            SET_EXECUTABLE(method)      \
                            SET_U_EXECUTABLE(method)     

#define object_ATTS         SET_EVENT_NOTIFIER(object) 

#define variable_type_ATTS  SET_VALUE(variable_type)    \
                            SET_DATA_TYPE(variable_type)\
                            SET_ARRAY_DIM(variable_type)\
                            SET_VALUE_RANK(variable_type)\
                            SET_IS_ABSTRACT(variable_type)

#define reference_type_ATTS    SET_IS_ABSTRACT(reference_type) \
                                SET_SYMMETRIC(reference_type)   \
                                SET_INVERSE_NAME(reference_type)

#define data_type_ATTS     SET_IS_ABSTRACT(data_type)

#define view_ATTS          SET_CONT_NO_LOOPS(view)     \
                            SET_EVENT_NOTIFIER(view)    

#define BASE_NODE(TYPE)     SET_PARENT_ID_NS(TYPE)      \
                            SET_PARENT_ID_I(TYPE)       \
                            SET_PARENT_ID_S(TYPE)       \
                            SET_NODE_ID_NS(TYPE)        \
                            SET_NODE_ID_I(TYPE)         \
                            SET_NODE_ID_S(TYPE)         \
                            SET_BROWSE_NAME(TYPE)       \
                            SET_DISPLAY_NAME(TYPE)      \
                            SET_DESCRIPTION(TYPE)       \
                            SET_U_WRITE_MASK(TYPE)      \
                            SET_WRITE_MASK(TYPE)        \
                            TYPE##_ATTS

#define SET_PARENT_ID_NS(TYPE)  DL_OPCUA_ERR_t self_set_##TYPE##_parent_node_id_ns(             \
                                opcua_##TYPE##_t* self, int ns){                                \
                            DL_OPCUA_ERR_t ret =                                                \
                                datalog_opcua_set_id_ns(&self->attributes->parent_node_id, ns); \
                            return ret;}

#define SET_PARENT_ID_I(TYPE)   DL_OPCUA_ERR_t self_set_##TYPE##_parent_node_id_i(              \
                                opcua_##TYPE##_t* self, int i){                                 \
                            DL_OPCUA_ERR_t ret =                                                \
                            datalog_opcua_set_id_i(&self->attributes->parent_node_id, i);       \
                            return ret;}

#define SET_PARENT_ID_S(TYPE)   DL_OPCUA_ERR_t self_set_##TYPE##_parent_node_id_s(              \
                                opcua_##TYPE##_t* self, char* s){                               \
                            DL_OPCUA_ERR_t ret =                                                \
                                datalog_opcua_set_id_s(&self->attributes->parent_node_id, s);   \
                            return ret;}

#define SET_NODE_ID_NS(TYPE)    DL_OPCUA_ERR_t self_set_##TYPE##_node_id_ns(                    \
                                opcua_##TYPE##_t* self, int ns){                                \
                            DL_OPCUA_ERR_t ret =                                                \
                                datalog_opcua_set_id_ns(&self->attributes->node_id, ns);        \
                            return ret;}

#define SET_NODE_ID_I(TYPE)     DL_OPCUA_ERR_t self_set_##TYPE##_node_id_i(                     \
                                opcua_##TYPE##_t* self, int i){                                 \
                            DL_OPCUA_ERR_t ret =                                                \
                                datalog_opcua_set_id_i(&self->attributes->node_id, i);          \
                            return ret;}

#define SET_NODE_ID_S(TYPE)     DL_OPCUA_ERR_t self_set_##TYPE##_node_id_s(                     \
                                opcua_##TYPE##_t* self, char* s){                               \
                            DL_OPCUA_ERR_t ret =                                                \
                                datalog_opcua_set_id_s(&self->attributes->node_id, s);          \
                            return ret;}

#define SET_BROWSE_NAME(TYPE)   DL_OPCUA_ERR_t self_set_##TYPE##_browse_name(                   \
                                opcua_##TYPE##_t* self, char* browse_name){                     \
                            DL_OPCUA_ERR_t ret =                                                \
                                datalog_opcua_set_browse_name(self->attributes, browse_name);   \
                            return ret;}

#define SET_DISPLAY_NAME(TYPE)  DL_OPCUA_ERR_t self_set_##TYPE##_display_name(                  \
                                opcua_##TYPE##_t* self,char* display_name){                     \
                            DL_OPCUA_ERR_t ret =                                                \
                                datalog_opcua_set_display_name(self->attributes,display_name);  \
                            return ret;}

#define SET_DESCRIPTION(TYPE)   DL_OPCUA_ERR_t self_set_##TYPE##_description(                   \
                                opcua_##TYPE##_t* self,char* description){                      \
                            DL_OPCUA_ERR_t ret =                                                \
                                datalog_opcua_set_description(self->attributes, description);   \
                            return ret;}                            

#define SET_U_WRITE_MASK(TYPE)  DL_OPCUA_ERR_t self_set_##TYPE##_user_write_mask(               \
                                opcua_##TYPE##_t* self, int ual){                               \
                            if(self->attributes == NULL) return DL_OPCUA_INVAL;                 \
                            self->attributes->user_write_mask = ual;                            \
                            return DL_OPCUA_OK;}

#define SET_WRITE_MASK(TYPE)    DL_OPCUA_ERR_t self_set_##TYPE##_write_mask(                    \
                                opcua_##TYPE##_t* self, int al){                                \
                            if(self->attributes == NULL) return DL_OPCUA_INVAL;                 \
                            self->attributes->write_mask = al;                                  \
                            return DL_OPCUA_OK;}

//SPECIFIC

#define SET_IS_ABSTRACT(TYPE)   DL_OPCUA_ERR_t self_set_##TYPE##_is_abstract(                   \
                                opcua_##TYPE##_t* self, bool abstract){                         \
                            if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;          \
                            self->TYPE##_attributes->is_abstract = abstract;                    \
                            return DL_OPCUA_OK;}

#define SET_VALUE(TYPE)         DL_OPCUA_ERR_t self_set_##TYPE##_value(                         \
                                opcua_##TYPE##_t* self, void* value){                           \
                                    /*TODO*/                                                    \
                                return DL_OPCUA_OK;}                                            \

#define SET_DATA_TYPE(TYPE)     DL_OPCUA_ERR_t self_set_##TYPE##_data_type(                     \
                                opcua_##TYPE##_t* self, char* ID){                              \
                                    /*TODO*/                                                    \
                                return DL_OPCUA_OK;}                                            \

#define SET_VALUE_RANK(TYPE)    DL_OPCUA_ERR_t self_set_##TYPE##_value_rank(                    \
                                opcua_##TYPE##_t* self, int32_t vr){                            \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                self->TYPE##_attributes->value_rank = vr;                       \
                                return DL_OPCUA_OK;}                                            \

#define SET_ARRAY_DIM(TYPE)     DL_OPCUA_ERR_t self_set_##TYPE##_array_dimensions(              \
                                opcua_##TYPE##_t* self, uint32_t ad){                           \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                self->TYPE##_attributes->array_dimensions = ad;                 \
                                return DL_OPCUA_OK;}

#define SET_ACCESS_LEVEL(TYPE)  DL_OPCUA_ERR_t self_set_##TYPE##_access_level(                  \
                                opcua_##TYPE##_t* self, unsigned char al){                      \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                self->TYPE##_attributes->access_level = al;                     \
                                return DL_OPCUA_OK;}

#define SET_U_ACCESS_LEVEL(TYPE)    DL_OPCUA_ERR_t self_set_##TYPE##_user_access_level(         \
                                    opcua_##TYPE##_t* self, unsigned char ual){                 \
                                    if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;  \
                                    self->TYPE##_attributes->user_access_level = ual;           \
                                    return DL_OPCUA_OK;}

#define SET_MIN_SAMP_INT(TYPE)  DL_OPCUA_ERR_t self_set_##TYPE##_min_samp_interval(             \
                                opcua_variable_t* self, double msi){                            \
                                if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;    \
                                self->variable_attributes->min_samp_interval = msi;             \
                                return DL_OPCUA_OK;}                                            

#define SET_HISTORIZING(TYPE)   DL_OPCUA_ERR_t self_set_##TYPE##_historizing(                   \
                                opcua_variable_t* self, bool hist){                             \
                                if(self->variable_attributes == NULL) return DL_OPCUA_INVAL;    \
                                self->variable_attributes->historizing = hist;                  \
                                return DL_OPCUA_OK;}                        

#define SET_DEC_ID(TYPE)    DL_OPCUA_ERR_t self_set_##TYPE##_declaration_id(                    \
                                opcua_##TYPE##_t* self, int id){                                \
                            if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;          \
                            self->TYPE##_attributes->TYPE##_declaration_id = id;                \
                            return DL_OPCUA_OK;}                

#define SET_EXECUTABLE(TYPE)    DL_OPCUA_ERR_t self_set_##TYPE##_execuatble(                    \
                                    opcua_##TYPE##_t* self, bool val){                          \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                self->TYPE##_attributes->executable = val;                      \
                                return DL_OPCUA_OK;}                            

#define SET_U_EXECUTABLE(TYPE)  DL_OPCUA_ERR_t self_set_##TYPE##_user_execuatble(               \
                                    opcua_##TYPE##_t* self, bool val){                          \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                self->TYPE##_attributes->user_executable = val;                 \
                                return DL_OPCUA_OK;}                                          

#define SET_EVENT_NOTIFIER(TYPE)    DL_OPCUA_ERR_t self_set_##TYPE##_event_notifier(            \
                                    opcua_##TYPE##_t* self, unsigned char event_notifier){      \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                self->TYPE##_attributes->event_notifier = event_notifier;       \
                                return DL_OPCUA_OK;}


//## REFERENCE TYPE
#define SET_SYMMETRIC(TYPE)     DL_OPCUA_ERR_t self_set_##TYPE##_symmetric(                     \
                                opcua_##TYPE##_t* self, bool symmetric){                        \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                self->TYPE##_attributes->symmetric = symmetric;                 \
                                return DL_OPCUA_OK;}
#define SET_INVERSE_NAME(TYPE)  DL_OPCUA_ERR_t self_set_##TYPE##_inverse_name(                  \
                                    opcua_##TYPE##_t* self, char* inverse_name){                \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                if(inverse_name == NULL) return DL_OPCUA_INVAL;                 \
                                self->TYPE##_attributes->inverse_name =                         \
                                    (char*)realloc(self->TYPE##_attributes->inverse_name,       \
                                            sizeof(char) * (strlen(inverse_name) + 1));         \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_MEM;        \
                                strcpy(self->TYPE##_attributes->inverse_name, inverse_name);    \
                                return DL_OPCUA_OK;}

//## VIEW
#define SET_CONT_NO_LOOPS(TYPE) DL_OPCUA_ERR_t self_set_##TYPE##_contains_no_loops(             \
                                    opcua_##TYPE##_t* self, bool val){                          \
                                if(self->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;      \
                                self->TYPE##_attributes->contains_no_loops = val;               \
                                return DL_OPCUA_OK;}

//CREATE REFERENCES
#define CREATE_REFERENCES_FUNCTIONS(TYPE)                                                   \
DL_OPCUA_ERR_t self_create_node_##TYPE##_references(opcua_##TYPE##_t* object){              \
    DL_OPCUA_ERR_t ret = datalog_opcua_create_node_references(object, DL_OPC_##TYPE);       \
    return ret;}

//SELF ADD REF 
#define SELF_ADD_REF(TYPE)                                                                  \
DL_OPCUA_ERR_t self_add_ref_##TYPE(opcua_##TYPE##_t* object, opcua_reference_t* reference){ \
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;                                                       \
    ret = datalog_opcua_add_reference(reference, object, DL_OPC_##TYPE);                    \
    return DL_OPCUA_OK;}                                                                    \

#define CREATE_XML_NODE(TYPE, LABEL) \
DL_OPCUA_ERR_t datalog_opcua_create_node_##TYPE(opcua_##TYPE##_t* object){  \
    if(object == NULL) return DL_OPCUA_INVAL;                               \
    if(object->attributes == NULL) return DL_OPCUA_INVAL;                   \
    if(object->TYPE##_attributes == NULL) return DL_OPCUA_INVAL;            \
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;                                       \
    object->node = xmlNewChild(opcua_document->root_node,                   \
            NULL, BAD_CAST #LABEL, NULL);                                  \
    ret = datalog_opcua_create_node_attributes(object->node, object->display_name_node,\
            object->attributes);                                            \
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;                            \
    ret = datalog_opcua_create_node_##TYPE##_attributes(object);            \
    if(ret != DL_OPCUA_OK) return DL_OPCUA_ATTR;                            \
    return DL_OPCUA_OK;}                                                    \


opcua_document_t* opcua_document;

FOR_EACH_TYPE(BASE_NODE)

FOR_EACH_TYPE(CREATE_REFERENCES_FUNCTIONS)

FOR_EACH_TYPE(SELF_ADD_REF)

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

DL_OPCUA_ERR_t self_set_node_attributes_node_id_ns(opcua_node_attributes_t* self, int ns)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_ns(&self->node_id, ns);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_node_attributes_node_id_i(opcua_node_attributes_t* self, int i)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_i(&self->node_id, i);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_node_attributes_node_id_s(opcua_node_attributes_t* self, char* s)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_id_s(&self->node_id, s);
    
    return ret;
}

DL_OPCUA_ERR_t self_set_attributes_node_browse_name(opcua_node_attributes_t* self, 
        char* browse_name)
{
    DL_OPCUA_ERR_t ret = datalog_opcua_set_browse_name(self, browse_name);
    return ret;
}

DL_OPCUA_ERR_t self_set_attributes_node_display_name(opcua_node_attributes_t* self, 
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

DL_OPCUA_ERR_t self_set_attribute_user_write_mask(opcua_node_attributes_t* self, 
        int ual)
{
    self->user_write_mask = ual;
    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t self_set_attribute_write_mask(opcua_node_attributes_t* self, int al)
{
    self->write_mask = al;
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
    
    if(attributes->user_write_mask != 0){
        sprintf(buffer, "%d", attributes->user_write_mask);
        xmlNewProp(node, BAD_CAST "UserAccessLevel", BAD_CAST buffer);
    }
    
    if(attributes->write_mask != 0){
        sprintf(buffer, "%d", attributes->write_mask);
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

        datalog_opcua_add_reference(tmp_ref, tmp_obj_type, DL_OPC_object_type);
        i++;
   }
    ret = datalog_opcua_create_node_references(tmp_obj_type, DL_OPC_object_type);

    return DL_OPCUA_OK;
}

FOR_EACH_TYPE_W_LABEL(CREATE_XML_NODE)
//REFERENCE

#define TYPE_CAST_SWITCH_REF_HEAD(TYPE)  \
    case DL_OPC_##TYPE:                                             \
        ref_head = ((opcua_object_type_t*)object)->reference_head;  \
        break;

opcua_reference_t* datalog_opcua_find_reference_i(void* object,
        DL_OPCUA_TYPE_t type, int i)
{
    opcua_reference_t *ref_head;
    
    switch(type){
        FOR_EACH_TYPE(TYPE_CAST_SWITCH_REF_HEAD);
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
        FOR_EACH_TYPE(TYPE_CAST_SWITCH_REF_HEAD);
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
        FOR_EACH_TYPE(TYPE_CAST_SWITCH_REF_HEAD);
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
        FOR_EACH_TYPE(TYPE_CAST_SWITCH_REF_HEAD);
    default:
        break;
    }
    
    while(memcmp(&ref_head->id, ID, sizeof(opcua_node_id_t))){
        if(ref_head == NULL) return NULL;
        ref_head = ref_head->next;
    }

    return ref_head;
}

#define TYPE_CAST_SWITCH_REF_HEAD_DOUBLE(TYPE)      \
    case DL_OPC_##TYPE:                                             \
        ref_head = &(*(opcua_##TYPE##_t**)object)->reference_head;  \
        break;

DL_OPCUA_ERR_t datalog_opcua_free_reference_list(void** object, 
        DL_OPCUA_TYPE_t type)
{
    opcua_reference_t **ref_head, *next;

    switch(type){
        FOR_EACH_TYPE(TYPE_CAST_SWITCH_REF_HEAD_DOUBLE)
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

#define TYPE_CAST_SWITCH_REF_HEAD_ADD_REF(TYPE)     \
        case DL_OPC_##TYPE:                                             \
            if(((opcua_##TYPE##_t*)object)->reference_head == NULL){    \
                ((opcua_##TYPE##_t*)object)->reference_head = reference;\
                return DL_OPCUA_OK;                                     \
            }else{                                                      \
                opcua_reference_t* ref_head =                           \
                    ((opcua_##TYPE##_t*)object)->reference_head;        \
                while(ref_head->next != NULL) ref_head = ref_head->next;\
                ref_head->next = reference;                             \
            }                                                           \
            break;                

DL_OPCUA_ERR_t datalog_opcua_add_reference(opcua_reference_t* reference,
        void* object, DL_OPCUA_TYPE_t object_type)
{
    switch(object_type){
        FOR_EACH_TYPE(TYPE_CAST_SWITCH_REF_HEAD_ADD_REF)
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
#define XML_REFERENCE_SWITCH(TYPE)\
        case DL_OPC_##TYPE:                                                                  \
        if(((opcua_##TYPE##_t*)object)->reference_head == NULL) break;              \
        if(((opcua_##TYPE##_t*)object)->references_node == NULL)                    \
            ((opcua_##TYPE##_t*)object)->references_node                            \
                = xmlNewChild(((opcua_##TYPE##_t*)object)->node,                    \
                    NULL, BAD_CAST "References", NULL);                             \
        if(((opcua_##TYPE##_t*)object)->references_node != NULL){                   \
            if(((opcua_##TYPE##_t*)object)->reference_head != NULL){                \
                opcua_reference_t* ref_head =                                       \
                    ((opcua_##TYPE##_t*)object)->reference_head;                    \
                while(ref_head != NULL){                                            \
                    xmlNodePtr tmp_node =                                           \
                        xmlNewChild(((opcua_##TYPE##_t*)object)->references_node,   \
                        NULL, BAD_CAST "Reference", NULL);                          \
                    datalog_opcua_add_reference_attributes(tmp_node, ref_head);     \
                    ref_head = ref_head->next;                                      \
                }                                                                   \
            }else return DL_OPCUA_INVAL;                                            \
        }else return DL_OPCUA_INVAL;                                                \
            break;                                                                  

#define CREATE_XML_REFERENCES(TYPE)
DL_OPCUA_ERR_t datalog_opcua_create_node_references(void* object, DL_OPCUA_TYPE_t object_type){
    char buffer[32];
    switch(object_type){
        FOR_EACH_TYPE(XML_REFERENCE_SWITCH)
        default:
            break;
    }
    return DL_OPCUA_OK;
}

//FREE
DL_OPCUA_ERR_t datalog_opcua_free_attribiute(opcua_node_attributes_t* attribute)
{
    if(attribute->browse_name != NULL) free(attribute->browse_name);
    if(attribute->display_name != NULL) free(attribute->display_name);
    if(attribute->description != NULL) free(attribute->description);
    return DL_OPCUA_OK;
}

#define FREE_OBJECT(TYPE) \
DL_OPCUA_ERR_t datalog_opcua_free_##TYPE(opcua_##TYPE##_t** TYPE){         \
    DL_OPCUA_ERR_t ret = DL_OPCUA_OK;                                       \
    ret = datalog_opcua_free_reference_list((void**)TYPE, DL_OPC_##TYPE);   \
    free((*TYPE)->TYPE##_attributes);                                  \
    free((*TYPE)->attributes);                                              \
    if((*TYPE)->attributes != NULL)                                         \
        datalog_opcua_free_attribiute((*TYPE)->attributes);                 \
    free(*TYPE);                                                            \
    return DL_OPCUA_OK;}                                                    \

FOR_EACH_TYPE(FREE_OBJECT)

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
    ret->set_node_id_ns = &self_set_node_attributes_node_id_ns;
    ret->set_node_id_i = &self_set_node_attributes_node_id_i;
    ret->set_node_id_s = &self_set_node_attributes_node_id_s;
    ret->set_user_write_mask = &self_set_attribute_user_write_mask;
    ret->set_write_mask = &self_set_attribute_write_mask;
    ret->set_browse_name = &self_set_attributes_node_browse_name;
    ret->set_display_name = &self_set_attributes_node_display_name;
    ret->set_description = &self_set_attribute_description;

    return ret;
}

#define CREATE_OBJECT_ATTRIBUTES(TYPE)                                          \
opcua_##TYPE##_attributes_t* datalog_opcua_create_##TYPE##_attributes(void){    \
    opcua_##TYPE##_attributes_t* ret = (opcua_##TYPE##_attributes_t*)           \
        calloc(1, sizeof(opcua_##TYPE##_attributes_t));                         \
    if(ret == NULL) return NULL;                                                \
    return ret;}                                                                \

FOR_EACH_TYPE(CREATE_OBJECT_ATTRIBUTES)

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

#define object_type_CREATE_OBJECT   \
    ret->object_type_attributes->is_abstract = -1;                  \
    ret->set_is_abstract = &self_set_object_type_is_abstract;         

#define method_CREATE_OBJECT        \
    ret->set_executable = &self_set_method_execuatble;              \
    ret->set_user_executable = &self_set_method_user_execuatble;

#define variable_CREATE_OBJECT      \
    ret->set_value = &self_set_variable_value;                      \
    ret->set_data_type = &self_set_variable_data_type;              \
    ret->set_value_rank = &self_set_variable_value_rank;            \
    ret->set_array_dimensions = &self_set_variable_array_dimensions;\
    ret->set_access_level = &self_set_variable_access_level;        \
    ret->set_user_access_level = &self_set_variable_user_access_level;\
    ret->set_min_samp_interval = &self_set_variable_min_samp_interval;\
    ret->set_historizing = &self_set_variable_historizing;          \

#define object_CREATE_OBJECT        \
    ret->object_attributes->event_notifier = -1;                    \
    ret->set_event_notifier = &self_set_object_event_notifier;

#define variable_type_CREATE_OBJECT \
    ret->set_value = &self_set_variable_type_value;                 \
    ret->set_data_type = &self_set_variable_type_data_type;         \
    ret->set_array_dimensions = &self_set_variable_type_array_dimensions;\
    ret->set_value_rank = &self_set_variable_type_value_rank;       \
    ret->set_is_abstract = &self_set_variable_type_is_abstract;     \
    ret->variable_type_attributes->is_abstract = -1;

#define reference_type_CREATE_OBJECT    \
    ret->set_is_abstract = &self_set_reference_type_is_abstract;    \
    ret->set_symmetric = &self_set_reference_type_symmetric;        \
    ret->set_inverse_name = &self_set_reference_type_inverse_name;  \
    ret->reference_type_attributes->is_abstract = -1;               \
    ret->reference_type_attributes->symmetric = -1;

#define data_type_CREATE_OBJECT     \
    ret->set_is_abstract = &self_set_data_type_is_abstract;         \
    ret->data_type_attributes->is_abstract = -1;

#define view_CREATE_OBJECT          \
    ret->set_contains_no_loops = &self_set_view_contains_no_loops;  \
    ret->set_event_notifier = &self_set_view_event_notifier;        \
    ret->view_attributes->contains_no_loops = -1;

#define CREATE_OBJECT(TYPE) \
opcua_##TYPE##_t* datalog_opcua_create_##TYPE(void){                                \
    opcua_##TYPE##_t* ret = (opcua_##TYPE##_t*)calloc(1, sizeof(opcua_##TYPE##_t)); \
    ret->attributes = datalog_opcua_create_attributes();                            \
    if(ret->attributes == NULL) return NULL;                                        \
    ret->TYPE##_attributes = datalog_opcua_create_##TYPE##_attributes();            \
    if(ret->TYPE##_attributes == NULL) return NULL;                                 \
    ret->set_parent_id_ns = &self_set_##TYPE##_parent_node_id_ns;                   \
    ret->set_parent_id_i = &self_set_##TYPE##_parent_node_id_i;                     \
    ret->set_parent_id_s = &self_set_##TYPE##_parent_node_id_s;                     \
    ret->set_node_id_ns = &self_set_##TYPE##_node_id_ns;                            \
    ret->set_node_id_i = &self_set_##TYPE##_node_id_i;                              \
    ret->set_node_id_s = &self_set_##TYPE##_node_id_s;                              \
    ret->set_browse_name = &self_set_##TYPE##_browse_name;                          \
    ret->set_display_name = &self_set_##TYPE##_display_name;                        \
    ret->set_description = &self_set_##TYPE##_description;                          \
    ret->set_user_write_mask = &self_set_##TYPE##_user_write_mask;                  \
    ret->set_write_mask = &self_set_##TYPE##_write_mask;                            \
    ret->create_references = &self_create_node_##TYPE##_references;                 \
    ret->add_reference = &self_add_ref_##TYPE;                                      \
    ret->create_node = &datalog_opcua_create_node_##TYPE;                           \
    ret->free_##TYPE = &datalog_opcua_free_##TYPE;                                  \
    TYPE##_CREATE_OBJECT                                                            \
    return ret;}

FOR_EACH_TYPE(CREATE_OBJECT)


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

DL_OPCUA_ERR_t datalog_opcua_create_attach_reference(
        opcua_reference_t* reference_head, char* parent_ID, char* target_ID,
        char* type, bool is_forward)
{
    opcua_reference_t* ret = datalog_opcua_create_reference();
    if(ret == NULL) return DL_OPCUA_MEM;
    ret->set_id_i(ret, datalog_opcua_parse_id_i(parent_ID));
    ret->set_id_ns(ret, datalog_opcua_parse_id_ns(parent_ID));
    ret->set_id_s(ret, datalog_opcua_parse_id_s(parent_ID));
    if(type != NULL) ret->set_type(ret, type);
    ret->set_is_forward(ret, is_forward);
    
    opcua_reference_t* ref_head = reference_head;
    while(reference_head->next != NULL) 
        reference_head = reference_head->next;

    reference_head->next = ret;

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_attach_reference_head(
        opcua_reference_t** ref_head, char* parent_ID, char* target_ID, 
        char* type, bool is_forward)
{
    (*ref_head) = (opcua_reference_t*)calloc(1, 
            sizeof(opcua_reference_t));
    if((*ref_head) == NULL) return DL_OPCUA_MEM;

    self_set_reference_id_i((*ref_head), datalog_opcua_parse_id_i(parent_ID));
    self_set_reference_id_ns((*ref_head), datalog_opcua_parse_id_ns(parent_ID));
    self_set_reference_id_s((*ref_head), datalog_opcua_parse_id_s(parent_ID));

    if(type != NULL) 
    self_set_reference_type((*ref_head), type);
    self_set_reference_is_forward((*ref_head), is_forward);

    return DL_OPCUA_OK;
}

//ID PARSING
int datalog_opcua_parse_id_i(char* id)
{
    char input[strlen(id)];
    strcpy(input, id);
    char* token = strtok(input, "\";");
    while(token){
        if(token[0] == 'i') return atoi(token + 2);
        token = strtok(NULL, "\";");
    }
    return 0;
}

int datalog_opcua_parse_id_ns(char* id)
{
    char input[strlen(id)];
    strcpy(input, id);
    char* token = strtok(input, "\";");
    while(token){
        if(token[0] == 'n') return atoi(token + 3);
        token = strtok(NULL, "\";");
    }
    return 0;
}

char* datalog_opcua_parse_id_s(char* id)
{
    char input[strlen(id)];
    strcpy(input, id);
    char* token = strtok(input, "\";");
    while(token){
        if(token[0] == 's'){
            char* ret = (char*)malloc(sizeof(char) * 
                    (strlen(token + 2) + 1));
            if(ret == NULL) return NULL;
            strcpy(ret, token + 2);
            return ret;
        }
        token = strtok(NULL, "\";");
    }
    return 0;
}

//RUNTIME
void datalog_opcua_runtime(void)
{
    printf("opcua runtime\n");

    DL_OPCUA_ERR_t ret  = datalog_opcua_init_doc();

    if(ret != DL_OPCUA_OK) return;

//TEST CODE
//TEST CODE END

    datalog_opcua_save_deinit_doc();
}
