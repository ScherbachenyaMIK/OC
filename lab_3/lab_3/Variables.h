#pragma once
CRITICAL_SECTION cs1, cs2;

HANDLE hWorkEvent;
HANDLE hCountEnterSectionEvent;
HANDLE hWorkEnterSectionEvent;
HANDLE count_element;
HANDLE work;

int t;
double x;
int result = 1;
int n;
