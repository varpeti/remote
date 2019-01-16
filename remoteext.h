#pragma once

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
        unsigned long offset=0;
        while (offset==0) 
        {
            if (!handle.IsRunning()) throw "findByPattern: Target is not running";
    
            handle.ParseMaps();
    
            for (auto region : handle.regions)
            {
                offset = (long)region.find
                (
                    handle,
                    data,
                    pattern
                );
    
                printf("%lx - %lx\n",region.start, offset);
                if (offset>0) break;
            }
    
            usleep(500);
        }
    
        return offset;
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

};