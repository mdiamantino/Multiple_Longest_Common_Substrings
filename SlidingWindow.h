#ifndef LOGEST_COMMON_SUBSTRING_SLIDINGWINDOW_H
#define LOGEST_COMMON_SUBSTRING_SLIDINGWINDOW_H

#include <utility>
#include <vector>
#include <deque>

/**
 * Class used by MultipleLongestCommonSubstring to iterate over suffixes
 * and keep track of the longest common prefix in the window.
 */
class SlidingWindow final {
private:
    std::vector<int> const _lcp_array;
    int _lowest_value, _highest_value;
    std::deque<int> _window_deque;
public:
    SlidingWindow(std::vector<int> lcpArray, int lowestValue, int highestValue) : _lcp_array(std::move(lcpArray)),
                                                                                  _lowest_value(lowestValue),
                                                                                  _highest_value(highestValue) {}

    /*
     * Increases window size and make sure that the first value of the deque is the best
     */
    void AdvanceWindow() {
        while (!_window_deque.empty() && _lcp_array[_window_deque.back()] > _lcp_array[_highest_value]) {
            _window_deque.pop_back();
        }
        _window_deque.push_back(_highest_value);
        _highest_value++;
    }

    /*
    * Decreases window size and make sure that the first value of the deque is the best
    */
    void ShrinkWindow() {
        _lowest_value++;
        while (!_window_deque.empty() && _window_deque.front() < _lowest_value) _window_deque.pop_front();
    }

    [[nodiscard]] int ExtractMin() const {
        return _lcp_array[_window_deque.front()];
    }
};

#endif //LOGEST_COMMON_SUBSTRING_SLIDINGWINDOW_H
