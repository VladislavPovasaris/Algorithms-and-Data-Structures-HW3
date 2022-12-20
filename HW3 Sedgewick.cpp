#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

string working_file_for_random = "Data1.txt"; // This global variable is created to change the files for fstream for random numbers faster.File names are: Data[n] n from 1 to 5
string working_file_for_almost_sorted = "Data11.txt"; // This global variable is created to change the files for fstream for almost sorted numbers faster.File names are: Data[nn] n from 1 to 5;

template <class T>
inline void shellsort_pass(T* data, int size, int h)
{
    int j;

    for (int i = h; i < size; i++) 
    {

        // insert element i into the slice data[i-h], data[i-2h], ...
        T temp = data[i];

        for (j = i; j - h >= 0 && temp < data[j - h]; j -= h) {

            data[j] = data[j - h];
        }

        data[j] = temp;
    }
}

template <class T>
void ShellSortSedgewick(T* data, int size)
{
    int x;

    // find the value of x that oversteps size and backtrack one
    for (x = 0; pow(4.0, x + 1) + 3 * pow(2.0, x) + 1 < size; x++);
    x--;

    int h;

    for (; x >= -1; x--)
    {
        if (x == -1)
            h = 1;
        else
            h = pow(4.0, x + 1) + 3 * pow(2.0, x) + 1;
        shellsort_pass(data, size, h);
    }
}
void heapify(int* arr, int n, int i) 
{
    // Find largest among root, left child and right child
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    // Swap and continue heapifying if root is not largest
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// main function to do heap sort
void HeapSort(int * arr, int n) 
{
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Heap sort
    for (int i = n - 1; i >= 0; i--) 
    {
        swap(arr[0], arr[i]);

        // Heapify root element to get highest element at root again
        heapify(arr, i, 0);
    }
}

void GenerateRandomData(int arr_size)
{
    int* array = new int[arr_size];
    ofstream out;
    out.open(working_file_for_random);
    srand(202129251123111); // must be changed manually to get different set of numbers. I used my student ID and added few additional digits
    for (int i = 0; i < arr_size; i++)
    {
        array[i] = rand() % 100000;
        out << array[i] << " ";
    }
    out.close();
    delete[] array;
}
void GenerateAlmostSortedData(int arr_size)
{
    vector<int> vect;
    ofstream out;
    out.open(working_file_for_almost_sorted);
    srand(2021292544412123); // must be changed manually to get different set of numbers. I used my student ID and added few additional digits
    for (int i = 0; i < arr_size; i++)
    {
        vect.push_back(rand() % 100000);
    }
    sort(vect.begin(), vect.begin() + (vect.size() - vect.size() / 10)); // sorting 90% of vector
    int random_shift = rand() % 5;
    if (random_shift == 1)
    {
        random_shift++;
    }
    else if (random_shift == 0)
    {
        random_shift += 2;
    }
    else
    {
        random_shift = 3;
    }
    random_shuffle(vect.begin() + ceil(vect.size() / random_shift), vect.end() - ceil(vect.size() / 2.5)); // additionaly shuffling vector starting from ceil(vect.size() / random_shift) ((~from 33 to 50%)) element till the 60% * size element 
    for (auto i : vect)
    {
        out << i << " ";
    }
    out.close();
}

void ReadRandomData(int arr_size, int* array_to_sort)
{
    ifstream in;
    in.open(working_file_for_random);
    string temp;
    for (int i = 0; i < arr_size; i++)
    {
        getline(in, temp, ' ');
        array_to_sort[i] = stoi(temp);
    }
    in.close();
}
void ReadData(int arr_size, int* array_to_sort)
{
    ifstream in1;
    in1.open(working_file_for_almost_sorted);
    string temp1;
    for (int i = 0; i < arr_size; i++)
    {
        getline(in1, temp1, ' ');
        array_to_sort[i] = stoi(temp1);
    }
    in1.close();
}

void display(int arr_size, int* array_to_sort)
{
    for (int i = 0; i < arr_size; i++)
    {
        cout << array_to_sort[i] << " ";
    }
}

int main() 
{
    //Study #1
    const int size = 100000; // with this variable we change the size of data for random numbers data
    int* array_to_sort_shell_edgewick = new int[size];
    int* array_to_sort_heap = new int[size];
    GenerateRandomData(size); // since numbers are already generated, this line might be commented as data is already present in the file
    ReadRandomData(size, array_to_sort_shell_edgewick);
    ReadRandomData(size, array_to_sort_heap);

    auto begin_sedgewick = std::chrono::high_resolution_clock::now();
    ShellSortSedgewick(array_to_sort_shell_edgewick, size);
    auto end_sedgewick = std::chrono::high_resolution_clock::now();
    auto elapsed_sedgewick = std::chrono::duration_cast<std::chrono::milliseconds>(end_sedgewick - begin_sedgewick);

    auto begin_heap = std::chrono::high_resolution_clock::now();
    HeapSort(array_to_sort_heap, size);
    auto end_heap = std::chrono::high_resolution_clock::now();
    auto elapsed_heap = std::chrono::duration_cast<std::chrono::milliseconds>(end_heap - begin_heap);

    //display(size, array_to_sort_shell_edgewick); // in case you want to check if array is sorted
    cout << endl << "For the size of data N=" << size << " :" << endl;
    cout << "Random numbers:    Shell with Sedgewick: " << elapsed_sedgewick.count() << "   " << "Heap : " << elapsed_heap.count() << endl;
    delete[] array_to_sort_shell_edgewick;
    delete[] array_to_sort_heap;

    //Study #2
    const int size_for_almost_sorted = 100000; // with this variable we change the size of data for almost sorted numbers. Must be dividable by 10.
    int* array_to_sort_sedgewick_2 = new int[size_for_almost_sorted];
    int* array_to_sort_heap_2 = new int[size_for_almost_sorted];
    GenerateAlmostSortedData(size_for_almost_sorted);
    ReadData(size_for_almost_sorted, array_to_sort_sedgewick_2);
    ReadData(size_for_almost_sorted, array_to_sort_heap_2);

    auto begin_sedgewick_for_almost_sorted = std::chrono::high_resolution_clock::now();
    ShellSortSedgewick(array_to_sort_sedgewick_2, size_for_almost_sorted);
    auto end_sedgewick_for_almost_sorted = std::chrono::high_resolution_clock::now();
    auto elapsed_sedgewick_for_almost_sorted = std::chrono::duration_cast<std::chrono::milliseconds>(end_sedgewick_for_almost_sorted - begin_sedgewick_for_almost_sorted);

    auto begin_heap_for_almost_sorted = std::chrono::high_resolution_clock::now();
    HeapSort(array_to_sort_heap_2, size_for_almost_sorted);
    auto end_heap_for_almost_sorted = std::chrono::high_resolution_clock::now();
    auto elapsed_heap_for_almost_sorted = std::chrono::duration_cast<std::chrono::milliseconds>(end_heap_for_almost_sorted - begin_heap_for_almost_sorted);

    cout << endl << "For the size of data N=" << size_for_almost_sorted<< " :" << endl;
    cout << "Almost sorted numbers:    Shell with Sedgewick: " << elapsed_sedgewick_for_almost_sorted.count() << "   " << "Heap : " << elapsed_heap_for_almost_sorted.count() << endl;
    delete[] array_to_sort_sedgewick_2;
    delete[] array_to_sort_heap_2;
   
}
