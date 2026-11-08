#pragma once
#include <stdint.h>
inline bool elapsed(uint32_t now,uint32_t start,uint32_t interval) {
  return uint32_t(now-start)>=interval;
}
inline bool parseSeconds(const char* s,uint32_t& value,uint32_t maximum) {
  if(!s || !*s) return false;
  uint32_t n=0;
  for(;*s;++s) {
    if(*s<'0'||*s>'9') return false;
    unsigned digit=*s-'0';
    if(n>maximum/10 || (n==maximum/10 && digit>maximum%10)) return false;
    n=n*10+digit;
  }
  if(n==0) return false;
  value=n; return true;
}
struct RelayTimer {
  bool on=false; uint32_t started=0,duration=0;
  void start(uint32_t now,uint32_t seconds) { on=true; started=now; duration=seconds*1000u; }
  void stop() { on=false; }
  bool tick(uint32_t now) {
    if(on && elapsed(now,started,duration)) { on=false; return true; }
    return false;
  }
};
struct AlarmLatch {
  bool high=false;
  bool update(float v,float threshold,float reset) {
    if(!high && v>threshold) {high=true; return true;}
    if(high && v<reset) high=false;
    return false;
  }
};
