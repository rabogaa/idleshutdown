//
// Created by user on 11/10/2017.
//

#pragma once

#ifndef TIMER_EXPORTS
#define TIMER_API __declspec(dllexport)
#else
#define TIMER_API __declspec(dllimport)
#endif

extern "C" TIMER_API LONG KS_GetLastInputTickCount();
