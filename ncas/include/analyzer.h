/*
  Created by nullifiedcat on 04.03.18.
*/

#pragma once

/*
 *  Analyzer processes all sentences/words from cutter
 *  and attaches meaning to them
 */

#include "model.h"

void analyze_sentences(array_t sentences);
void analyze_sentence(sentence_t sentence);
void analyze_word(word_t word);