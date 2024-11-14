using namespace std;
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <algorithm>


struct FileData {
    std::vector<int> numbers;
    std::string path;
    std::string name;
};

struct OrdenationData
{
    std::vector<int> ordenedList;
    std::chrono::duration<double> time;
};

FileData openFile(std::filesystem::directory_entry entry) {
    FileData file = FileData();
    std::ifstream arquivo(entry.path());
    if (arquivo.is_open()) {
        std::string linha;
        while (std::getline(arquivo, linha)) {
            try {
                if(linha == " " || linha == "") {
                    continue;
                }
                file.path = entry.path();
                file.name = entry.path().filename();
                file.numbers.push_back(std::stoi(linha));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Erro ao converter para int: " << linha << std::endl;
            }
        }
        arquivo.close();
    } else {
        std::cerr << "Não foi possível abrir o arquivo: " << entry.path() << std::endl;
    }
    return file;
}

std::vector<FileData> openRecursiveFiles(const string& folderPath) {
    std::vector<FileData> lists;

    try {
        if (!filesystem::exists(folderPath) || !filesystem::is_directory(folderPath)) {
            std::cerr << "Diretório inválido: " << folderPath << std::endl;
            return std::vector<FileData>{};
        }

        for (const auto& entry : filesystem::recursive_directory_iterator(folderPath)) {
            if (filesystem::is_regular_file(entry)) {
                FileData file = openFile(entry);
                lists.push_back(file);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao acessar o diretório: " << e.what() << std::endl;
    }
    return lists;
}

std::vector<int> generateRandomData(int size) {
    std::vector<int> data;
    for (int i = 0; i < size; ++i) {
        data.push_back(rand() % 10000);
    }
    return data;
}

void showBestAlgorithm(
    float sortedBubbleTotalTime, 
    float sortedInsertionTotalTime, 
    float sortedSelectionTotalTime) 
{
    float melhorTempo = std::min({sortedBubbleTotalTime, sortedInsertionTotalTime, sortedSelectionTotalTime});

    std::cout << "\nAnálise de desempenho:" << std::endl;
    if (melhorTempo == sortedBubbleTotalTime) {
        std::cout << "Bubble Sort teve o melhor desempenho!" << std::endl;
    } else if (melhorTempo == sortedInsertionTotalTime) {
        std::cout << "Insertion Sort teve o melhor desempenho!" << std::endl;
    } else {
        std::cout << "Selection Sort teve o melhor desempenho!" << std::endl;
    }
    std::cout << "Diferença de tempo entre:" << std::endl;
    std::cout << "Bubble vs Insertion: " 
        << std::abs(sortedBubbleTotalTime - sortedInsertionTotalTime) << " segundos" << std::endl;
    std::cout << "Bubble vs Selection: " 
        << std::abs(sortedBubbleTotalTime - sortedSelectionTotalTime) << " segundos" << std::endl;
    std::cout << "Insertion vs Selection: " 
        << std::abs(sortedInsertionTotalTime - sortedSelectionTotalTime) << " segundos" << std::endl;

}

OrdenationData bubbleSort(std::vector<int>& arr) {

    auto init = std::chrono::high_resolution_clock::now();

    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool changed = false;
        int limit = n - i - 1;
        for (int j = 0; j < limit; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                changed = true;
            }
        }
        if (!changed) { 
            break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end - init;
    
    return OrdenationData{
        .ordenedList = arr,
        .time = time
    };
}

OrdenationData insertionSort(std::vector<int>& arr) {
    auto init = std::chrono::high_resolution_clock::now();

    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end - init;
    
    return OrdenationData{
        .ordenedList = arr,
        .time = time
    };
}

OrdenationData selectionSort(std::vector<int>& arr) {
    auto init = std::chrono::high_resolution_clock::now();

    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end - init;
    
    return OrdenationData{
        .ordenedList = arr,
        .time = time
    };
}   

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <diretório com arquivos de dados>" << std::endl;
        return 1;
    }

    std::string folderPath = argv[1];
    std::vector<FileData> lists = openRecursiveFiles(folderPath);

    FileData randomDataFile;
    randomDataFile.numbers = generateRandomData(1000);
    randomDataFile.path = "Interno";
    randomDataFile.name = "Aleatório Gerado Internamente";
    lists.push_back(randomDataFile);

    double sortedBubbleTotalTime = 0;
    double sortedInsertionTotalTime = 0;
    double sortedSelectionTotalTime = 0;

    for(int i = 0; i < lists.size(); i++) {
        std::vector<int> numbersForBubble = lists[i].numbers;
        std::vector<int> numbersForInsertion = lists[i].numbers;
        std::vector<int> numbersForSelection = lists[i].numbers;

        OrdenationData sortedBubble = bubbleSort(numbersForBubble);
        sortedBubbleTotalTime += sortedBubble.time.count();

        OrdenationData sortedInsertion = insertionSort(numbersForInsertion);
        sortedInsertionTotalTime += sortedInsertion.time.count();
        
        OrdenationData sortedSelection = selectionSort(numbersForSelection);
        sortedSelectionTotalTime += sortedSelection.time.count();
    }

    std::cout << "Tempo de execução Bubble: " << sortedBubbleTotalTime << " segundos" << std::endl;
    std::cout << "Tempo de execução Insertion: " << sortedInsertionTotalTime << " segundos" << std::endl;
    std::cout << "Tempo de execução Selection: " << sortedSelectionTotalTime << " segundos" << std::endl;

    showBestAlgorithm(sortedBubbleTotalTime, sortedInsertionTotalTime, sortedSelectionTotalTime);

    return 0;
}
