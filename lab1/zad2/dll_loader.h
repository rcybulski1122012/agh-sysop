#include <dlfcn.h>
#include "libwc.h"

#ifdef USE_DLL

bool loadDLL(char* file) {
    void *handle = dlopen(file, RTLD_LAZY);
    if(!handle) return false;

    *(void **) (&BlocksTable_create) = dlsym(handle, "BlocksTable_create");
    *(void **) (&BlocksTable_countWords) = dlsym(handle, "BlocksTable_countWords");
    *(void **) (&BlocksTable_get) = dlsym(handle, "BlocksTable_get");
    *(void **) (&BlocksTable_remove) = dlsym(handle, "BlocksTable_remove");
    *(void **) (&BlocksTable_free) = dlsym(handle, "BlocksTable_free");

    if(dlerror() != NULL) return false;

    return true;
}

#else
bool loadDLL(char* file) {return true}
#endif
