/*
 * In OS X, compile with
 * > gcc an_undead_program.c -o an_undead_program
 *
 * Run with
 * > ./an_undead_program &
 *
 * To terminate or signal a process, use `kill` command.
 * For example, the following command issues a SIGHUP signal to the process.
 * > kill -HUP $process_id
 */
#include <stdio.h>
#include <signal.h>

void handle(int arg) {
    printf("stop waking me up!\n");
}

int main() {
    signal(SIGHUP, handle);
    while (1)
        ; // do nothing except catching some signal
    return 0;
}
