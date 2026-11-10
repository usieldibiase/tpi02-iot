#include <cassert>
#include <iostream>
#include "../lib/Control/Control.h"
int main() {
  uint32_t n=99;
  for(const char* s : {"", "0", "-1", "+1", "1.5", "abc", "3601", "9999999999999999999999", "1 2", " 2"})
    assert(!parseSeconds(s,n,3600));
  assert(parseSeconds("1",n,3600)&&n==1);
  assert(parseSeconds("3600",n,3600)&&n==3600);
  RelayTimer t;t.start(100,2);assert(t.on);
  assert(!t.tick(2099));assert(t.on);
  assert(t.tick(2100));assert(!t.on);
  assert(!t.tick(2101));
  t.start(0xfffffff0u,1);
  assert(!t.tick(0x000003d7u));assert(t.tick(0x000003d8u));
  t.start(100,10);t.stop();assert(!t.tick(20000));assert(!t.on);
  t.start(100,10);t.start(200,2);assert(!t.tick(2199));assert(t.tick(2200));
  AlarmLatch a;
  assert(!a.update(2.5f,2.5f,2.3f));
  assert(a.update(2.51f,2.5f,2.3f));
  assert(!a.update(3.3f,2.5f,2.3f));
  assert(!a.update(2.4f,2.5f,2.3f));assert(a.high);
  assert(!a.update(2.3f,2.5f,2.3f));assert(a.high);
  assert(!a.update(2.29f,2.5f,2.3f));assert(!a.high);
  assert(a.update(2.6f,2.5f,2.3f));
  std::cout << "PASS: validacion, limites, temporizacion, rollover, apagado, reinicio e histeresis\n";
}
