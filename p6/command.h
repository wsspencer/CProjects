/**
  @file command.h
  @author

  Parsing code and representation for commands in the language.
*/

#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdio.h>
#include "label.h"

/** It's weird, but you can give a short name to a struct before you define it.
    Then, you can use the short name in the definition. */
typedef struct CommandStruct Command;

/** Representation for the Command interface, a superclass for all
    types of commands.  Subclasses will start out just like this
    struct, with the same fields in the same order.  Extra fields used
    by the subclass will be added after these common fields.
*/
struct CommandStruct {
  /** Pointer to a function to execute this command.
      @param cmd The command to be executed.
      @param labelMap Map for where all the labels are.
      @param pc Index of the command being run (program counter), so
      this command can return the index of the next command.
      @return Index of the next instruction to run in the program (new
      program counter).  Normally, this will just be one greater than
      the pc input, but on a branch, the program could jump to
      anywhere.
   */
  int (*execute)( Command *cmd, LabelMap *labelMap, int pc );
  void (*destroy)( Command *cmd );

  /** Source file line containing this command. Used for reporting errors. */
  int line;
};

/** Parse the next command from the given input stream and return a
    pointer to Command object to represent it.
    @param fp stream to parse the command from.
    @return the Command object constructed from the input.
*/
Command *parseCommand( char *cmdName, FILE *fp );

#endif
