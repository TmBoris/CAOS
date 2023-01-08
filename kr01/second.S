    .text
    .global call_process
call_process:
    push $process+3
    ret
