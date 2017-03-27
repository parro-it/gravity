#include "gruv.h"
#include "gravity_hash.h"

void empty_result_cb(uv_fs_t *req) {
    gruv_fs_data *data = (gruv_fs_data *) req->data;

    gravity_value_t err;

    if (req->result < 0) {
        const char * message = uv_strerror(req->result);
        err = VALUE_FROM_STRING(data->vm, message, strlen(message));
    } else {
        err  = VALUE_FROM_NULL;
    }

    gravity_vm_runclosure(
        data->vm,
        data->closure,
        VALUE_FROM_NULL,
        &err,
        1
    );

    uv_fs_req_cleanup(req);
}

gravity_map_t * statStructToMap(uv_fs_t *req) {
    gruv_fs_data *data = (gruv_fs_data *) req->data;
    gravity_vm *vm = data->vm;
    gravity_map_t *map = gravity_map_new(vm, 10);

    STAT_INSERT_INT(dev);
    STAT_INSERT_INT(mode);
    STAT_INSERT_INT(nlink);
    STAT_INSERT_INT(uid);
    STAT_INSERT_INT(gid);
    STAT_INSERT_INT(rdev);
    STAT_INSERT_INT(ino);
    STAT_INSERT_INT(size);
    STAT_INSERT_INT(blksize);
    STAT_INSERT_INT(blocks);
    STAT_INSERT_INT(flags);
    STAT_INSERT_INT(gen);

    STAT_INSERT_TIME(atim);
    STAT_INSERT_TIME(mtim);
    STAT_INSERT_TIME(ctim);
    STAT_INSERT_TIME(birthtim);

    return map;
}

void stat_cb(uv_fs_t *req) {
    gruv_fs_data *data = (gruv_fs_data *) req->data;

    gravity_value_t err;

    if (req->result < 0) {
        const char * message = uv_strerror(req->result);
        err = VALUE_FROM_STRING(data->vm, message, strlen(message));
    } else {
        err  = VALUE_FROM_NULL;
    }

    gravity_value_t args[] = {err, VALUE_FROM_NULL};
    if (req->result >= 0) {
        args[1] = VALUE_FROM_OBJECT(statStructToMap(req));
    }

    gravity_vm_runclosure(
        data->vm,
        data->closure,
        VALUE_FROM_NULL,
        args,
        2
    );

    uv_fs_req_cleanup(req);
}

uv_fs_t * mk_req(gravity_vm *vm, gravity_value_t *args, uint16_t nargs) {
    uv_fs_t *req = malloc(sizeof(uv_fs_t));

    gruv_fs_data * data = malloc(sizeof(gruv_fs_data));
    req->data = data;
    data->vm = vm;
    data->closure = VALUE_AS_CLOSURE(GET_VALUE(nargs - 1));
    return req;
}

bool gruv_mkdir (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    uv_fs_t *req = mk_req(vm, args, nargs);

    gravity_string_t *path = VALUE_AS_STRING(GET_VALUE(1));
    gravity_int_t mode = VALUE_AS_INT(GET_VALUE(2));
    uv_fs_mkdir(gruv_loop, req, path->s, mode, empty_result_cb);
    
    RETURN_NOVALUE();
}

bool gruv_rmdir (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    uv_fs_t *req = mk_req(vm, args, nargs);

    gravity_string_t *path = VALUE_AS_STRING(GET_VALUE(1));
    
    uv_fs_rmdir(gruv_loop, req, path->s, empty_result_cb);

    RETURN_NOVALUE();
}


bool gruv_stat (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    uv_fs_t *req = mk_req(vm, args, nargs);

    gravity_string_t *path = VALUE_AS_STRING(GET_VALUE(1));
    
    uv_fs_stat(gruv_loop, req, path->s, stat_cb);
    
    RETURN_NOVALUE();
}


