#include "Value/Value.h"
#include <algorithm>
#include <vector>
#include <thread>
#include "Utility/Timer.h"
#include <iostream>

const unsigned int WIN_X = 1900;
const unsigned int WIN_Y = 1005;
bool g_ongoing = false;
bool g_shuffleOngoing = false;
bool g_startSorting = false;
bool g_joinShuffleThread = false;
bool g_joinSortingThread = false;
double g_sleep = 0.0;
double g_sortTime = 0.0;
sf::Font g_font;

bool g_abort = false;

enum Algorithm
{
    BubbleSort = 0,
    InsertionSort,
    SelectionSort,
    RandomSort,
    GnomeSort,
    RadixSort,
    QuickSort,
    HeapSort,
    MergeSort,
    PancakeSort,
    CycleSort,
    ShellSort,
    StoogeSort,
    CombSort,
    CocktailSort,
    PigeonholeSort,
    StdSort
};
const int MAX_ALG = 17;

void SetSleep(Algorithm a);
std::string EnumToString(Algorithm a);
void Shuffle(std::vector<Value>* val, int it);
double Sort(std::vector<Value>* val, Algorithm a);

double _bubbleSort(std::vector<Value>* val);
double _insertionSort(std::vector<Value>* val);
double _selectionSort(std::vector<Value>* val);
double _randomSort(std::vector<Value>* val);
double _gnomeSort(std::vector<Value>* val);
double _radixSort(std::vector<Value>* val);
double _quickSort(std::vector<Value>* val);
double _heapSort(std::vector<Value>* val);
double _mergeSort(std::vector<Value>* val);
double _pancakeSort(std::vector<Value>* val);
double _cycleSort(std::vector<Value>* val);
double _shellSort(std::vector<Value>* val);
double _stoogeSort(std::vector<Value>* val);
double _combSort(std::vector<Value>* val);
double _cocktailSort(std::vector<Value>* val);
double _pigeonholeSort(std::vector<Value>* val);
double _stdSort(std::vector<Value>* val);
void Init(std::vector<Value> & val);

