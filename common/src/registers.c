/*
 * registers.c
 *
 *  Created on: Feb 11, 2018
 *      Author: nullifiedcat
 */

#include "registers.h"

#include <stdlib.h>

struct register_description_s registers[] = {
        [REGISTER_A] =
            (struct register_description_s){
                .offset = 0, .size = 4, .exposed = 1, .name = "ra" },
        [REGISTER_B] =
            (struct register_description_s){
                .offset = 4, .size = 4, .exposed = 1, .name = "rb" },
        [REGISTER_C] =
            (struct register_description_s){
                .offset = 8, .size = 4, .exposed = 1, .name = "rc" },
        [REGISTER_D] =
            (struct register_description_s){
                .offset = 12, .size = 4, .exposed = 1, .name = "rd" },
        [REGISTER_E] =
            (struct register_description_s){
                .offset = 16, .size = 4, .exposed = 1, .name = "re" },
        [REGISTER_F] =
            (struct register_description_s){
                .offset = 20, .size = 4, .exposed = 1, .name = "rf" },
        [REGISTER_G] =
            (struct register_description_s){
                .offset = 24, .size = 4, .exposed = 1, .name = "rg" },
        [REGISTER_H] =
            (struct register_description_s){
                .offset = 28, .size = 4, .exposed = 1, .name = "rh" },
        [REGISTER_PC] =
            (struct register_description_s){
                .offset = 32, .size = 4, .exposed = 0, .name = "pc" },
        [REGISTER_SP] =
            (struct register_description_s){
                .offset = 36, .size = 4, .exposed = 1, .name = "sp" },
        [REGISTER_BP] =
            (struct register_description_s){
                .offset = 40, .size = 4, .exposed = 1, .name = "bp" },
        [REGISTER_FLAGS] =
            (struct register_description_s){
                .offset = 44, .size = 4, .exposed = 0, .name = "flags" },
};

const struct register_description_s *register_get(uint32_t name)
{
    if (name > REGISTER_COUNT)
        return NULL;

    if (registers[name].exposed == 0)
        return NULL;

    return &registers[name];
}
