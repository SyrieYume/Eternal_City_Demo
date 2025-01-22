#pragma once

#define and &&
#define or ||
#define not !

// ARG_NUM(...) 展开为传入参数个数, 例如 ARG_NUM(1, 2, 3) -> 3
#define ARG_NUM(...)  ARG_X("ignored", __VA_ARGS__, 9,8,7,6,5,4,3,2,1,0)
#define ARG_X(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9, X,...) X


// CONCAT(a,b) 将a和b拼合起来， 例如 CONCAT(abc, def) -> abcdef
#define CONCAT(a,b)  CONCAT1(a,b)
#define CONCAT1(a,b)  a##b


// 实现 range(start, end, step)
#define range_3(start, end, step) start, end, step
#define range_2(start, end) start, end, 1
#define range_1(end) 0, end, 1
#define range(...) CONCAT(range_, ARG_NUM(__VA_ARGS__))(__VA_ARGS__)


// 实现 foreach(... in ...) 语法
#define var "ignored",
#define in ,
#define for_2(item, items) for(typeof(items[0]) *item = items, *end = items + len(items); item < end; item++)
#define for_4(i, _start, _end, _step) for(i = _start, end = _end; i < end; i += _step)
#define for_5(ignored, i, _start, _end, _step) for(int i = _start, end = _end; i != end; i += _step)
#define foreach(...) CONCAT(for_, ARG_NUM(__VA_ARGS__))(__VA_ARGS__)

#define len(list) (sizeof(list) / sizeof(list[0]))