int main()
{
    srand(time(0));

    sf::RenderWindow wnd(sf::VideoMode(WIN_X, WIN_Y), "Sorter");
    wnd.setPosition(sf::Vector2i(0, 0));
    g_font.loadFromFile("AGENCYB.TTF");

    sf::Text info;
    info.setFont(g_font);
    info.setFillColor(sf::Color::Red);

    int it = WIN_X * 2;

    Algorithm useAlg[MAX_ALG];
    for (int i = 0; i < MAX_ALG; i++)
    {
        useAlg[i] = (Algorithm)i;
    }
    int targetAlg = 0;

    std::vector<Value> vals(WIN_X);
    Init(vals);

    std::thread shuffle_tread;
    std::thread sorting_trhead;
    
    bool restartPressedLastFrame = false;
    
    bool lPressedLastFrame = false;
    bool rPressedLastFrame = false;
    bool uPressedLastFrame = false;
    bool dPressedLastFrame = false;
    
    bool addPressedLastFrame = false;
    bool subPressedLastFrame = false;
    

    Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.01;

    while (wnd.isOpen() || g_ongoing || g_shuffleOngoing || g_joinShuffleThread || g_joinSortingThread)
    {
        if (g_abort)
        {
            g_abort = g_ongoing || g_shuffleOngoing;
        }

        sf::Event e;
        while (wnd.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                wnd.close();
                g_abort = true;
            }
        }

        if (!wnd.isOpen())
            g_abort = true;

        bool restartPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        bool lPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
        bool rPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
        bool uPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
        bool dPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
        bool subPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract);
        bool addPressed =
            sf::Keyboard::isKeyPressed(sf::Keyboard::Add);
        
        bool abortPressed = 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Q);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            g_abort = true;
            wnd.close();
        }
            
        g_abort = g_abort || abortPressed;
        
        if (!lPressedLastFrame && lPressed)
        {
            targetAlg--;
            if (targetAlg < 0)
                targetAlg = MAX_ALG - 1;

            SetSleep(useAlg[targetAlg]);
        }
        if (!rPressedLastFrame && rPressed)
        {
            targetAlg++;
            if (targetAlg >= MAX_ALG)
                targetAlg = 0;

            SetSleep(useAlg[targetAlg]);
        }
        if (!uPressedLastFrame && uPressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION += 0.005;
            else
                Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION += 0.10;
        }

        if (!subPressedLastFrame && subPressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                it -= 10;
            else
            {
                if (it == 0)
                {
                    it = -1;
                }
                else if (it < 0)
                    it *= 2;
                else
                    it /= 2;
            }
        }

        if (!addPressedLastFrame && addPressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                it += 10;
            else
            {
                if (it == 0)
                    it = 10;
                else if (it < 0)
                    it /= 2;
                else
                    it *= 2;
            }
        }

        if (!dPressedLastFrame && dPressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION -= 0.005;
            else
                Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION -= 0.10;

            if (Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION < 0)
                Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0;
        }

        if (!g_ongoing && !g_shuffleOngoing &&
            !restartPressedLastFrame && restartPressed)
        {
            Init(vals);
            g_shuffleOngoing = true;
            g_sleep = Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION;
            Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.1;
            shuffle_tread = std::thread(&Shuffle, &vals, it);
        }  

        lPressedLastFrame = lPressed;
        rPressedLastFrame = rPressed;
        uPressedLastFrame = uPressed;
        dPressedLastFrame = dPressed;

        subPressedLastFrame = subPressed;
        addPressedLastFrame = addPressed;

        restartPressedLastFrame = restartPressed;
        
        if (g_joinShuffleThread)
        {
            g_joinShuffleThread = false;
            shuffle_tread.join();
        }

        if (g_startSorting)
        {
            g_startSorting = false;
            g_ongoing = true;
            Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = g_sleep;
            Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
            g_sleep = 0.0;
            sorting_trhead = std::thread(&Sort, &vals, useAlg[targetAlg]);

        }

        if (g_joinSortingThread)
        {
            g_joinSortingThread = false;
            sorting_trhead.join();
        }

        std::string alg = "Algorithm: ";

        alg += EnumToString(useAlg[targetAlg]);

        alg += "\n";

        std::string work = g_ongoing ? "Sorting" : "Done";

        info.setString(alg +
            "Delay: " +
            std::to_string(Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION) +
            " ms\n" +
            "Suffle it: " + std::to_string(it) + "\n" +
            "Sort time: " + std::to_string(abs(g_sortTime)) + " ms\n" + work);

        wnd.clear();
        for (int i = 0; i < vals.size(); i++)
            vals[i].Draw(&wnd);
        wnd.draw(info);
        wnd.display();
    }

  return 0;
}

void SetSleep(Algorithm a)
{
    switch (a)
    {
    case BubbleSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.01;
        break;
    case InsertionSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.05;
        break;
    case SelectionSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.5;
        break;
    case RandomSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.01;
        break;
    case GnomeSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.01;
        break;
    case RadixSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.1;
        break;
    case QuickSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.5;
        break;
    case HeapSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.5;
        break;
    case MergeSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.5;
        break;
    case PancakeSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.005;
        break;
    case CycleSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.1;
        break;
    case ShellSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.1;
        break;
    case StoogeSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.5;
        break;
    case CombSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.1;
        break;
    case CocktailSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.005;
        break;
    case PigeonholeSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.1;
        break;
    case StdSort:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.5;
        break;
    default:
        Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.0;
        break;
    }
}

std::string EnumToString(Algorithm a)
{
    switch (a)
    {
    case BubbleSort:
        return "BubbleSort";
        break;
    case InsertionSort:
        return "InsertionSort";
        break;
    case SelectionSort:
        return "SelectionSort";
        break;
    case RandomSort:
        return "RandomSort";
        break;
    case GnomeSort:
        return "GnomeSort";
        break;
    case RadixSort:
        return "RadixSort";
        break;
    case QuickSort:
        return "QuickSort";
        break;
    case HeapSort:
        return "HeapSort";
        break;
    case MergeSort:
        return "MergeSort";
        break;
    case PancakeSort:
        return "PancakeSort";
        break;
    case CycleSort:
        return "CycleSort";
        break;
    case ShellSort:
        return "ShellSort";
        break;
    case StoogeSort:
        return "StoogeSort";
        break;
    case CombSort:
        return "CombSort";
        break;
    case CocktailSort:
        return "CocktailSort";
        break;
    case PigeonholeSort:
        return "PigeonholeSort";
        break;
    case StdSort:
        return "StdSort";
        break;
    default:
        return "UNKOWN";
        break;
    }
}

