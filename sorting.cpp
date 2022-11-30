#include<random>
#include<iostream>

void create_random_array(int array[], int size)
{
    for(int i = 0; i < size; ++i)
    {
        array[i] = rand() % size;
    }
}

template<typename T> 
void swap( T *a, T *b)
{
    T temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(int array[], int size)
{
    bool swapped = true;
    while(swapped == true)
    {
        // Reset swapped, if no swapping is required
        // then we know the list is sorted        
        swapped = false;
            
        // Traverse array to the second to last element
        // We will check the element ahead of i for comparison
        // so we use size - 1 to avoid overrunning 
        // the end of the array
        for(int i = 0; i < size - 1 ; ++i)
        {
            if( array[i] > array[i+1])
            {
                swap( &array[i], &array[i+1]);
                swapped = true;
            }
        }
    }
}

int main()
{
    const int size = 1000;
    int arr[size];

    create_random_array(arr, size);

    for(int i = 0; i < 10; ++i)
    {
        std::cout << "Random Array at " << i << " has a value of " << arr[i] << std::endl;
    }

    bubble_sort(arr, size);

    std::cout << std::endl;

    for(int i = 0; i < 10; ++i)
    {
        std::cout << "Sorted Array at " << i << " has a value of " << arr[i] << std::endl;
    }

}