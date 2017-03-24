#include "gruv.h"

/*

struct uv_fs_s {
  UV_REQ_FIELDS
  uv_fs_type fs_type;
  uv_loop_t* loop;
  uv_fs_cb cb;
  ssize_t result;
  void* ptr;
  const char* path;
  uv_stat_t statbuf;  / Stores the result of uv_fs_stat() and uv_fs_fstat(). *
  UV_FS_PRIVATE_FIELDS
};

typedef struct {
  uint64_t st_dev;
  uint64_t st_mode;
  uint64_t st_nlink;
  uint64_t st_uid;
  uint64_t st_gid;
  uint64_t st_rdev;
  uint64_t st_ino;
  uint64_t st_size;
  uint64_t st_blksize;
  uint64_t st_blocks;
  uint64_t st_flags;
  uint64_t st_gen;
  uv_timespec_t st_atim;
  uv_timespec_t st_mtim;
  uv_timespec_t st_ctim;
  uv_timespec_t st_birthtim;
} uv_stat_t;

*/

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

    if (err == NULL) {
        args[0] = VALUE_FROM_NULL;
    } else {
        args[0] = VALUE_FROM_STRING(gruvReq->vm, err, strlen(err));
    }

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

void stat_cb(uv_fs_t *req) {
    gruv_fs_t *gruvReq = (gruv_fs_t *) req;
    gravity_closure_t *callback = (gravity_closure_t *) gruvReq->data;

    gravity_value_t args[2];

    if (req->result < 0) {
        const char * err = uv_strerror(req->result);
        args[0] = VALUE_FROM_STRING(gruvReq->vm, err, strlen(err));
        args[1] = VALUE_FROM_NULL;
    } else {
        args[0] = VALUE_FROM_NULL;
        args[1] = VALUE_FROM_INT(req->statbuf.st_size);
    }

    gravity_vm_runclosure(
        gruvReq->vm,
        callback,
        VALUE_FROM_NULL,
        args,
        2
    );

    uv_fs_req_cleanup(req);
}


bool gruv_mkdir (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {

    gruv_fs_t *req = malloc(sizeof(gruv_fs_t));

    req->vm = vm;
    req->data = VALUE_AS_CLOSURE(GET_VALUE(2));
    gravity_value_t pathValue = GET_VALUE(1);
    gravity_string_t *path = VALUE_AS_STRING(pathValue);
    uv_fs_mkdir(gruv_loop, (uv_fs_t*) req, path->s, S_IRUSR | S_IWUSR, mkdir_cb);
    RETURN_NOVALUE();
}

bool gruv_rmdir (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    gruv_fs_t *req = malloc(sizeof(gruv_fs_t));

    gravity_string_t *path = VALUE_AS_STRING(GET_VALUE(1));
    req->data = VALUE_AS_CLOSURE(GET_VALUE(2));
    req->vm = vm;

    uv_fs_rmdir(gruv_loop, (uv_fs_t*) req, path->s, mkdir_cb);

    RETURN_NOVALUE();
}


bool gruv_stat (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    gruv_fs_t *req = malloc(sizeof(gruv_fs_t));

    gravity_string_t *path = VALUE_AS_STRING(GET_VALUE(1));
    req->data = VALUE_AS_CLOSURE(GET_VALUE(2));
    req->vm = vm;
    uv_fs_stat(gruv_loop, (uv_fs_t*) req, path->s, stat_cb);
    RETURN_NOVALUE();
}


