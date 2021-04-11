#ifndef LOGEST_COMMON_SUBSTRING_SLIDINGWINDOW_H
#define LOGEST_COMMON_SUBSTRING_SLIDINGWINDOW_H

#include <utility>
#include <vector>
#include <deque>

class SlidingWindow {
private:
    std::vector<int> const _lcp_array;
    int _lowest_value, _highest_value;
    std::deque<int> _window_deque;
public:
    SlidingWindow(std::vector<int> lcpArray, int lowestValue, int highestValue) : _lcp_array(std::move(lcpArray)),
                                                                                  _lowest_value(lowestValue),
                                                                                  _highest_value(highestValue) {}

    void AdvanceWindow() {
        while (!_window_deque.empty() && _lcp_array[_window_deque.back()] > _lcp_array[_highest_value]) {
            _window_deque.pop_back();
        }
        _window_deque.push_back(_highest_value); // Add the next index to the back of the deque
        _highest_value++; // Increase the window size
    }

    void ShrinkWindow() {
        _lowest_value++; // Decrease window size by pushing it forward
        while (!_window_deque.empty() && _window_deque.front() < _lowest_value) _window_deque.pop_front();
    }

    int ExtractMin() {
        return _lcp_array[_window_deque.front()];
    }
};

#endif //LOGEST_COMMON_SUBSTRING_SLIDINGWINDOW_H
