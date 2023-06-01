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


#imguiCmake
cmake_minimum_required(VERSION 3.0)

project(myImGUITest)

# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(./include)
include_directories(./backends)
include_directories(./misc)
include_directories(${PROJECT_SOURCE_DIR}/quickjs)

set(SOURCES
	quickjs/quickjs.h
	quickjs/quickjs-libc.h
	quickjs/quickjs.c
	quickjs/libregexp.c
	quickjs/libunicode.c
	quickjs/libbf.c
	
	quickjs/cutils.c
	quickjs/quickjs-libc.c
  quickjs/qjsc.c)

add_library(quickjs SHARED ${SOURCES})

add_library(painter SHARED module/main.cpp)

target_link_libraries(painter PUBLIC quickjs)

add_executable(test src/main.cpp 
${PROJECT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
${PROJECT_SOURCE_DIR}/include/imgui.cpp 
${PROJECT_SOURCE_DIR}/include/imgui_demo.cpp 
${PROJECT_SOURCE_DIR}/include/imgui_draw.cpp 
${PROJECT_SOURCE_DIR}/include/imgui_tables.cpp 
${PROJECT_SOURCE_DIR}/include/imgui_widgets.cpp
${PROJECT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)


target_link_libraries(test PUBLIC glfw GL dl pthread painter)

#imguiModule
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "string.h"
#include "stdlib.h"
#include <quickjs.h>
#include <quickjs-libc.h>

#define JS_INIT_MODULE js_init_module
#define countof(x) (sizeof(x) / sizeof((x)[0]))

bool drawLine = false;
bool isDrawing = 0;
ImDrawList *drawList;
bool done = 0;
ImVec2 lineStart;
ImVec2 lineEnd;
bool flag = 0;
bool flag1 = 0;
bool clear = 0;

std::vector<ImVec2> lineS;
std::vector<ImVec2> lineE;
std::vector<std::vector<ImVec2>> points;
int k = 0;

extern "C" {

static void glfw_error_callback(int error, const char *description) {
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
  // std::cout << "x:" << xpos << ' ' << "y:" << ypos << std::endl;
  if (drawLine) {
    lineEnd = ImVec2(static_cast<float>(xpos), static_cast<float>(ypos));
  } else if (flag1) {
    std::cout << "x:" << xpos << ' ' << "y:" << ypos << std::endl;
    points[k].push_back(
        ImVec2(static_cast<float>(xpos), static_cast<float>(ypos)));
  }
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (drawLine) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && drawLine) {
      flag = 1;
      double mouseX, mouseY;
      glfwGetCursorPos(window, &mouseX, &mouseY);
      lineStart =
          ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
      lineEnd = lineStart;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE &&
               drawLine) {
      double mouseX, mouseY;
      glfwGetCursorPos(window, &mouseX, &mouseY);
      lineEnd = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
      done = 1;
      flag = 0;
    }
  } else if (isDrawing) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
      flag1 = 1;
      if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        points[k].emplace_back(
            ImVec2(static_cast<float>(xpos), static_cast<float>(ypos)));
        // isDrawing = true;
      } else if (action == GLFW_RELEASE) {
        isDrawing = 0;
        flag1 = 0;
        k++;
        std::vector<ImVec2> t;
        points.push_back(t);
      }
    }
  }
}

void drawCurve() {
  if (points.size() > 1) {
    for (int j = 0; j < k; j++)
      for (size_t i = 0; i < points[j].size() - 1; i++) {
        ImGui::GetForegroundDrawList()->AddLine(
            points[j][i], points[j][i + 1], IM_COL32(255, 255, 255, 255), 2.0f);
      }
  }
  if (points[k].size() < 2)
    return;

  // for (int i = 0; i < points.size(); i++) {
  //   for (int j = 0; j < points[i].size() - 1; i++) {
  //     ImGui::GetForegroundDrawList()->AddLine(points[i][j], points[i][j],
  //     IM_COL32(255, 255, 255, 255),
  //                       2.0f);
  //   }
  // }
  for (size_t i = 0; i < points[k].size() - 1; i++) {
    ImGui::GetForegroundDrawList()->AddLine(points[k][i], points[k][i + 1],
                                            IM_COL32(255, 255, 255, 255), 2.0f);
  }
}

int painter() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;
  std::vector<ImVec2> t;
  points.push_back(t);
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow *window =
      glfwCreateWindow(1280, 720, "myImGuiTest", nullptr, nullptr);

  if (window == nullptr)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  ImGui::StyleColorsDark();
  // 设置鼠标按钮回调
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  // 设置鼠标移动回调
  glfwSetCursorPosCallback(window, cursorPosCallback);
  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Our state
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("画板!"); // Create a window called "Hello, world!"
                           // and append into it.

    ImGui::Text("This is some useful text."); // Display some text (you can

    ImGui::ColorEdit3(
        "clear color",
        (float *)&clear_color); // Edit 3 floats representing a color

    ImGui::Checkbox("drawLine", &drawLine);
    ImGui::Checkbox("isDrawing", &isDrawing);
    if (lineS.size()) {
      for (int i = 0; i < lineS.size(); i++)
        ImGui::GetForegroundDrawList()->AddLine(
            lineS[i], lineE[i], IM_COL32(255, 255, 255, 255), 2.0f);
    }
    if (drawLine) {
      if (flag) {
        ImGui::GetForegroundDrawList()->AddLine(
            lineStart, lineEnd, IM_COL32(255, 255, 255, 255), 2.0f);
      }
      if (done) {
        lineS.emplace_back(lineStart);
        lineE.emplace_back(lineEnd);
        done = 0;
      }
    }
    drawCurve();
    ImGui::Checkbox("clear", &clear);
    if (clear) {
      lineE.clear();
      lineS.clear();
      points.clear();
      clear = 0;
      std::vector<ImVec2> t;
      points.push_back(t);
      k = 0;
    }
    ImGui::End();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

static JSValue js_test_painter(JSContext *ctx, JSValueConst this_val,
                           int argc, JSValueConst *argv)
{
    return JS_NewFloat64(ctx, painter());
}

/* 定义API的函数入口名称及列表 */
static const JSCFunctionListEntry js_test_funcs[] = {
    /* JS_CFUNC_DEF(函数入口名称，入参个数，QuickJS C 函数) */
    JS_CFUNC_DEF("painter", 2, js_test_painter),
};
/* 定义初始化回调方法（由系统调用，入参格式固定），将函数入口列表 在模块中暴露 */
static int js_test_init(JSContext *ctx, JSModuleDef *m)
{
    return JS_SetModuleExportList(ctx, m, js_test_funcs,
                                  countof(js_test_funcs));
}

/* 定义初始化模块方法，由系统自动调用，且函数名称不可更改 */
JSModuleDef *JS_INIT_MODULE(JSContext *ctx, const char *module_name)
{
    JSModuleDef *m;
    m = JS_NewCModule(ctx, module_name, js_test_init);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_test_funcs, countof(js_test_funcs));
    return m;
}

}


#.vscode

{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/test",
            "args": [],
            "cwd": "${workspaceFolder}/build",
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "为 gdb 启用整齐打印",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}

```
