#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include "../quickjs/quickjs.h"

// Callback to print raw string to stdout
static JSValue js_raw_print_stdout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (argc >= 1) {
        const char *str = JS_ToCString(ctx, argv[0]);
        if (str) {
            std::cout << str;
            JS_FreeCString(ctx, str);
        }
    }
    return JS_UNDEFINED;
}

// Callback to print raw string to stderr
static JSValue js_raw_print_stderr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if (argc >= 1) {
        const char *str = JS_ToCString(ctx, argv[0]);
        if (str) {
            std::cerr << str;
            JS_FreeCString(ctx, str);
        }
    }
    return JS_UNDEFINED;
}

// Helper to print exceptions
void print_exception(JSContext *ctx) {
    JSValue exception_val = JS_GetException(ctx);
    const char *message = JS_ToCString(ctx, exception_val);
    std::cerr << "Exception: " << (message ? message : "unknown") << "\n";
    if (message) {
        JS_FreeCString(ctx, message);
    }

    JSValue stack = JS_GetPropertyStr(ctx, exception_val, "stack");
    if (!JS_IsUndefined(stack)) {
        const char *stack_str = JS_ToCString(ctx, stack);
        if (stack_str) {
            std::cerr << stack_str << "\n";
            JS_FreeCString(ctx, stack_str);
        }
    }
    JS_FreeValue(ctx, stack);
    JS_FreeValue(ctx, exception_val);
}

int main(int argc, char *argv[]) {
    std::string code;
    std::string filename = "<stdin>";

    // Read input code from file or stdin
    if (argc > 1 && std::strcmp(argv[1], "-") != 0) {
        filename = argv[1];
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file '" << filename << "'\n";
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        code = buffer.str();
    } else {
        std::stringstream buffer;
        buffer << std::cin.rdbuf();
        code = buffer.str();
    }

    // Initialize QuickJS Runtime and Context
    JSRuntime *rt = JS_NewRuntime();
    if (!rt) {
        std::cerr << "Error: Could not create JS runtime\n";
        return 1;
    }
    JSContext *ctx = JS_NewContext(rt);
    if (!ctx) {
        std::cerr << "Error: Could not create JS context\n";
        JS_FreeRuntime(rt);
        return 1;
    }

    // Bind custom print functions to global scope
    JSValue global_obj = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global_obj, "__raw_print_stdout", 
                      JS_NewCFunction(ctx, js_raw_print_stdout, "__raw_print_stdout", 1));
    JS_SetPropertyStr(ctx, global_obj, "__raw_print_stderr", 
                      JS_NewCFunction(ctx, js_raw_print_stderr, "__raw_print_stderr", 1));
    JS_FreeValue(ctx, global_obj);

    // Bootstrap JS environment (console object setup)
    const char *bootstrap = R"js(
        globalThis.console = {
            log: function(...args) {
                let formatted = args.map(arg => {
                    if (arg === null) return 'null';
                    if (arg === undefined) return 'undefined';
                    if (Array.isArray(arg)) {
                        return '[' + arg.map(x => {
                            if (x === null) return 'null';
                            if (x === undefined) return 'undefined';
                            if (typeof x === 'string') return `'${x}'`;
                            if (typeof x === 'object') return JSON.stringify(x);
                            return String(x);
                        }).join(', ') + ']';
                    }
                    if (typeof arg === 'object') {
                        try {
                            return JSON.stringify(arg);
                        } catch(e) {
                            return '[object Object]';
                        }
                    }
                    return String(arg);
                });
                __raw_print_stdout(formatted.join(' ') + '\n');
            },
            error: function(...args) {
                __raw_print_stderr(args.map(String).join(' ') + '\n');
            },
            warn: function(...args) {
                __raw_print_stderr('Warning: ' + args.map(String).join(' ') + '\n');
            }
        };
    )js";

    JSValue bootstrap_val = JS_Eval(ctx, bootstrap, std::strlen(bootstrap), "<bootstrap>", JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(bootstrap_val)) {
        std::cerr << "Error: Failed to initialize console object\n";
        print_exception(ctx);
        JS_FreeValue(ctx, bootstrap_val);
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
        return 1;
    }
    JS_FreeValue(ctx, bootstrap_val);

    // Evaluate user JavaScript code
    JSValue val = JS_Eval(ctx, code.c_str(), code.length(), filename.c_str(), JS_EVAL_TYPE_GLOBAL);
    int exit_code = 0;

    if (JS_IsException(val)) {
        print_exception(ctx);
        exit_code = 1;
    } else {
        // Execute any pending microtasks (e.g. promise jobs)
        JSContext *ctx1;
        for (;;) {
            int err = JS_ExecutePendingJob(rt, &ctx1);
            if (err <= 0) {
                if (err < 0) {
                    print_exception(ctx1);
                    exit_code = 1;
                }
                break;
            }
        }
    }

    JS_FreeValue(ctx, val);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    return exit_code;
}
