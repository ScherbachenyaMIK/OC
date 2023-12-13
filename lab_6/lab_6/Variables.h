#pragma once
std::condition_variable start_event;
std::mutex start_event_m1;
std::mutex start_event_m2;
std::condition_variable work_finish_event;
std::mutex work_finish_event_m1;
std::mutex work_finish_event_m2;

std::mutex main_count_element_cs;

int threads_started = 0;