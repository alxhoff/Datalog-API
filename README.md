# Datalog API

[![Build Status](https://travis-ci.com/alxhoff/Datalog-API.svg?branch=master)](https://travis-ci.com/alxhoff/Datalog-API)

_windows users you are on your own_

## Introduction
The original implementation of the LUA/C library is not the easiest to work with and does not lead to logical or easy to follow code. I have quickly thrown this API together to make the datalog coding style similar to that of the Python implementation.

## Prerequisites

### Ubuntu
``` bash
sudo apt-get install lua5.2 cmake libxml2-dev
```
### Arch

``` bash
sudo pacman -S cmake libxml2 lua52
```
### Submodules

The cJSON library is a submodule

``` bash
git submodule update --init
```

## Building

```bash
mkdir build && cd build
cmake ..
make
```

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
+ Check safe rules, "A clause is safe if every variable in its head occurs in some literal in its body"
+ Rule retraction
+ ~~Asserting rules list, currently seg faults~~
+ Error checking on literal types
+ ~~Check that all methods are implemented for all objects, both with and without structs.~~
+ Lots more stuff I am to yet to discover
+ Check file existance

### Known bugs to fix

+ Same rule twice seg faults
+ Tilde on last position of rule seg faults

# Objects

The API revolves around a struct objects to enable a more logical way or representing datalog commands rather than the push and pop confusion found in the library.

Literals are represented by the object __datalog_literal_t__, queries by __datalog_query_t__ and clauses by __datalog_clause_t__. I may of missed a few methods to be implemented but I should get these done as I actually use this API for other code.

## Literal objects

The parser represents literals using the __dl_parser_literal_t__ object that stores the predicate and arguments for the literal.

## Fact objects

A fact in Datalog is a clause without a body, aka it's a standard literal. Facts are essentially a literal stored in a container allowing them to be used as elements of a linked list as well as storing a reference to the XML node that corresponds to the fact. The parser will scan the entire document for facts, creating a linked list. This linked list is then traversed and each XML fact node is walked and processed so that each fact is represented by a __dl_parser_fact_t__ object which contains the fact's literal.

The facts in the XML tree can be found by calling __dl_parser_mappings__ as this will in turn walk the XML tree and call __dl_parser_add_fact__ on all found facts. Found facts are then processed using __dl_parser_process_fact__. After processing the found facts can be found pointed to by the linked list head pointer __dl_parser_fact_t* facts_head__ found within the __dl_parser_doc_t__ object.

## Rule objects

In Datalog rules are clauses that contain a body, the body being a potentially infinite length array of literals. In the XML parser rules are represented similarly to facts, using the __dl_parser_rule_t__ object, with the exception that the rules object represents the body of the rule by an array of literal pointers. This list is dynamically allocated to allow for potentially infinite length rules. Parsing of rules happens in the same fashion as facts, in that a call to __dl_parser_mappings__ will walk the XML tree and call __dl_parser_add_rule__ on any found rules. These are then stores in a similar linked list fashion and processed through calls to __dl_parser_process_rule__ whilst iterating over the linked list. The linked list of processed rules can be found using the head pointer __dl_parser_rule_t* rules_head__ found in the __dl_parser_doc_t__ object.

# XML Parser (very Beta)

The XML parser is designed to be used to be able parse XML files to a datalog program to load rules and facts into the datalog database. The parser is built around libxml2 and built into a shared library.

## Using the parser

Parser use should pretty much only require the user to call

```c
dl_parser_doc_t* dl_doc = dl_parser_init(filename);
```
followed by
```c
dl_parser_mappings(dl_doc);
```
but one can also print any number of the objects in the library though the number of print functions. Document meta-data can also be parsed by calling __dl_parser_metadata__ which will populate a __dl_parser_metadata_t__ object stored in the main __dl_parser_doc_t__ object.

See included example for further usage.

### Initialisation and Deinitialisation

The parser must be started by calling the __dl_parser_init__ function, specifying the file to be opened. This will initialize the document object used by the parser. Calling __dl_parser_deinit__ all objects used by the parser are free'd and the parser deinit'd.

### Parsing a Document

The function __dl_parser_runtime__ can be called to completely pass a document, returning a __dl_parser_return_doc_t__ object that contains a linked list to all the parsed rules, a linked list to all the parsed facts found within the document as well as populating the __dl_parser_metadata_t__ object within the return doc object.

One can also parse the various types of data separately by running either: __dl_parser_metadata__ or __dl_parser_mappings__ to parse only the meta-data or the rules and facts respectively.

### Asserting a Parsed Document

The __dl_parser_return_doc_t__ object can have both it's rules list and facts lists asserted by calling the functions __dl_assert_fact_list__ and __dl_assert_rule_list__ respectively.

## XML Style Guide

The XML documents to be parsed must adhere to a strict style guide. I would like to say I have been scrupulous in implementing error checking but as this is more a time>quality implementation, you will most likely crash your program by parsing an invalid XML document. More through error checking is on the TODO.

### XML Schema

A XML Schema can be found in the API directory. An XML file can be validated using ```xmllint```

Example on the file query_test.xml,w:
```bash
xmllint -schema datalog.xsd query_test.xml --noout
```
Remove the ```--noout``` option for a verbose output.

### Tags

#### Document

All Datalog XML documents must have a root tag __<datalog>__

#### Metadata

+ `<name>`: The name of the device
+ `<type>`: The type of device
+ `<manufacturer>`: Manufacturer of the device
+ `<contact>`: Contact regarding the device, probably from supplier
+ `<model>`: Device model
+ `<serial>`: Device serial number
+ `<year>`: Year the device was produced

#### Mappings

The mappings tags houses the portion of the document where all rules and facts are to be represented.

#### Facts

A fact is represented very similarly to a rule, except it does not contain a `<body>` tag. The `<head>` tag contains one literal.

#### Rules

A rule is represented by two tags, a `<head>` tag and a `<body>` tag. The head can only contain one literal while the body can contain a theoretically infinite number of literals.

#### Head

`<head>` tags are used to represent the head of a clause and contain one literal.

#### Body

`<body>` tags are used to represent the body of a clause can can contain a theoretically infinite number of literals.

#### Literal

A literal is used to display Datalog's most rudimentary data type. One literal is comprised of a predicate and the corresponding terms (minimum 1, infinite maximum).
A literal such as __test(foo, BAR)__. would be represented using the following
```xml
<literal>
<predicate>test</predicate>
<terms>
<constant>foo</constant> <!-- constant as the term has a lowercase letter -->
<variable>BAR</variable> <!-- Variable as the term has a capital letter -->
</terms>
</literal>
```
+ `<predicate>`: The literal's predicated
+ `<terms>`: Terms can have two types, the tags can be interchanged to modify the type of literal you wish to express
+ `<constant>`: represents a constant term
+ `<variable>`: represents a variable term

### Example XML

```xml
<?xml version="1.0" encoding="UTF-8"?>
<datalog>
<metadata>
<description>EDD to NAMUR mappings</description>
<author>alex</author>
<device>
<name>test device</name>
<type>test type</type>
<manufacturer>siemens</manufacturer>
<contact>alxhoff@gmail.com</contact>
<model>test model</model>
<serial>test serial</serial>
<year>2017</year>
</device>
</metadata>
<mappings>
<fact>
<head>
<literal>
<predicate>test</predicate>
<terms>
<constant>hello</constant>
<constant>world</constant>
<constant>how</constant>
<constant>are</constant>
<constant>you</constant>
</terms>
</literal>
</head>
</fact>
<rule>
<head>
<literal>
<predicate>test</predicate>
<terms>
<constant>this</constant>
<variable>is</variable>
</terms>
</literal>
</head>
<body>
<literal>
<predicate>foo</predicate>
<terms>
<constant>a</constant>
<variable>rule</variable>
</terms>
</literal>
<literal>
<predicate>test</predicate>
<terms>
<constant>with two</constant>
<constant>literals</constant>
</terms>
</literal>
</body>
</rule>
</mappings>
</datalog>

```

# Command Line Interface (CLI)
I have made a small set of functions that work on a loop to read commands from the native CLI to the Datalog's CLI emulator. The CLI can be used to interact with and previous Datalog happenings run within the same program as it shares the same global database as the other API modules.
## Using the CLI
The CLI accepts 4 types of input: normal Datalog statements, "help" to display help, "clear" to clear the CLI and "exit" to exit the CLI function loop.
Please see the inbuilt help function for information on how to express Datalog statements.
The CLI can be embedded into a program via it's main runtime function __datalog_command_line_run__. The function runs on a goto loop and will run until the program is terminated or the "exit" command is given.