void Shuffle(std::vector<Value>* val, int it)
{
    if (it < 0)
    {
        if (it == -1)
            it = 0;
        std::reverse(val->begin(), val->end());
    }

    for (int i = 0; i < abs(it); i++)
    {
        if (g_abort)
        {
            g_startSorting = true;
            g_shuffleOngoing = false;
            g_joinShuffleThread = true;
            return;
        }

        int i1 = rand() % val->size();
        int i2 = rand() % val->size();

        Value t = val->at(i1);
        val->at(i1) = val->at(i2);
        val->at(i2) = t;
    }
    g_startSorting = true;
    g_shuffleOngoing = false;
    g_joinShuffleThread = true;
}

double Sort(std::vector<Value>* val, Algorithm a)
{
    double t = 0.0;

    switch (a)
    {
    case BubbleSort:
        t = _bubbleSort(val);
        break;
    case InsertionSort:
        t = _insertionSort(val);
        break;
    case SelectionSort:
        t = _selectionSort(val);
        break;
    case RandomSort:
        t = _randomSort(val);
        break;
    case GnomeSort:
        t = _gnomeSort(val);
        break;
    case RadixSort:
        t = _radixSort(val);
        break;
    case QuickSort:
        t = _quickSort(val);
        break;
    case HeapSort:
        t = _heapSort(val);
        break;
    case MergeSort:
        t = _mergeSort(val);
        break;
    case PancakeSort:
        t = _pancakeSort(val);
        break;
    case CycleSort:
        t = _cycleSort(val);
        break;
    case ShellSort:
        t = _shellSort(val);
        break;
    case StoogeSort:
        t = _stoogeSort(val);
        break;
    case CombSort:
        t = _combSort(val);
        break;
    case CocktailSort:
        t = _cocktailSort(val);
        break;
    case PigeonholeSort:
        t = _pigeonholeSort(val);
        break;
    case StdSort:
        t = _stdSort(val);
        break;
    default:
        t = _stdSort(val);
        break;
    }
    g_ongoing = false;
    g_joinSortingThread = true;
    g_sortTime = t;
    return t;
}

double _bubbleSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int size = val->size();

    for (int i = 0; i < size; i++)
    {
        //val->at(i).SetColor(sf::Color::Red);
        for (int j = i + 1; j < size; j++)
        {
            if (g_abort)
                return -1;

            //val->at(j).SetColor(sf::Color::Red);
            if (val->at(j) < val->at(i))
            {
                Value t = val->at(i);
                val->at(i) = val->at(j);
                val->at(j) = t;
            }
            //val->at(j).SetColor(sf::Color::White);
        }
        //val->at(i).SetColor(sf::Color::Green);
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;

    return time;
}

double _insertionSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int size = val->size();

    for (int i = 1; i < size; i++)
    {
        Value t = val->at(i);
        //val->at(i).SetColor(sf::Color::Red);
        int j = i - 1;
        while ((j >= 0) && (t < val->at(j)))
        {
            if (g_abort)
                return -1;
            val->at(j + 1) = val->at(j);
            j = j - 1;
        }
        val->at(j + 1) = t;
        //val->at(i).SetColor(sf::Color::White);
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    return time;
}

double _selectionSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int size = val->size();

    for (int i = 0; i < size - 1; i++)
    {
        int minIdx = i;

        //val->at(i).SetColor(sf::Color::Red);

        for (int j = i + 1; j < size; j++)
        {
            if (g_abort)
                return -1;
            if (val->at(j) < val->at(minIdx))
            {
                val->at(minIdx) = val->at(minIdx); // Just to sleep

                minIdx = j;
                //val->at(minIdx).SetColor(sf::Color::Red);
            }
        }

        //val->at(i).SetColor(sf::Color::White);
        //val->at(minIdx).SetColor(sf::Color::White);
        Value t = val->at(i);
        val->at(i) = val->at(minIdx);
        val->at(minIdx) = t;
        //val->at(i).SetColor(sf::Color::Green);
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    return time;
}

