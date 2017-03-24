#include "gruv.h"

static uv_loop_t *gruv_loop;

void gruv_register(gravity_vm *vm) {
    gravity_vm_setvalue(vm, GRUV_CLASS, VALUE_FROM_OBJECT(gruv_class));
}

void gruv_init() {
    gruv_class = gravity_class_new_pair(NULL, GRUV_CLASS, NULL, 0, 0);
    gravity_class_t *gruv_meta = gravity_class_get_meta(gruv_class);

    gravity_class_bind(gruv_meta, "version", NEW_CLOSURE_VALUE(gruv_version));
    gravity_class_bind(gruv_meta, "startLoop", NEW_CLOSURE_VALUE(gruv_startLoop));
    gravity_class_bind(gruv_meta, "mkdir", NEW_CLOSURE_VALUE(gruv_mkdir));

    SETMETA_INITED(gruv_class);

    gruv_loop = malloc(sizeof(uv_loop_t));

    uv_loop_init(gruv_loop);

}

static bool gruv_version (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused (vm, nargs)
    const char * version = uv_version_string();
    RETURN_VALUE(
        VALUE_FROM_STRING(vm, version, strlen(version)),
        rindex
    );
}
