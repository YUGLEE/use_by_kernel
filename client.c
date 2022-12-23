#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define __NR_bind_service   451
#define __NR_call           453
#define __NR_unbind_service 452
#define SERVICE_ID          1024

struct RPC_DATA {
    long client;
    long service;
    char data[128];
    size_t data_len;
};

static long client;

void clean_stdin(char *msg) {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
    puts(msg);
    puts("Please re-input:");
}

void bind_service() {
    long result = syscall(__NR_bind_service);
    if (result == -1) {
        puts("bind_service fail!");
    } else {
        client = result;
        printf("%s%ld%s\n", "User: ", result, ",\nbind_service successful!");
    }
}

void call() {
    struct RPC_DATA rpc_data;
    rpc_data.client = client;
    rpc_data.service = SERVICE_ID;
    puts("Please input your data:");
    scanf("%s", rpc_data.data);
    rpc_data.data_len = sizeof(rpc_data.data);
    syscall(__NR_call, rpc_data.client, &rpc_data, rpc_data.data_len);
    printf("========================\n"
           "client:\n\t%ld\n"
           "service:\n\t%ld\n"
           "data:\n\t%s\n"
           "data_len:\n\t%zu\n",
           rpc_data.client, rpc_data.service, rpc_data.data, rpc_data.data_len);
}

void unbind_service() {
    long result = syscall(__NR_unbind_service, client);
    if (result == -1) {
        puts("unbind_service fail!");
    } else {
        printf("%s%ld%s\n", "user: ", result, ",\nunbind_service successful!");
    }
}

void menu() {
    puts("=================");
    puts("1:bind service");
    puts("2:call");
    puts("3:unbind service");
    puts("4:exit");
    puts("=================");
}

int main() {
    bool is_exit = false;
    while (!is_exit) {
        menu();
        int choice;
        while (!scanf("%d", &choice)) {
            clean_stdin("Choice must be a number!");
        }
        switch (choice) {
            case 1: {
                bind_service();
                break;
            }
            case 2: {
                call();
                break;
            }
            case 3: {
                unbind_service();
                break;
            }
            case 4: {
                is_exit = true;
                break;
            }
            default: {
                puts("Please input num from 1 to 4!");
            }
        }
    }
    puts("goodbye!");
    return 0;
}