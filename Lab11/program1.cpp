//Kevin Anderson
//Zach Hopman

#include <Windows.h>
#include <iostream>
#include <Psapi.h>

using std::cout;
using std::endl;

int main() {
    SYSTEM_INFO info;
    GetSystemInfo(&info);

    cout << "Page Size: " << info.dwPageSize << endl;

    int* array = new int[1 << 18];

    MEMORY_BASIC_INFORMATION vinfo;
    VirtualQuery(array, &vinfo, sizeof(vinfo));

    cout << "Array Size: " << vinfo.RegionSize << endl;
    
    PROCESS_MEMORY_COUNTERS counters;
    GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters));

    cout << "Committed Memory(KB): " << counters.PagefileUsage / 1024.0 << endl;
    cout << "Reserved Memory(KB): " << counters.WorkingSetSize / 1024.0 << endl;
    cout << "Free Memory(KB): " << (counters.WorkingSetSize / 1024.0) - (counters.PagefileUsage / 1024.0) << endl;

    delete array;
    cout << "Free Array Memory" << endl;

    GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters));

    cout << "Committed Memory(KB): " << counters.PagefileUsage / 1024.0 << endl;
    cout << "Reserved Memory(KB): " << counters.WorkingSetSize / 1024.0 << endl;
    cout << "Free Memory(KB): " << (counters.WorkingSetSize / 1024.0) - (counters.PagefileUsage / 1024.0) << endl;

    return 0;
}
