#pragma once

/* C++ domain */
#ifdef __cplusplus
class Console
{
private:
    static Console* single;
    Console();
    ~Console();
public:
    static void start();
};
#endif

/* Wrapping function for C domain */
#ifdef __cplusplus
extern "C" {
#endif

void ConsoleStart(void);

#ifdef __cplusplus
}
#endif