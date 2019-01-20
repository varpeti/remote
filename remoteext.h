#pragma once

#include <vector>

#include "remote.h"

class remoteext
{
private:
    remoteext(){};
    ~remoteext(){};
public:  
    static void init(remote::Handle &handle, std::string name)
    {
        //*/// root

        if (getuid() != 0)
        {
            throw "Cannot start as NON ROOT user.";
        }
    
        //*/// pid
    
        while (true)
        {
            if (remote::FindProcessByName(name, &handle)) break;
            usleep(1000);
        }
    
        //*///
    }

    static unsigned long findByPattern(remote::Handle &handle, const char* data, const char* pattern)
    {
        unsigned long address=0;
        while (address==0) 
        {
            if (!handle.IsRunning()) throw "findByPattern: Target is not running";
    
            handle.ParseMaps();
    
            for (auto region : handle.regions)
            {
                address = (long)region.find
                (
                    handle,
                    data,
                    pattern
                );
    
                printf("%lx - %lx\n",region.start, address);
                if (address>0) break;
            }
    
            usleep(500);
        }
    
        return address;
    }

    static remote::MapModuleMemoryRegion *getRegionOfAddress(remote::Handle &handle, const void* address)
    {
        remote::MapModuleMemoryRegion *region = NULL;
        while (region==NULL) 
        {
            if (!handle.IsRunning()) throw "FindRegionOfAddress: Target is not running";
    
            handle.ParseMaps();
    
            region = handle.GetRegionOfAddress((void*)address);
    
            usleep(500);
        }
    
        return region;
    }

    static void getAddressesOfValue(remote::Handle &handle, void* addressOfValue, size_t size,std::vector<size_t> &addresses)
    {
        char *buffer = (char*)addressOfValue;

        //*///

        printf("(self)%lx:",(unsigned long)buffer);
        for (int i = 0; i < size; ++i)
        {
            printf("%02x ",buffer[i]);
        }
        printf("\n");

        //*///

        if (!handle.IsRunning()) throw "getAddressesOfValue: Target is not running";

        handle.ParseMaps();

        for (auto region : handle.regions)
        {
            region.find(handle,buffer,size,addresses);
        }

        //*/// 

        for (int i = 0; i < addresses.size(); ++i)
        {
            printf("%lx: ",addresses[i]);
            char data[size];
            if( handle.Read( (void*)(addresses[i]), data, sizeof(data) ) ) 
            {
                for (int j = 0; j < size; ++j)
                {
                    printf("%02x ",data[j]);
                }
            }
            
            printf("\n");
        }

        //*///

    }

};