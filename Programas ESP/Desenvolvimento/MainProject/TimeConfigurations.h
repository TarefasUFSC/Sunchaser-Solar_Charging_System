#ifndef TIMECONFIGURATIONS_H
#define TIMECONFIGURATIONS_H

#include <Arduino.h>

class TimeConfigurations{
    private:
        int ReadingInterval; // time interval between readings
        int CacheMaxSize;    // max size of the cache
        int LTMazSize;       // max size of the long term memory
    public:
        TimeConfigurations(){
            ReadingInterval = 60; // 1 hour
            CacheMaxSize = 24;    // 1 day
            LTMazSize = 720;      // 1 month
        };
        void set_ReadingInterval(int newInterval){this->ReadingInterval = newInterval;};
        void set_CacheMaxSize(int newCacheMaxSize){this->CacheMaxSize = newCacheMaxSize;};
        void set_LTMazSize(int newLTMazSize){this->LTMazSize = newLTMazSize;};
        int get_ReadingInterval(){return ReadingInterval;};
        int get_CacheMaxSize(){return CacheMaxSize;};
        int get_LTMMazSize(){return LTMazSize;};
};


#endif
