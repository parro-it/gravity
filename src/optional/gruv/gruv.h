#ifndef __GRUV__
#define __GRUV__ 1

#define SETMETA_INITED(c)                       gravity_class_get_meta(c)->is_inited = true
#define GET_VALUE(_idx)                         args[_idx]
#define RETURN_VALUE(_v,_i)                     do {gravity_vm_setslot(vm, _v, _i); return true;} while(0)
#define RETURN_CLOSURE(_v,_i)                   do {gravity_vm_setslot(vm, _v, _i); return false;} while(0)
#define RETURN_FIBER()                          return false
#define RETURN_NOVALUE()                        return true
#define RETURN_ERROR(...)                       do {                                                                        \
                                                    char buffer[4096];                                                      \
                                                    snprintf(buffer, sizeof(buffer), __VA_ARGS__);                          \
                                                    gravity_fiber_seterror(gravity_vm_fiber(vm), (const char *) buffer);    \
                                                    gravity_vm_setslot(vm, VALUE_FROM_NULL, rindex);                        \
                                                    return false;                                                           \
                                                } while(0)

#define DECLARE_1VARIABLE(_v,_idx)              register gravity_value_t _v = GET_VALUE(_idx)
#define DECLARE_2VARIABLES(_v1,_v2,_idx1,_idx2) DECLARE_1VARIABLE(_v1,_idx1);DECLARE_1VARIABLE(_v2,_idx2)

#define CHECK_VALID(_v, _msg)                   if (VALUE_ISA_NOTVALID(_v)) RETURN_ERROR(_msg)
#define INTERNAL_CONVERT_FLOAT(_v)              _v = convert_value2float(vm,_v); CHECK_VALID(_v, "Unable to convert object to Float")
#define INTERNAL_CONVERT_BOOL(_v)               _v = convert_value2bool(vm,_v); CHECK_VALID(_v, "Unable to convert object to Bool")
#define INTERNAL_CONVERT_INT(_v)                _v = convert_value2int(vm,_v); CHECK_VALID(_v, "Unable to convert object to Int")
#define INTERNAL_CONVERT_STRING(_v)             _v = convert_value2string(vm,_v); CHECK_VALID(_v, "Unable to convert object to String")

#define NEW_FUNCTION(_fptr)                     (gravity_function_new_internal(NULL, NULL, _fptr, 0))
#define NEW_CLOSURE_VALUE(_fptr)                ((gravity_value_t){ .isa = gravity_class_closure,       \
                                                                    .p = (gravity_object_t *)gravity_closure_new(NULL, NEW_FUNCTION(_fptr))})

#define FUNCTION_ISA_SPECIAL(_f)                (OBJECT_ISA_FUNCTION(_f) && (_f->tag == EXEC_TYPE_SPECIAL))
#define FUNCTION_ISA_DEFAULT_GETTER(_f)         ((_f->index < GRAVITY_COMPUTED_INDEX) && (_f->special[EXEC_TYPE_SPECIAL_GETTER] == NULL))
#define FUNCTION_ISA_DEFAULT_SETTER(_f)         ((_f->index < GRAVITY_COMPUTED_INDEX) && (_f->special[EXEC_TYPE_SPECIAL_SETTER] == NULL))
#define FUNCTION_ISA_GETTER(_f)                 (_f->special[EXEC_TYPE_SPECIAL_GETTER] != NULL)
#define FUNCTION_ISA_SETTER(_f)                 (_f->special[EXEC_TYPE_SPECIAL_SETTER] != NULL)
#define FUNCTION_ISA_BRIDGED(_f)                (_f->index == GRAVITY_BRIDGE_INDEX)

#include "gravity_core.h"
#include "gravity_opcodes.h"
#include "gravity_macros.h"
#include "gravity_memory.h"
#include <uv.h>

#define GRUV_CLASS                      "Gruv"

static gravity_class_t *gruv_class;

extern uv_loop_t *gruv_loop;

void gruv_init();
void gruv_register(gravity_vm *vm);

static bool gruv_version (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);
bool gruv_mkdir (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);
bool gruv_startLoop (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex);

typedef struct gruv_fs_t {
    uv_fs_t inherit;
    void *data;
    gravity_vm *vm;
} gruv_fs_t;


#endif
