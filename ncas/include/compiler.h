/*
  Created by nullifiedcat on 04.03.18.
*/


#pragma once

/*
 *  Compiler
 */

#include <stdint.h>
#include <stddef.h>

#include "instruction.h"
#include "registers.h"
#include "model.h"

struct parsed_operand_s
{
    enum operand_mode mode;
    /*
     * Value:
     *  0: immediate value
     *  1: address
     *  2: offset
     *  3: -
     *  4: -
     *  5: offset
     *  6: -
     *  7: -
     */
    uint32_t value;
    /*
     * Only used in mode 6
     */
    uint16_t factor;
    /*
     * Modes 3..6
     */
    enum register_name rname1;
    /*
     * Mode 6 only
     */
    enum register_name rname2;
    /*
     * Mode 0 only
     */
    enum cpu_width width;
};

struct parsed_instruction_s
{
    uint32_t opcode;
    enum cpu_width width;
    enum cpu_width size;

    struct parsed_operand_s operands[2];
};

typedef struct compiler_s
{
    array_t bytecode;
    uint32_t address;
} *compiler_t;

compiler_t compiler_create();
void compiler_destroy(compiler_t compiler);

void compiler_emit_bytes(compiler_t compiler, void *data, size_t count);
void compiler_emit_number(compiler_t compiler, uint32_t number, enum cpu_width width);
void compiler_emit_operand(compiler_t compiler, struct parsed_operand_s operand);
void compiler_emit_instruction(compiler_t compiler, struct parsed_instruction_s instruction);


void compiler_move_address(compiler_t compiler, uint32_t address);
