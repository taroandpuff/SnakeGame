#### TODO
- 连接`quick js`

#### 构建框架
- 基于`ubuntu2004`下的`QT5`
- 使用`CMake`编译
- 可使用`VSCode` 进行调试


```
# camke quickjs
cmake_minimum_required(VERSION 3.0)

project(libquickjs)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILE_FEATURES "${CMAKE_CXX_COMPILE_FEATURES} /MT /GR- /utf-8")
add_definitions(-DUNICODE -D_UNICODE)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(quickjs)

set(SOURCES
	quickjs/quickjs.h
	quickjs/quickjs-libc.h
	quickjs/quickjs.c
	quickjs/libregexp.c
	quickjs/libunicode.c
	quickjs/cutils.c
	quickjs/quickjs-libc.c)



	

# build quickjs static library
add_library(quickjs STATIC ${SOURCES})

add_subdirectory(test)
# build test program
# add_executable(test test/main.cpp)
# target_link_libraries(test quickjs dl pthread)



# cmake quickjs-test

add_executable(test main.cpp)
target_link_libraries(test quickjs dl pthread)




#include <quickjs.h>
#include <quickjs-libc.h>

#include <iostream>
#include <fstream>
#include <string>


const char script[] = R"(
    console.log("hello world");

)";

int main()
{
    // create ESM code file for test
    std::ofstream("test-module.js")
        << "export function funcInModule(){ console.log('[=] message from ES Module'); };";

    // start test
    auto runtime = JS_NewRuntime();
    if (nullptr == runtime)
    {
        std::cout << "Failed to create js runtime" << std::endl;
        return 1;
    }
    auto context = JS_NewContext(runtime);
    if (nullptr == context)
    {
        std::cout << "Failed to create js context" << std::endl;
        return 1;
    }

    JS_SetModuleLoaderFunc(runtime, NULL, js_module_loader, NULL);  /* set loader for ES6 modules */
    js_std_init_handlers(runtime);           // init event handlers
    js_std_add_helpers(context, 0, nullptr); // add helpers like 'console', 'print'
    js_init_module_os(context, "os");        // register module 'os'

    auto result = JS_Eval(context, script, sizeof(script) - 1, "<input>", JS_EVAL_TYPE_MODULE);
    if (JS_IsException(result))
    {
        js_std_dump_error(context);
        return 1;
    }
    JS_FreeValue(context, result);

    js_std_loop(context);

    js_std_free_handlers(runtime);
    JS_FreeContext(context);
    JS_FreeRuntime(runtime);

    return 0;
}
```
