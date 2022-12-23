#include <unistd.h>

#define __NR_serve   454
#define __NR_reply   455

struct RPC_DATA {
    long client;
    long service;
    char data[128];
    size_t data_len;
};

int main(void) {
    while (1) {
        sleep(5);
        struct RPC_DATA rpc_data;
        long result = syscall(__NR_serve, &rpc_data, &rpc_data.data_len);
        if (result) {
            for (int i = 0; rpc_data.data[i] != '\0'; i++) {
                if (rpc_data.data[i] >= 'a' && rpc_data.data[i] <= 'z')
                    rpc_data.data[i] -= 32;
                else if (rpc_data.data[i] >= 'A' && rpc_data.data[i] <= 'Z')
                    rpc_data.data[i] += 32;
            }
            rpc_data.data_len = sizeof(rpc_data.data);
            syscall(__NR_reply, rpc_data.client, &rpc_data);
        }
    }
}