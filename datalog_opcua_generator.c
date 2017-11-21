#include <stdio.h>
#include <string.h>

#include "datalog_opcua_generator.h"

void datalog_opcua_save_deinit_doc(char* filename, xmlDocPtr doc)
{
    xmlSaveFormatFileEnc( filename, doc, "UTF-8", 1);

    xmlFree(doc);

    xmlCleanupParser();

    xmlMemoryDump();
}

xmlDocPtr datalog_opcua_init_doc(void)
{
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL, tmp_node = NULL,
               tmp_parent = NULL, tmp_child = NULL;
    char buff[256];
    int i = 0;

    //create doc
    doc = xmlNewDoc(BAD_CAST "1.0");
    //root node
    root_node = xmlNewNode(NULL, BAD_CAST "UANodeSet");
    xmlDocSetRootElement(doc, root_node);

    //DTD
    xmlCreateIntSubset(doc, BAD_CAST "UANodeSet", NULL, BAD_CAST "dtdhere");

    //create namespace Uris
    tmp_node = xmlNewChild(root_node, NULL, BAD_CAST "NamespaceUris", NULL);
    xmlNewChild(tmp_node, NULL, BAD_CAST "Uri", 
            BAD_CAST "urn:UnifiedAutomation:CppDemoServer:BuildingAutomation");

    //create aliases
    tmp_parent = xmlNewChild(root_node, NULL, BAD_CAST "Aliases", NULL);
    
    while(alias_array[i].integer != 0){
        sprintf(buff, "i=%d", alias_array[i].integer);
        printf("Current pair: %s,%d\n", alias_array[i].name, alias_array[i].integer);
        tmp_child = xmlNewChild(tmp_parent, NULL, BAD_CAST "Alias", BAD_CAST buff);
        xmlNewProp(tmp_child, BAD_CAST "Alias",  BAD_CAST alias_array[i].name);
        i++;
    }

    return doc;
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

    xmlDocPtr doc = datalog_opcua_init_doc();

    if(doc == NULL) return;

    datalog_opcua_save_deinit_doc("test_doc.xml", doc);
}