double _randomSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int size = val->size();

    bool isSorted = false;
    bool swap = true;
    while (!isSorted)
    {
        if (g_abort)
            return -1;
        if (swap)
        {
            isSorted = true;
            for (int i = 1; i < size; i++)
            {
                if (val->at(i) < val->at(i - 1))
                {
                    isSorted = false;
                    break;
                }
            }
        }
        if (isSorted)
            break;

        
        int id1 = rand() % size;
        int id2 = rand() % size;

        if (id2 < id1)
        {
            int t = id1;
            id1 = id2;
            id2 = t;
        }
        
        if (val->at(id1) >= val->at(id2))
        {
            Value t = val->at(id1);
            val->at(id1) = val->at(id2);
            val->at(id2) = t;
        }
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;

    return time;
}

double _gnomeSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;

    int index = 0;
    int n = val->size();

    while (index < n)
    {
        if (g_abort)
            return -1;
        if (index == 0)
            index++;
        if (val->at(index) >= val->at(index - 1))
            index++;
        else
        {
            Value t = val->at(index);
            val->at(index) = val->at(index - 1);
            val->at(index - 1) = t;
            index--;
        }
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;

    return time;
}

Value __getMax(std::vector<Value>* vec)
{
    size_t size = vec->size();
    size_t mxIdx = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (vec->at(i) > vec->at(mxIdx))
            mxIdx = i;
    }
    
    return vec->at(mxIdx);
}

void __countSort(std::vector<Value>* vec, size_t n, int exp)
{
    double sleepVal = Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION;
    Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0;
    std::vector<Value> output = *vec;
    Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = sleepVal;

    int count[10];
    memset(count, 0, sizeof(int) * 10);

    for (int i = 0; i < n; i++)
        count[(output[i].GetVal() / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];
     
     for (int i = n - 1; i >= 0; i--)
    {
        vec->at(count[(output[i].GetVal() / exp) % 10] - 1) = output[i];
        count[(output[i].GetVal() / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
        output[i] = vec->at(i);

    *vec = output;

    /*std::vector<Value> output(n);
    
    int count[10];
    memset(count, 0, sizeof(int) * 10);

    for (int i = 0; i < n; i++)
        count[(vec->at(i).GetVal() / exp) % 10]++;

    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (int i = n - 1; i >= 0; i--)
    {
        output[count[(vec->at(i).GetVal() / exp) % 10] - 1] = vec->at(i);
        count[(vec->at(i).GetVal() / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
        vec->at(i) = output[i];*/
}

double _radixSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;

    int m = __getMax(val).GetVal();
    size_t n = val->size();

    for (int exp = 1; m / exp > 0; exp *= 10)
    {
        if (g_abort)
            return -1;
        __countSort(val, n, exp);
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;

    return time;
}

int __partition(std::vector<Value> * vec, int low, int high)
{
    Value pivot = vec->at(high);
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (vec->at(j) <= pivot)
        {
            i++;
            Value t = vec->at(i);
            vec->at(i) = vec->at(j);
            vec->at(j) = t;
        }
    }

    Value t = vec->at(i + 1);
    vec->at(i + 1) = vec->at(high);
    vec->at(high) = t;
    return (i + 1);
}

void __quickSort(std::vector<Value>* val, int low, int high)
{
    if (low < high)
    {
        if (g_abort)
            return;
        int pi = __partition(val, low, high);
        __quickSort(val, low, pi - 1);
        __quickSort(val, pi + 1, high);
    }
}

double _quickSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;

    __quickSort(val, 0, (int)val->size() - 1);

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;

    if (g_abort)
        return -1;
    return time;
}

void __heapify(std::vector<Value>* val, int size, int i)
{
    if (g_abort)
        return;

    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && val->at(left) > val->at(largest))
        largest = left;
    if (right < size && val->at(right) > val->at(largest))
        largest = right;

    if (largest != i)
    {
        Value t = val->at(i);
        val->at(i) = val->at(largest);
        val->at(largest) = t;

        __heapify(val, size, largest);
    }

}

void __heapSort(std::vector<Value>* val, int size)
{
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        if (g_abort)
            return;
        __heapify(val, size, i);
    }

    for (int i = size - 1; i >= 0; i--)
    {
        if (g_abort)
            return;
        Value t = val->at(0);
        val->at(0) = val->at(i);
        val->at(i) = t;
        
        __heapify(val, i, 0);
    }
}

double _heapSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;

    __heapSort(val, (int)val->size());

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    if (g_abort)
        return -1;

    return time;
}

