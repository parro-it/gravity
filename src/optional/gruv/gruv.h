#ifndef __GRUV__
#define __GRUV__ 1

#include "../gravity_optional.h"
#include "gravity_core.h"
#include "gravity_opcodes.h"
#include "gravity_macros.h"
#include "gravity_memory.h"
#include <uv.h>

#define GRUV_CLASS                      "Gruv"
#define GRUV_STAT_CLASS                      "GruvStat"

static gravity_class_t *gruv_class;
static gravity_class_t *gruv_stat_class;

extern uv_loop_t *gruv_loop;

void gruv_init();
void gruv_register(gravity_vm *vm);

static bool gruv_version (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);
bool gruv_mkdir (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);
bool gruv_rmdir (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);
bool gruv_startLoop (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);
bool gruv_stat (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);
bool gruv_open (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);
    
typedef struct {
    gravity_closure_t *closure;
    gravity_vm *vm;
} gruv_fs_data;



#define STAT_INSERT_INT(name) gravity_hash_insert( \
    map->hash, \
    VALUE_FROM_STRING(vm, #name, strlen(#name)), \
    VALUE_FROM_INT(req->statbuf.st_ ## name) \
)

#define STAT_INSERT_TIME(name) gravity_hash_insert( \
    map->hash, \
    VALUE_FROM_STRING(vm, #name, strlen(#name)), \
    VALUE_FROM_INT(req->statbuf.st_ ## name.tv_sec) \
)

#endif
