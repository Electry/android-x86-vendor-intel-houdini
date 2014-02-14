/**
 *
 * Houdini hook library
 *
 * Copyright (C) 2013 The Android-x86 Open Source Project
 *
 * by Chih-Wei Huang <cwhuang@linux.org.tw>
 *
 * Licensed under GPLv2 or later
 *
 **/

#define LOG_TAG "houdini_hook"

#include <dlfcn.h>
#include <cutils/log.h>

extern "C" void dvmPlatformInvoke(void *pEnv, void *clazz, int argInfo, int argc,
        const int *argv, const char *shorty, void *func, void *pReturn);

namespace houdini {

static void *(*h_dvmHookDlopen)(const char *, int, bool *) = 0;
static int  (*h_dvmHookJniOnload)(bool, void *, void *, void *) = 0;
static bool (*h_dvmHookCheckMethod)(void *) = 0;
static void (*h_dvmHookCreateActivity)(bool, void *, void *, void *, void *, size_t) = 0;
static void (*h_dvmHookPlatformInvoke)(void *, void *, int, int, const int *, const char *, void *, void *) = 0;
static void *(*h_dvmHookDlsym)(bool, void *, const char *) = 0;

static void *get_symbol(const char *symbol)
{
    static void *h_handle = 0;

    if (!h_handle && !(h_handle = dlopen("libdvm_houdini.so", RTLD_LAZY))) {
        ALOGE("Unable to open libdvm_houdini lib");
        return 0;
    }

    const char *err = dlerror();
    void *func = dlsym(h_handle, symbol);
    err = dlerror();
    ALOGE_IF(err, "Unable to find %s function: %s", symbol, err);
    return func;
}

static void hookInit()
{
    h_dvmHookDlopen = (void *(*)(const char *, int, bool *)) get_symbol("_ZN7houdini10hookDlopenEPKciPb");
    h_dvmHookJniOnload = (int (*)(bool, void *, void *, void *)) get_symbol("_ZN7houdini13hookJniOnloadEbPvS0_S0_");
    h_dvmHookCheckMethod = (bool (*)(void *)) get_symbol("_ZN7houdini15hookCheckMethodEPv");
    h_dvmHookCreateActivity = (void (*)(bool, void *, void *, void *, void *, size_t)) get_symbol("_ZN7houdini18hookCreateActivityEbPvS0_S0_S0_j");
    h_dvmHookPlatformInvoke = (void (*)(void *, void *, int, int, const int *, const char *, void *, void *)) get_symbol("_ZN7houdini21dvmHookPlatformInvokeEPvS0_iiPKiPKcS0_S0_");
    h_dvmHookDlsym = (void *(*)(bool, void *, const char *)) get_symbol("_ZN7houdini9hookDlsymEbPvPKc");
}

void *hookDlopen(const char *filename, int flag, bool *useHoudini)
{
    if (!h_dvmHookDlopen) {
        hookInit();
    }

    return h_dvmHookDlopen ? h_dvmHookDlopen(filename, flag, useHoudini) : dlopen(filename, flag);
}

int hookJniOnload(bool useHoudini, void *func, void *jniVm, void *arg)
{
    return h_dvmHookJniOnload ? h_dvmHookJniOnload(useHoudini, func, jniVm, arg) : ((int (*)(void *, void *))func)(jniVm, arg);
}

bool hookCheckMethod(void *fnPtr)
{
    return h_dvmHookCheckMethod ? h_dvmHookCheckMethod(fnPtr) : false;
}

void hookCreateActivity(bool useHoudini, void *createActivityFunc,
        void *activity, void *houdiniActivity, void *savedState, size_t savedStateSize)
{
    if (!h_dvmHookCreateActivity) {
        hookInit();
    }
    if (h_dvmHookCreateActivity) {
        h_dvmHookCreateActivity(useHoudini, createActivityFunc, activity, houdiniActivity, savedState, savedStateSize);
    } else {
        ((void (*)(void *, void *, size_t))createActivityFunc)(activity, savedState, savedStateSize);
    }
}

void dvmHookPlatformInvoke(void *pEnv, void *clazz, int argInfo, int argc,
        const int *argv, const char *shorty, void *func, void *pReturn)
{
    if (h_dvmHookPlatformInvoke) {
	h_dvmHookPlatformInvoke(pEnv, clazz, argInfo, argc, argv, shorty, func, pReturn);
    } else {
	dvmPlatformInvoke(pEnv, clazz, argInfo, argc, argv, shorty, func, pReturn);
    }
}

void *hookDlsym(bool useHoudini, void *handle, const char *symbol)
{
    return h_dvmHookDlsym ? h_dvmHookDlsym(useHoudini, handle, symbol) : dlsym(handle, symbol);
}

}
