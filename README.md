# use_by_kernel

## step 1:

```shell
#run in local 
git clone -b finalwork https://github.com/L1nf/linux.git
git clone https://github.com/L1nf/use_by_kernel.git
cd use_by_kernel
chmod +x ./finalwork.sh
./finalwork.sh
```

## step 2:

```shell
#run in qemu
nohup ./service &
./client
```
