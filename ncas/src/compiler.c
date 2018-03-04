/*
  Created by nullifiedcat on 04.03.18.
*/

#include <instruction.h>
#include <stdio.h>
#include "compiler.h"

#include "instruction.h"

compiler_t compiler_create()
{
    compiler_t result = calloc(1, sizeof(struct compiler_s));

    result->address = 0;
    result->bytecode = array_init(sizeof(char), NULL);

    return result;
}

void compiler_destroy(compiler_t compiler)
{
    array_destroy(compiler->bytecode);
    free(compiler);
}

void compiler_emit_bytes(compiler_t compiler, void *data, size_t count)
{
    array_push(compiler->bytecode, data, count);
    compiler->address += count;
}

void compiler_emit_number(compiler_t compiler, uint32_t number, enum cpu_width width)
{
    switch (width)
    {
    case CPU_WIDTH_VOID:
        printf("Tried to emit VOID number?\n");
        break;
    case CPU_WIDTH_BYTE:
        /* TODO lsb/msb */
        compiler_emit_bytes(compiler, &number, 1);
        break;
    case CPU_WIDTH_WORD:
        compiler_emit_bytes(compiler, &number, 2);
        break;
    case CPU_WIDTH_DWORD:
        compiler_emit_bytes(compiler, &number, 4);
        break;
    }
}

void compiler_emit_operand(compiler_t compiler, struct parsed_operand_s operand)
{
    switch (operand.mode)
    {
    case OPERAND_MODE_IMMEDIATE:
        compiler_emit_number(compiler, operand.value, operand.width);
        break;
    case OPERAND_MODE_ADDRESS:
    case OPERAND_MODE_OFFSET:
        compiler_emit_number(compiler, operand.value, CPU_WIDTH_DWORD);
        break;
    case OPERAND_MODE_REGISTER:
    case OPERAND_MODE_REGISTER_INDIRECT:
        compiler_emit_bytes(compiler, &operand.rname1, 1);
        break;
    case OPERAND_MODE_REGISTER_INDIRECT_WITH_OFFSET:
        compiler_emit_bytes(compiler, &operand.rname1, 1);
        compiler_emit_number(compiler, operand.value, CPU_WIDTH_DWORD);
        break;
    case OPERAND_MODE_REGISTER_INDIRECT_WITH_FACTOR:
        compiler_emit_bytes(compiler, &operand.rname1, 1);
        compiler_emit_bytes(compiler, &operand.rname2, 1);
        compiler_emit_number(compiler, operand.factor, CPU_WIDTH_WORD);
        break;
    case OPERAND_MODE_NONE:
        break;
    }
}

void compiler_emit_instruction(compiler_t compiler, struct parsed_instruction_s instruction)
{
    union instruction_head_s head;
    head.byte = 0;

    head.size = instruction.size;

    if (instruction.size == CPU_WIDTH_VOID)
    {
        /* Single-byte instruction */
        head.micro.instruction = (uint8_t)instruction.opcode;
        compiler_emit_bytes(compiler, &head.byte, 1);
    }
    else
    {
        head.addressing.first = instruction.operands[0].mode;
        head.addressing.second = instruction.operands[1].mode;
        compiler_emit_bytes(compiler, &head.byte, 1);
        compiler_emit_operand(compiler, instruction.operands[0]);
        compiler_emit_operand(compiler, instruction.operands[1]);
    }

}

void compiler_move_address(compiler_t compiler, uint32_t address)
{
    compiler->address = address;
    /* TODO emit something in bytecode? */
}