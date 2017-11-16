# Datalog API

_windows users you are on your own_

## Introduction
The original implementation of the LUA/C library is not the easiest to work with and does not lead to logical or easy to follow code. I have quickly thrown this API together to make the datalog coding style similar to that of the Python implementation. 

## Prerequisites 
The LUA/C library can be found [here](http://datalog.sourceforge.net) and must be downloaded and placed into the libs folder such that the file tree looks like this  
```
  |--- Datalog-API  
        |  
        |---bin  
        |---build  
        |---doc  
        |  
        |---libs  
             |---libdatalog <---rename downloaded folder to this  
                 |---source files  
                 |---lua  
                      |---lua source files  
```
## Building
I have included a demo main.c as well as a demo CMake that will build the API as a shared library that can then be linked into an exsisting project.  

### How to build
It's pretty tough....  
cd into the root director and create a build dir, or don't, it'll be your mess.  

```bash
 cd Datalog-API
 mkdir build
 cd build
```
 Generate CMake junk __or__ run ccmake to configure debugging options
```bash
 cmake ..
```
__or__
```bash
 ccmake ..
```
and finally make
```bash
 make
```
The executable can be found in the bin subdirectory in the root dir.  

Library objects in the build subdirectory.

## Documentation

To generate the documentation run the command

```bash
doxygen
```
from the doc folder. Then navigate to ./html/index.html 

## Work in progress
I wrote this quickly and dirtily, so excuse the mess.  

### To-Do  
+ ~~Fix demo project~~
+ ~~Integrate parser into API~~ Test parser wrapping
+ ~~Clauses with bodies~~
+ Error checking on literal types
+ Check that all methods are implemented for all objects, both with and without structs.
+ Lots more stuff I am to yet to discover

## Objects 
The API revolves around a struct objects to enable a more logical way or representing datalog commands rather than the push and pop confusion found in the library.  

Literals are represented by the object __datalog_literal_t__, queries by __datalog_query_t__ and clauses by __datalog_clause_t__. I may of missed a few methods to be implemented but I should get these done as I actually use this API for other code. 

# XML Parser (very Beta)

 The XML parser is designed to be used to be able parse XML files to a datalog program to load rules and facts into the datalog database. The parser is built around libxml2 and built into a shared library.

## Using the parser
### Initialisation and Deinitialisation
The parser must be started by calling the dl_parser_init function, specifying the file to be opened. This will initialise the document object used by the parser. Calling dl_parser_deinit all objects used by the parser are free'd and the parser deinit'd.

### Parsing a Document
The function dl_parser_runtime can be called to completley pass a document, returning a dl_parser_return_doc_t object that contains a linked list to all the parsed rules, a linked list to all the parsed facts found within the document as well as populating the dl_parser_metadata_t object within the return doc object.   
One can also parse the various types of data seperatley by running either: dl_parser_metadata or dl_parser_mappings to parse only the metadata or the rules and facts respectivley. 

### Asserting a Parsed Document
The dl_parser_return_doc_t object can have both it's rules list and facts lists asserted by calling the functions dl_assert_fact_list and dl_assert_rule_list respectivley.

## XML Style Guide

The XML documents to be parsed must adhere to a strict style guide. I would like to say I have been scrupulous in implementing error checking but as this is more a time>quality implementation, you will most likley crash your program by parsing an invalid XML document. More throrogh error checking is on the TODO.

### Tags

#### Document

All datalog XML documents must have a root tag <datalog>

#### Metadata

+ <name>: The name of the device
+ <type>: The type of device
+ <manufacturer>: Manufacturer of the device
+ <contact>: Contact reguarding the device, probably from supplier
+ <model>: Device model
+ <serial>: Device serial number
+ <year>: Year the device was produced

#### Mappings

The mappings tags houses the portion of the document where all rules and facts are to be represented.

#### Facts

A fact is represented very similarly to a rule, except it does not contain a `<body>` tag. The `<head>` tag contains one literal.

#### Rules

A rule is represented by two tags, a `<head>` tag and a `<body>` tag. The head can only contain one literal while the body can contain a theorerically infinite number of literals.

#### Head

`<head>` tags are used to represent the head of a clause and contain one literal.

#### Body

 `<body>` tags are used to represent the body of a calsue can can contain a theoretically infiinite number of literals.

#### Literal

A literal is used to display datalog's most rudementary data type. One literal is comprised of a predicate and the corresponsing terms.   
A literal such as test(foo, BAR). would be represented using the following
```xml
<literal>
    <predicate>test</predicate>
    <terms>
        <constant>foo</constant> <-- constant as the term has a lowercase letter
        <variabel>BAR</variable> <-- Variable as the term has a capital letter
    </terms>
</literal>
```
+ <predicate>: The literal's predicated
+ <terms>: Terms can have two types, the tags can be interchanged to modify the type of literal you wish to express
    + <constant>: represents a constant term
    + <variable>: represents a variable term

### Example XML

```xml

```

# Command Line Interface (CLI)
I have made a small set of funtions that work on a loop to read commands from the native CLI to the Datalog's CLI emulator. The CLI can be used to interact with and previous datalog happenings run within the same program as it shares the same global database as the other API modules. 
## Using the CLI
The CLI accepts 4 types of input: normal datalog statements, "help" to display help, "clear" to clear the CLI and "exit" to exit the CLI function loop.   
Please see the inbuilt help function for information on how to express datalog statements.   
The CLI can be embedded into a program via it's main runtime fuction datalog_command_line_run. The function runs on a goto loop and will run until the program is terminated or the "exit" command is given.

# Literal objects
The parser represents literals using the dl_parser_literal_t object that stores the predicate and arguments for the literal. Literals are probably not used directly in the parser.

# Fact objects
 A fact in Datalog is a clause without a body, aka it's a standard literal. Facts are essentailly a literal stored in a container allowing them to be used as elements of a linked list as well as storing a reference to the XML node that corresponds to the fact. The parser will scan the entire document for facts, creating a linked list. This linked list is then traversed and each XML fact node is walked and processed so that each fact is represented by a dl_parser_fact_t object which contains the fact's literal.   

 The facts in the XML tree can be found by calling dl_parser_mappings as this will in turn walk the XML tree and call dl_parser_add_fact on all found facts. Found facts are then processed using dl_parser_process_fact. After processing the found facts can be found pointed to by the linked list head pointer dl_parser_fact_t* facts_head found within the dl_parser_doc_t object.

# Rule objects
 In Datalog rules are clauses that contain a body, the body being a potentially infinite length array of literals. In the XML parser rules are represented similarly to facts, using the dl_parser_rule_t object, with the exception that the rules object represents the body of the rule by an array of literal pointers. This list is dynamically allocated to allow for potentially infinite length rules. Parsing of rules happens in the same fashion as facts, in that a call to dl_parser_mappings will walk the XML tree and call dl_parser_add_rule on any found rules. These are then stores in a similar linked list fashion and processed through calls to dl_parser_process_rule whilst iterating over the linked list. The linked list of processed rules can be found using the head pointer dl_parser_rule_t* rules_head found in the dl_parser_doc_t object.

 ## Parser use
 Parser use should pretty much only require the user to call 

 ```c
 dl_parser_doc_t* dl_doc = dl_parser_init(filename);
 ```
 followed by   
 ```c
 dl_parser_mappings(dl_doc);
```

but one can also print any number of the objects in the library though the number of print functions. Document metadata can also be parsed by calling dl_parser_metadata which will populat a dl_parser_metadata_t object stored in the main dl_parser_doc_t object.   
 See included example for furthur usage.
