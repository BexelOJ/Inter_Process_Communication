#include "vmlinux.h"

#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>

char LICENSE[] SEC("license") = "GPL";

SEC("tracepoint/syscalls/sys_enter_execve")
int trace_execve(struct trace_event_raw_sys_enter* ctx)
{
    char filename[256];

    bpf_probe_read_user_str(
        filename,
        sizeof(filename),
        (const char*)ctx->args[0]);

    bpf_printk(
        "execve(): %s",
        filename);

    return 0;
}



