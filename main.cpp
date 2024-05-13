#include <iostream>     // std::cout
#include <vector>       // std::vector
#include <array>        // std::array
#include <cstdlib>      // std::rand
#include <chrono>       // std::chrono::seconds
#include <numeric>      // std::accumulate
#include <exception>    // std::exception

constexpr std::uint32_t LEN = 1000000u;
constexpr std::array<std::uint8_t, 6> screens = { 4u, 8u, 16u, 32u, 64u, 128u };

template<typename T>
concept FloatingPointNumber = std::is_floating_point_v<T>;

template<FloatingPointNumber FP>
std::vector<FP> GenInputData() {
    std::vector<FP> retVec;
    retVec.reserve(LEN);

    for(std::uint32_t i = 0; i < LEN; i++) {
        retVec.emplace_back((FP)(rand()) / (FP)(std::rand()));
    }

    return retVec; // NRVO
}

template<FloatingPointNumber FP>
std::vector<FP> CalcMovingAverage(const std::vector<FP>& values, std::uint8_t screen) {
    std::vector<FP> average;
    average.reserve(values.size() - screen);

    auto iter = std::next(values.begin(), screen);
    for(; iter != values.end(); iter++) {
        auto sum = std::accumulate(std::next(iter, -(screen)), iter, 0.0);
        average.emplace_back(sum / screen);
    }

    return average; // NRVO
}

template<FloatingPointNumber FP>
void PrettyPrintResult(const std::vector<FP>& average,
                        std::uint8_t screen,
                        const std::chrono::milliseconds execTime) {
    if(!average.empty()) {
        std::cout   << "Execution time for : "
                    << typeid(*average.begin()).name() // for me prints "f" & "d"
                    << ", with screen = "
                    << std::to_string(screen)
                    << " is "
                    << std::to_string(execTime.count())
                    << "."
                    << std::endl
                    << "Counting speed is "
                    << std::to_string(LEN / execTime.count())
                    << " units per milisecond, that equals "
                    << std::to_string((LEN * 1000) / execTime.count())
                    << " units per second. "
                    << std::endl;
        return;
    }
    throw std::invalid_argument("Simple moving average is empty!");
}

int main() {
    try {
        // Setup data for test
        std::srand(std::time(nullptr));
        auto inputFloatData = GenInputData<float>();
        auto inputDoubleData = GenInputData<double>();

        // Iterateing through screens and counting average
        for(const auto& screen : screens) {
            auto startFloat = std::chrono::system_clock::now();
            auto floatAverage = CalcMovingAverage(inputFloatData, screen);
            auto finishFloat = std::chrono::system_clock::now();

            auto startDouble = std::chrono::system_clock::now();
            auto doubleAverage = CalcMovingAverage(inputDoubleData, screen);
            auto finishDouble = std::chrono::system_clock::now();

            const auto durationFloat = std::chrono::duration_cast<std::chrono::milliseconds>(finishFloat - startFloat);
            const auto durationDouble = std::chrono::duration_cast<std::chrono::milliseconds>(finishDouble - startDouble);
            PrettyPrintResult(floatAverage, screen, durationFloat);
            PrettyPrintResult(doubleAverage, screen, durationDouble);
        }
    } catch(const std::exception& ex) {
        std::cout << "Programm interrupt with error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}