#include "gruv.h"

void mkdir_cb(uv_fs_t *req) {
    gruv_fs_t *gruvReq = (gruv_fs_t *) req;
    gravity_closure_t *callback = (gravity_closure_t *) gruvReq->data;

    char * err = NULL;
    bool results = true;

    if (req->result < 0) {
        err = (char *) uv_strerror(req->result);
        results = false;
    }

    gravity_value_t args[2];

    args[0] = VALUE_FROM_STRING(gruvReq->vm, err, strlen(err));
    args[1] = VALUE_FROM_BOOL(results);

    gravity_vm_runclosure(
        gruvReq->vm,
        callback,
        VALUE_FROM_BOOL(results),
        args,
        2
    );

    uv_fs_req_cleanup(req);

}


bool gruv_mkdir (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused (vm, nargs)

    gruv_fs_t *req = malloc(sizeof(gruv_fs_t));

    req->vm = vm;
    req->data = VALUE_AS_CLOSURE(GET_VALUE(2));

    gravity_value_t pathValue = GET_VALUE(1);
    gravity_string_t *path = VALUE_AS_STRING(pathValue);

    uv_fs_mkdir(gruv_loop, (uv_fs_t*) req, path->s, S_IRUSR | S_IWUSR, mkdir_cb);

    RETURN_NOVALUE();
}
