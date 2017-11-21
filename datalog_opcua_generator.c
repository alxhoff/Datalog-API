#include <stdio.h>
#include <string.h>

#include "datalog_opcua_generator.h"

opcua_document_t* opcua_document;

DL_OPCUA_ERR_t datalog_opcua_set_display_name(void* object, DL_OPCUA_TYPE_t type, 
        char* display_name)
{
    switch(type){
    case DL_OPC_VARIABLE:
        ((opcua_variable_t*)object)->display_name = 
            (char*)realloc(((opcua_variable_t*)object)->display_name, 
                    sizeof(char) * (strlen(display_name) + 1));
        if(((opcua_variable_t*)object)->display_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_variable_t*)object)->display_name, display_name);
        break;
    case DL_OPC_METHOD:
        ((opcua_method_t*)object)->display_name = 
            (char*)realloc(((opcua_method_t*)object)->display_name, 
                    sizeof(char) * (strlen(display_name) + 1));
        if(((opcua_method_t*)object)->display_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_method_t*)object)->display_name, display_name);
        break;
    case DL_OPC_OBJ:
        ((opcua_object_t*)object)->display_name = 
            (char*)realloc(((opcua_object_t*)object)->display_name, 
                    sizeof(char) * (strlen(display_name) + 1));
        if(((opcua_object_t*)object)->display_name == NULL) return DL_OPCUA_MEM;
        strcpy(((opcua_object_t*)object)->display_name, display_name);
        break;
    default:
        break;
    }
    return DL_OPCUA_OK;
}

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

    //create doc
    opcua_document = datalog_opcua_create_document(XML_FILENAME, 
            XML_FILE_VERSION); 

    //DTD
    xmlCreateIntSubset(opcua_document->document, BAD_CAST "UANodeSet", 
            NULL, BAD_CAST "dtdIsHere");

    //create namespace Uris
    tmp_node = xmlNewChild(opcua_document->root_node, NULL, 
            BAD_CAST "NamespaceUris", NULL);
    xmlNewChild(tmp_node, NULL, BAD_CAST "Uri", BAD_CAST NAMESPACE_URI);

    //create aliases
    tmp_parent = xmlNewChild(opcua_document->root_node, NULL, BAD_CAST "Aliases", NULL);
    
    while(alias_array[i].integer != 0){
        sprintf(buff, "i=%d", alias_array[i].integer);
        tmp_child = xmlNewChild(tmp_parent, NULL, BAD_CAST "Alias", BAD_CAST buff);
        xmlNewProp(tmp_child, BAD_CAST "Alias",  BAD_CAST alias_array[i].name);
        i++;
    }

    //extension
    tmp_parent = xmlNewChild(opcua_document->root_node, NULL, 
            BAD_CAST "Extensions", NULL);
    tmp_parent = xmlNewChild(tmp_parent, NULL, BAD_CAST "Extension", NULL);
    tmp_child = xmlNewChild(tmp_parent, NULL, BAD_CAST "ModelInfo", NULL);
    xmlNewProp(tmp_child, BAD_CAST "Tool", BAD_CAST "UaModeler");
    xmlNewProp(tmp_child, BAD_CAST "Hash", BAD_CAST "T9MjgfInUChe45aJYm9rKw==");
    xmlNewProp(tmp_child, BAD_CAST "Version", BAD_CAST "1.4.0");

    return DL_OPCUA_OK;
}
//CREATE OBJECT NODES
DL_OPCUA_ERR_t datalog_opcua_create_node_attributes(xmlNodePtr parent_node, 
        opcua_node_attributes_t* attributes)
{
    char buffer[32];
    //create general attributes
    if(attributes->browse_name != NULL)
        xmlNewProp(parent_node, BAD_CAST "BrowseName",
                BAD_CAST attributes->browse_name);
    if((attributes->node_id.i != 0) && (attributes->node_id.ns != 0)){
        sprintf(buffer, "ns=%d;i=%d", attributes->node_id.ns, attributes->node_id.i);
        xmlNewProp(parent_node, BAD_CAST "NodeId", BAD_CAST buffer); 
    }
    if((attributes->parent_node_id.i != 0) && (attributes->parent_node_id.ns != 0)){
        sprintf(buffer, "ns=%d;i=%d", attributes->parent_node_id.ns,
                attributes->parent_node_id.i);
        xmlNewProp(parent_node, BAD_CAST "ParentNodeId", BAD_CAST buffer);
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

    return DL_OPCUA_OK;
}

DL_OPCUA_ERR_t datalog_opcua_create_node_object_attributes(opcua_object_t* object)
{
    char buffer[32];

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

    return DL_OPCUA_OK;
}
//

DL_OPCUA_ERR_t datalog_opcua_set_reference_type(opcua_reference_t* reference,
        char* type)
{
    reference->type = (char*)realloc(reference->type, 
        sizeof(char) * (strlen(type) + 1));
    if(reference->type == NULL) return DL_OPCUA_MEM;

    strcpy(reference->type, type);

    return DL_OPCUA_OK;
}

opcua_reference_t* datalog_opcua_create_reference(void)
{
    opcua_reference_t* ret = (opcua_reference_t*)
        calloc(1, sizeof(opcua_reference_t));
    if(ret == NULL) return NULL;

    return ret;
}

opcua_node_attributes_t* datalog_opcua_create_attributes(void)
{
    opcua_node_attributes_t* ret = (opcua_node_attributes_t*)
        calloc(1, sizeof(opcua_node_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

DL_OPCUA_ERR_t datalog_opcua_set_attribute_browse_name(opcua_node_attributes_t* attributes, 
        char* browse_name)
{
    attributes->browse_name = (char*)malloc(sizeof(char) *  strlen(browse_name));
    if(attributes->browse_name == NULL) return DL_OPCUA_MEM;
    strcpy(attributes->browse_name, browse_name);
    return DL_OPCUA_OK;
}

opcua_method_attributes_t* datalog_opcua_create_method_attributes(void)
{
    opcua_method_attributes_t* ret = (opcua_method_attributes_t*)
        calloc(1, sizeof(opcua_method_attributes_t));
    if(ret == NULL) return NULL;
    return ret;
}

DL_OPCUA_ERR_t datalog_opcua_set_variable_data_type(opcua_variable_attributes_t* var,
        char* data_type)
{
    var->data_type = 
        (char*)realloc(var->data_type, sizeof(char) * (strlen(data_type) + 1));
    if(var->data_type == NULL) return DL_OPCUA_MEM;
    strcpy(var->data_type, data_type);

    return DL_OPCUA_OK;
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

opcua_method_t* datalog_opcua_create_method(void)
{
    opcua_method_t* ret = (opcua_method_t*)calloc(1, sizeof(opcua_method_t));
    if(ret == NULL) return NULL;

    ret->attributes = datalog_opcua_create_attributes();
    if(ret->attributes == NULL) return NULL;

    ret->method_attributes = datalog_opcua_create_method_attributes();
    if(ret->method_attributes == NULL) return NULL;

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

    return ret;
}

void datalog_opcua_runtime(void)
{
    printf("opcua runtime\n");

    DL_OPCUA_ERR_t ret  = datalog_opcua_init_doc();

    if(ret != DL_OPCUA_OK) return;

//TEST CODE
    opcua_method_t* test_method = datalog_opcua_create_method();

    ret = datalog_opcua_set_display_name(test_method, DL_OPC_METHOD, 
            "testDisplayName");

    if(ret != DL_OPCUA_OK) return;

    ret = datalog_opcua_create_node_method(test_method);

    if(ret != DL_OPCUA_OK) return;
//TEST CODE END

    datalog_opcua_save_deinit_doc();
}

