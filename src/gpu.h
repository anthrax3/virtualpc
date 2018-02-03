#ifndef SRC_GPU_H_
#define SRC_GPU_H_

#include "mem.h"

struct gpu_s
{
    uint16_t width;
    uint16_t height;
    struct memory_s *vram;
};

void
gpu_init(struct gpu_s *out, size_t vram_size, uint16_t width, uint16_t height);

#endif /* SRC_GPU_H_ */
