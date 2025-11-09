#include <iostream>
using namespace std;

// Manual sentinel value for error signaling (must be consistent with client code)
// Assumes valid data in the heap is non-negative (>= 0).
const int SENTINEL_ERROR_VALUE = -1;

// Manual swap function
template<class DT>
void manual_swap(DT& a, DT& b) {
    DT temp = a;
    a = b;
    b = temp;
}

template<class DT>
class MinHeap
{
public:
    // Constructor
    MinHeap(int maxsize);
    
    // Inserts data
    bool insert(const DT data);
    
    // Removes the root (minimum element). Returns SENTINEL_ERROR_VALUE on error.
    DT deleteMin();
    
    // Prints contents
    void printContents();
    
    // Destructor
    ~MinHeap();

private:
    DT *arr;
    int capacity;
    int size;

    // Helper functions
    void heapifyUp(int index);
    void heapifyDown(int index);
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i + 1); }
    int rightChild(int i) { return (2 * i + 2); }
};

// ===================================================
// MinHeap Implementation
// ===================================================

template<class DT>
MinHeap<DT>::MinHeap(int maxsize) : capacity(maxsize), size(0)
{
    if (maxsize <= 0) {
        cerr << "Warning: MinHeap size must be positive. Setting to default 10." << endl;
        capacity = 10;
    }
    arr = new DT[capacity];
}

template<class DT>
MinHeap<DT>::~MinHeap()
{
    delete[] arr;
}

template<class DT>
bool MinHeap<DT>::insert(const DT data)
{
    if (size == capacity) {
        cout << "Heap is full! Cannot insert " << data << endl;
        return false;
    }
    arr[size] = data;
    size++;
    heapifyUp(size - 1);
    return true;
}

template<class DT>
void MinHeap<DT>::heapifyUp(int index)
{
    while (index != 0 && arr[parent(index)] > arr[index]) { // MIN-HEAP condition
        manual_swap(arr[index], arr[parent(index)]);
        index = parent(index);
    }
}

// Specialization for int: returns SENTINEL_ERROR_VALUE on error
template<>
int MinHeap<int>::deleteMin()
{
    if (size <= 0) {
        // Print error message directly (replaces throwing an exception)
        cerr << "Error: Heap is empty! Returning SENTINEL_ERROR_VALUE." << endl;
        return SENTINEL_ERROR_VALUE;
    }
    if (size == 1) {
        size--;
        return arr[0];
    }

    int root = arr[0];
    arr[0] = arr[size - 1];
    size--;
    heapifyDown(0);
    return root;
}

template<class DT>
void MinHeap<DT>::heapifyDown(int index)
{
    int smallest = index;
    int l = leftChild(index);
    int r = rightChild(index);

    // Find the smallest among the node and its children
    if (l < size && arr[l] < arr[smallest]) { // MIN-HEAP condition
        smallest = l;
    }
    if (r < size && arr[r] < arr[smallest]) { // MIN-HEAP condition
        smallest = r;
    }

    if (smallest != index) {
        manual_swap(arr[index], arr[smallest]);
        heapifyDown(smallest);
    }
}

template<class DT>
void MinHeap<DT>::printContents()
{
    cout << "Heap Contents (Array Order): ";
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << (i == size - 1 ? "" : ", ");
    }
    cout << endl;
}
// The SENTINEL_ERROR_VALUE constant is pulled in from MinHeap.h

// --- Helper function for robust input without using std::limits ---
void clearInputBuffer() {
    // Clears error flags on cin
    cin.clear(); 
    
    // Manually discards characters until a newline is found, 
    // ensuring no bad input pollutes the next read operation.
    while (cin.get() != '\n'); 
}

// --- Main Airport System Logic ---
void runAirportSystem()
{
    // MinHeap is used for priority queue: lower fuel (value) = higher priority
    MinHeap<int> *landingQueue = new MinHeap<int>(100);

    cout << "Welcome to Lahore airport civil aviation landing system" << endl;
    char response_char;

    // --- Input Loop for Aircraft Insertion ---
    while (true)
    {
        cout << "Is there an aircraft in airspace requesting to land? (Y/N):" << endl;
        if (!(cin >> response_char)) {
            // Handle fatal input error (like EOF)
            break;
        }

        // Manual case conversion
        if (response_char >= 'a' && response_char <= 'z') {
            response_char = response_char - ('a' - 'A');
        }

        if (response_char == 'Y')
        {
            int fuel_units = SENTINEL_ERROR_VALUE;
            cout << "Please enter the amount of fuel present in the aircraft:" << endl;

            // Input validation loop: check for successful read AND non-negative value
            while (!(cin >> fuel_units) || fuel_units < 0) {
                cout << "Invalid input. Please enter a non-negative integer for fuel:" << endl;
                clearInputBuffer(); 
            }

            // Insert fuel units (which are guaranteed to be >= 0)
            landingQueue->insert(fuel_units);
        }
        else if (response_char == 'N')
        {
            cout << "No more aircraft requesting to land." << endl;
            break;
        }
        else
        {
            cout << "Invalid response. Please answer 'Y' or 'N'." << endl;
            clearInputBuffer();
        }
    }

    // --- Deletion Loop for Landing Access ---
    cout << "\nGrant access of runway to the aircrafts in the following order:" << endl;

    while (true)
    {
        // deleteMin extracts the lowest fuel (highest priority)
        int granted_fuel = landingQueue->deleteMin();

        // Check for the sentinel value to determine if the queue is empty
        if (granted_fuel == SENTINEL_ERROR_VALUE) {
            // MinHeap::deleteMin already prints an error message to cerr if empty
            break;
        }
        
        cout << "Aircraft with " << granted_fuel << " units of fuel" << endl;
    }

    // Clean up dynamic memory
    delete landingQueue;
}

// --- Main Function ---
int main()
{
    runAirportSystem();
    return 0;
}