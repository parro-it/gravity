#include "gruv.h"

void gruv_register(gravity_vm *vm) {
    gravity_vm_setvalue(vm, GRUV_CLASS, VALUE_FROM_OBJECT(gruv_class));
}

void gruv_init() {
    gruv_class = gravity_class_new_pair(NULL, GRUV_CLASS, NULL, 0, 0);
    gravity_class_t *gruv_meta = gravity_class_get_meta(gruv_class);
    gravity_class_bind(gruv_meta, "version", NEW_CLOSURE_VALUE(gruv_version));
    SETMETA_INITED(gruv_class);
}

static bool gruv_version (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused (vm, nargs)
    const char * version = uv_version_string();
    RETURN_VALUE(
        VALUE_FROM_STRING(vm, version, strlen(version)),
        rindex
    );
}
