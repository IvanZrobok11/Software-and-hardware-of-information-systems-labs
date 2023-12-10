const int ARRAY_SIZE = 500;

template <typename T>
void generateRandomArray(T arr[], int size, T range) {
    randomSeed(analogRead(0)); // Ініціалізація генератора випадкових чисел

    for (int i = 0; i < size; ++i) {
        arr[i] = random(range); // Заповнення масиву випадковими числами в діапазоні [0, range)
    }
}

template <typename T>
void bubbleSort(T arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

template<typename T>
void swap(T* a, T* b) {
    T t = *a;
    *a = *b;
    *b = t;
}

template<typename T>
int partition(T arr[], int low, int high) {
    T pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

template<typename T>
void quickSort(T arr[], int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}


template <typename T>
void printArray(T arr[], int n) {
    Serial.println();
    for (int i = 0; i < n; ++i) {
        Serial.print(arr[i]);
        Serial.print(" ");
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial);
}
template<typename T>
void AlgorithmHandle(int frequency, T range)
{ 
    T *arr 	 = new T[ARRAY_SIZE];
    generateRandomArray(arr, ARRAY_SIZE, range);

    Serial.println("----------------------------------");
    Serial.println(__PRETTY_FUNCTION__);
    //Serial.print("\nДо сортування:");
    //printArray(arr, ARRAY_SIZE);

    setCpuFrequencyMhz(frequency);
    int64_t start_time = esp_timer_get_time();

    quickSort(arr, 0, ARRAY_SIZE - 1);

    int defaultFrequency = 240;
    setCpuFrequencyMhz(defaultFrequency);

    int64_t end_time = esp_timer_get_time();
    int64_t execution_time = end_time - start_time;
    Serial.print("Execution time: ");
	  Serial.print(execution_time);
	  Serial.println(" microseconds");

    Serial.print("Frequency: ");
    Serial.print(frequency);
    Serial.println(" MHz.");
    Serial.println("----------------------------------");
    Serial.println();
    //Serial.print("\nПісля сортування:");
    //printArray(arr, ARRAY_SIZE);

    delete[] arr;
}
void loop() {

    //int
    int frequencies[4] = { 40, 80, 160, 240 };

    for (int i = 0; i < 4; i++) {
      AlgorithmHandle<int>(frequencies[i], 10000);
    }

    //float
    for (int i = 0; i < 4; i++) {
      AlgorithmHandle<float>(frequencies[i], 10000.0f);
    }

    //double
    for (int i = 0; i < 4; i++) {
      AlgorithmHandle<double>(frequencies[i], 10000.0);
    }

    //char
    char charRange = 'Z' - 'A' + 1;
    for (int i = 0; i < 4; i++) {
      AlgorithmHandle<char>( frequencies[i], charRange);
    }

    delay(60000);
}