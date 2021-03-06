#include "../remoteext.h"

#define PROCESS_NAME "dummy.run"
#define PATTERN "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x68\x0d\x00\x00\x00\x00\x00\x00\x48\x00\x00\x00\x00\x7f\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\xa0\x00\x00\x00\x00\x7f\x00\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf2\x00\x00\x00\x00\x7f\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x20\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x76\x00\x00\x00\x00\x00\x00\x00\xc8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x00\x00\x00\x00\x00\x7f\x00\x00\x0d\x00\x00\x00\x00\x00\x00\x00\x44\x65\x66\x61\x75\x6c\x74\x53\x74\x72\x69\x6e\x67\x00\x00\x00\x4c\x6f\x6e\x67\x20\x63\x68\x61\x72\x20\x61\x72\x72\x61\x79\x20\x72\x69\x67\x68\x74\x20\x74\x68\x65\x72\x65\x20\x2d\x3e\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00","??xx??????xxx????xxxx????xxxxxxxxxxxx????xxxx????xxxx????xxxx?????xx?????xxxxxxxxxxxx????xxx?????xxxx?????xx?????xxxx?????xxx????????????xxx?????xxx?????xxx?????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define NUM_OFFSET 0x80


int main()
{
    try
    {
        //*/// Init

        remote::Handle handle;

        printf("Finding process ID: \n");
        remoteext::init(handle,PROCESS_NAME);
        printf("Discovered with PID: %d \n",handle.GetPid());

        //*/// FindByPattern

        printf("Finding address by Pattern: \n");
        unsigned long address = remoteext::findByPattern(handle,PATTERN);
        printf("Discovered address: %lx \n",address);


        //*/// Memory dump

        printf("Dump nearby memory of address: \n");

        remote::MapModuleMemoryRegion *region = remoteext::getRegionOfAddress(handle, (void *) address);
        unsigned long size = 0x100;

        region->print(handle, (void*)address, size);

        //*/// Read-Write

        int num(0);
        if( handle.Read( (void*)(address+NUM_OFFSET), &num, sizeof(num) ) ) printf("num: %d\n",num);
        else throw "Reading failed";

        num=9;
        if( handle.Write( (void*)(address+NUM_OFFSET), &num, sizeof(num) ) ) printf("Writed successfully: %d\n",num);
        else throw "Writing failed";

        //*/// Scan

        int a(17); printf("%lx: %d\n", (unsigned long)&a, a);
        std::vector<size_t> addresses;
        remoteext::getAddressesOfValue(handle, &a, sizeof(a), addresses);

    }
    catch (const char* msg)
    {
        printf("%s\n", msg);
    }

    printf("end\n");

    getchar(); //TODO remove me

    return 0;
}