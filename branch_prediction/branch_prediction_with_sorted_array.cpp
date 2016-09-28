#include <algorithm>
#include <ctime>
#include <iostream>

int main()
{
    // Generate data
    const unsigned arraySize = 32768;
    int data[arraySize];
    int sortedData[arraySize];

    for (unsigned c = 0; c < arraySize; ++c)
    {
        int random_number = std::rand() % 256;

        data[c] = random_number;
        sortedData[c] = random_number;
    }

    std::sort(sortedData, sortedData + arraySize);

    // Test
    clock_t start = clock();
    long long sum = 0;

    for (unsigned i = 0; i < 100000; ++i)
    {
        // Primary loop
        for (unsigned c = 0; c < arraySize; ++c)
        {
            if (sortedData[c] >= 128)
                sum += sortedData[c];
        }
    }

    double elapsedTime = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;

    std::cout << "Elapsed time with sorted array: ";
    std::cout << elapsedTime << std::endl;
    std::cout << "sum = " << sum << std::endl;
}
