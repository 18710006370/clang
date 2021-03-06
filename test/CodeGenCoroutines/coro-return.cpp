// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -fcoroutines-ts -std=c++14 -emit-llvm %s -o - -disable-llvm-passes | FileCheck %s

namespace std {
namespace experimental {
template <typename... T>
struct coroutine_traits;
}
}

struct suspend_always {
  bool await_ready();
  void await_suspend();
  void await_resume();
};

template<>
struct std::experimental::coroutine_traits<void> {
  struct promise_type {
    void get_return_object();
    suspend_always initial_suspend();
    suspend_always final_suspend();
    void return_void();
  };
};

// CHECK-LABEL: f0(
extern "C" void f0() {
  // CHECK: %__promise = alloca %"struct.std::experimental::coroutine_traits<void>::promise_type"
  // CHECK: %call = call i8* @_Znwm(
  // CHECK: call void @_ZNSt12experimental16coroutine_traitsIJvEE12promise_type11return_voidEv(%"struct.std::experimental::coroutine_traits<void>::promise_type"* %__promise)
  // CHECK: call void @_ZdlPv
  co_return;
}

template<>
struct std::experimental::coroutine_traits<int> {
  struct promise_type {
    int get_return_object();
    suspend_always initial_suspend();
    suspend_always final_suspend();
    void return_value(int);
  };
};

// CHECK-LABEL: f1(
extern "C" int f1() {
  // CHECK: %__promise = alloca %"struct.std::experimental::coroutine_traits<int>::promise_type"
  // CHECK: %call = call i8* @_Znwm(
  // CHECK: call void @_ZNSt12experimental16coroutine_traitsIJiEE12promise_type12return_valueEi(%"struct.std::experimental::coroutine_traits<int>::promise_type"* %__promise, i32 42)
  // CHECK: call void @_ZdlPv
  co_return 42;
}