void __merge(std::vector<Value>* val, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<Value> L(n1);
    std::vector<Value> R(n2);

    for (i = 0; i < n1; i++)
        L[i] = val->at(l + i);
    for (j = 0; j < n2; j++)
        R[j] = val->at(m + 1 + j);

    i = 0;
    j = 0; 
    k = l; 
    while (i < n1 && j < n2)
    {
        if (g_abort)
            return;
        if (L[i] <= R[j])
        {
            val->at(k) = L[i];
            i++;
        }
        else
        {
            val->at(k) = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        val->at(k) = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        val->at(k) = R[j];
        j++;
        k++;
    }
}

void __mergeSort(std::vector<Value>* val, int l, int r)
{
    if (l < r)
    {
        if (g_abort)
            return;
        int m = l + (r - l) / 2;
        __mergeSort(val, l, m);
        __mergeSort(val, m+1, r);
        __merge(val, l, m, r);
    }
}

double _mergeSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;

    __mergeSort(val, 0, (int)val->size() - 1);

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    if (g_abort)
        return -1;

    return time;
}

int __findMax(std::vector<Value>* val, int size)
{
    int mi = 0;

    for (int i = 1; i < size; i++)
        if (val->at(i) > val->at(mi))
            mi = i;

    return mi;
}

void __flip(std::vector<Value>* val, int i)
{
    
    int start = 0;
    while (start < i)
    {
        if (g_abort)
            return;
        Value t = val->at(start);
        val->at(start) = val->at(i);
        val->at(i) = t;
        start++;
        i--;
    }
}

double _pancakeSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;

    int n = val->size();

    for (int curr_size = n; curr_size > 1; --curr_size)
    {
        if (g_abort)
            return -1;
        int mi = __findMax(val, curr_size);

        if (mi != curr_size - 1)
        {
            __flip(val, mi);
            __flip(val, curr_size - 1);
        }
        //val->at(curr_size - 1).SetColor(sf::Color::Green);
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    return time;
}

double _cycleSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int n = val->size();

    int writes = 0;

    for (int cycle_start = 0; cycle_start <= n - 2; cycle_start++)
    {
        if (g_abort)
            return -1;
        Value item = val->at(cycle_start);

        int pos = cycle_start;
        for (int i = cycle_start + 1; i < n; i++)
            if (val->at(i) < item)
                pos++;

        if (pos == cycle_start)
            continue;

        while (item == val->at(pos))
            pos += 1;

        if (pos != cycle_start)
        {
            Value t = item;
            item = val->at(pos);
            val->at(pos) = t;

            writes++;
        }

        while (pos != cycle_start)
        {
            if (g_abort)
                return -1;
            pos = cycle_start;

            for (int i = cycle_start + 1; i < n; i++)
                if (val->at(i) < item)
                    pos += 1;

            while (item == val->at(pos))
                pos += 1;

            if (item != val->at(pos)) {
                Value t = item;
                item = val->at(pos);
                val->at(pos) = t;
                writes++;
            }
        }
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    return time;
}

double _shellSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int n = val->size();

    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            if (g_abort)
                return -1;
            Value t = val->at(i);

            int j;
            for (j = i; j >= gap && val->at(j - gap) > t; j -= gap)
                val->at(j) = val->at(j - gap);

            val->at(j) = t;
        }
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    return time;
}

void __stoogesort(std::vector<Value>* vec, int l, int h)
{
    if (g_abort)
        return;
    if (l >= h)
        return;

    if (vec->at(l) > vec->at(h))
    {
        Value t = vec->at(l);
        vec->at(l) = vec->at(h);
        vec->at(h) = t;
    }

    if (h - l + 1 > 2) {
        int t = (h - l + 1) / 3;

        __stoogesort(vec, l, h - t);
        __stoogesort(vec, l + t, h);
        __stoogesort(vec, l, h - t);
    }
}

double _stoogeSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int n = val->size();

    __stoogesort(val, 0, n - 1);

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    if (g_abort)
        return -1;
    return time;
}

int __getNextGap(int gap)
{
    gap = (gap * 10) / 13;

    if (gap < 1)
        return 1;
    return gap;
}

double _combSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int n = val->size();

    int gap = n;

    bool swapped = true;

    while (gap != 1 || swapped == true)
    {
        if (g_abort)
            return -1;
        gap = __getNextGap(gap);
        swapped = false;
        for (int i = 0; i < n - gap; i++)
        {
            if (g_abort)
                return -1;
            if (val->at(i) > val->at(i + gap))
            {
                Value t = val->at(i);
                val->at(i) = val->at(i + gap);
                val->at(i + gap) = t;
                swapped = true;
            }
        }
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    return time;
}

