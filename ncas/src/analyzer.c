/*
  Created by nullifiedcat on 04.03.18.
*/

#include "analyzer.h"
#include "../../pc/include/cpu.h"

#include <stdio.h>
#include <string.h>
#include <model.h>
#include <mnemonics.h>
#include <registers.h>

static const char *keyword_table[] = {[KW_LABEL]  = "label",
                                      [KW_CONST]  = "const",
                                      [KW_ORIGIN] = "origin",
                                      [KW_STORE]  = "store", NULL };

static int identify_keyword(word_t word)
{
    int i = 1;

    for (; keyword_table[i]; ++i)
    {
        if (!strcmp(word->content, keyword_table[i]))
        {
            word->meaning      = WM_KEYWORD;
            word->data.keyword = (enum keyword) i;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

static const char *width_table[] = {[CPU_WIDTH_BYTE]  = "byte",
                                    [CPU_WIDTH_WORD]  = "word",
                                    [CPU_WIDTH_DWORD] = "dword", NULL };

static int identify_width(word_t word)
{
    int i = 0;

    for (; width_table[i]; ++i)
    {
        if (!strcmp(width_table[i], word->content))
        {
            word->meaning    = WM_WIDTH;
            word->data.width = (enum cpu_width) i;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

static int identify_misc(word_t word)
{
    if (word->content[0] == '"')
    {
        word->meaning = WM_STRING;
        return EXIT_SUCCESS;
    }

    if (strlen(word->content) != 1)
        return EXIT_FAILURE;

    switch (word->content[0])
    {
    case '+':
        word->meaning      = WM_OPERATOR;
        word->data.operator= OPERATOR_PLUS;
        return EXIT_SUCCESS;
    case '-':
        word->meaning      = WM_OPERATOR;
        word->data.operator= OPERATOR_MINUS;
        return EXIT_SUCCESS;
    case '*':
        word->meaning      = WM_OPERATOR;
        word->data.operator= OPERATOR_MULTIPLY;
        return EXIT_SUCCESS;
    case ',':
        word->meaning = WM_COMMA;
        return EXIT_SUCCESS;
    case '[':
        word->meaning = WM_ADDRESSING_START;
        return EXIT_SUCCESS;
    case ']':
        word->meaning = WM_ADDRESSING_END;
        return EXIT_SUCCESS;
    default:
        break;
    }

    return EXIT_FAILURE;
}

static int identify_number(word_t word)
{
    size_t length = strlen(word->content);
    int base;

    /* The default base will be 16, h is redundant */

    switch (word->content[length - 1])
    {
    case 'o':
        base = 8;
        break;
    case 'd':
        base = 10;
        break;
    case 'b':
        base = 2;
        break;
    default:
        base = 16;
    }

    char *endptr;
    uint32_t number = (uint32_t) strtoul(word->content, &endptr, base);

    if (endptr < word->content + length - 1)
        return EXIT_FAILURE;

    word->meaning     = WM_NUMBER;
    word->data.number = number;
    return EXIT_SUCCESS;
}

static int identify_register(word_t word)
{
    uint32_t i = 0;
    for (; i < REGISTER_COUNT; ++i)
    {
        const struct register_description_s *reg = register_get(i);

        if (reg == NULL)
            continue;

        if (!strcmp(reg->name, word->content))
        {
            word->meaning    = WM_REGISTER;
            word->data.rname = (enum register_name) i;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

static int identify_mnemonic(word_t word)
{
    uint32_t out;
    enum cpu_width width;
    if (instruction_from_mnemonic(word->content, &out, &width) == EXIT_SUCCESS)
    {
        word->meaning                = WM_MNEMONIC;
        word->data.instruction.code  = out;
        word->data.instruction.width = width;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

void analyze_sentences(array_t sentences)
{
    size_t i = 0;
    for (; i < sentences->length; ++i)
    {
        analyze_sentence(*(sentence_t *) array_get(sentences, i));
    }
}

void analyze_sentence(sentence_t sentence)
{
    if (sentence->words->length == 0)
    {
        printf(
            "WARNING: Sentence word count is 0! This should never happen!\n");
        return;
    }

    size_t i = 0;
    for (; i < sentence->words->length; ++i)
    {
        analyze_word(*(word_t *) array_get(sentence->words, i));
    }

    word_t first = *(word_t *) array_get(sentence->words, 0);

    switch (first->meaning)
    {
    case WM_KEYWORD:
        sentence->meaning = SM_HINT;
        break;
    case WM_MNEMONIC:
        sentence->meaning = SM_INSTRUCTION;
        break;
    default:
        sentence->meaning = SM_UNKNOWN;
        printf("WARNING: Sentence meaning unknown: first word is %d (%s)\n",
               first->meaning, first->content);
        break;
    }
}

static const char *meanings[] = {
        "Unknown", "Keyword", "Width", "Operator", "Comma", "AddressingStart", "AddressingEnd",
        "Number", "String", "Mnemonic", "Register", "Identifier", NULL
};

void analyze_word(word_t word)
{
    printf("? %s => ", word->content);

    if (identify_keyword(word) == EXIT_SUCCESS ||
        identify_width(word) == EXIT_SUCCESS ||
        identify_misc(word) == EXIT_SUCCESS ||
        identify_number(word) == EXIT_SUCCESS ||
        identify_mnemonic(word) == EXIT_SUCCESS ||
        identify_register(word) == EXIT_SUCCESS)
    {
        printf("%s\n", meanings[word->meaning]);
    }
    else
    {
        printf("(UNKNOWN)\n");
    }
}
