#include <bal/abi.h>
#include <bal/boot.h>
#include <brutal/alloc.h>
#include <brutal/debug.h>
#include <brutal/fibers.h>
#include <ipc/ipc.h>

WEAK int br_main(
    long arg1,
    long arg2,
    MAYBE_UNUSED long arg3,
    MAYBE_UNUSED long arg4,
    MAYBE_UNUSED long arg5)
{
    IpcComponent component;
    ipc_component_init(&component, nullptr, alloc_global());
    ipc_component_inject(&component, (IpcCapability *)arg1, (int)arg2);
    int result = ipc_component_main(&component);
    ipc_component_deinit(&component);
    return result;
}

void _entry(long arg1, long arg2, long arg3, long arg4, long arg5)
{
    fiber_start(nullptr, nullptr);

    int result = br_main(arg1, arg2, arg3, arg4, arg5);

    br_exit(&(BrExitArgs){
        .handle = BR_HANDLE_SELF,
        .result = result,
    });
}