double _cocktailSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int n = val->size();

    bool swapped = true;
    int start = 0;
    int end = n - 1;

    while (swapped)
    {
        if (g_abort)
            return -1;
        swapped = false;

        for (int i = start; i < end; ++i)
        {
            if (g_abort)
                return -1;
            if (val->at(i) > val->at(i + 1))
            {
                Value t = val->at(i);
                val->at(i) = val->at(i + 1);
                val->at(i + 1) = t;
                swapped = true;
            }
        }

        if (!swapped)
            break;

        --end;

        for (int i = end - 1; i >= start; --i)
        {
            if (g_abort)
                return -1;
            if (val->at(i) > val->at(i + 1))
            {
                Value t = val->at(i);
                val->at(i) = val->at(i + 1);
                val->at(i + 1) = t;
                swapped = true;
            }
        }

        ++start;
    }


    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    return time;
}

double _pigeonholeSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;
    int n = val->size();

    Value mi = val->at(0);
    Value ma = val->at(0);

    for (int i = 1; i < n; i++)
    {
        if (g_abort)
            return -1;
        if (val->at(i) < mi)
            mi = val->at(i);
        if (val->at(i) > ma)
            ma = val->at(i);
    }

    int range = ma.GetVal() - mi.GetVal() + 1;

    std::vector<std::vector<Value>> holes(range);

    for (int i = 0; i < n; i++)
    {
        if (g_abort)
            return -1;
        holes[val->at(i).GetVal() - mi.GetVal()].push_back(val->at(i));
    }

    int index = 0;

    for (int i = 0; i < range; i++)
    {
        if (g_abort)
            return -1;
        std::vector<Value>::iterator it;
        for (it = holes[i].begin(); it != holes[i].end(); ++it)
            val->at(index++) = *it;
    }

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;
    return time;
}

double _stdSort(std::vector<Value>* val)
{
    Timer t;
    t.Start();
    double time = 0.0f;

    std::sort(val->begin(), val->end());

    time = t.Stop(Timer::MILLISECONDS) - Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;
    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;

    return time;
}

sf::Color Lerp(const sf::Color& _a, const sf::Color& _b, double f)
{
    sf::Color ret;

    double r = (double)_a.r + f * ((double)_b.r - (double)_a.r);
    double g = (double)_a.g + f * ((double)_b.g - (double)_a.g);
    double b = (double)_a.b + f * ((double)_b.b - (double)_a.b);

    ret.r = (unsigned int)(r + 0.5);
    ret.g = (unsigned int)(g + 0.5);
    ret.b = (unsigned int)(b + 0.5);

    return ret;
}

void Init(std::vector<Value> & val)
{
    sf::Color R(255,   0,   0);
    sf::Color O(255, 165,   0);
    sf::Color Y(255, 255,   0);
    sf::Color G(  0, 255,   0);
    sf::Color B(  0, 165, 255);
    sf::Color I( 75,   0, 130);
    sf::Color V(148,   0, 211);

    int t = val.size() / 6;

    for (int i = 0; i < val.size(); i++)
    {
        if (g_abort)
            return;
        double v = (double)(i) / ((double)val.size() - 1.0);

        double f = 0;

        sf::Color c, low, high;

        if (i > t * 5)
        {
            low = I;
            high = V;

            f = (double)(i - t * 5) / t;
        }
        else if (i > t * 4)
        {
            low = B;
            high = I;
            f = (double)(i - t * 4) / t;
        }
        else if (i > t * 3)
        {
            low = G;
            high = B;
            f = (double)(i - t * 3) / t;
        }
        else if (i > t * 2)
        {
            low = Y;
            high = G;
            f = (double)(i - t * 2) / t;
        }
        else if (i > t * 1)
        {
            low = O;
            high = Y;
            f = (double)(i - t * 1) / t;
        }
        else
        {
            low = R;
            high = O;
            f = (double)(i - t * 0) / t;
        }
        
        c = Lerp(low, high, f);

        val[i].SetIdx(i);
        val[i].SetVal(v * WIN_Y);
        val[i].SetColor(c);
    }
}
