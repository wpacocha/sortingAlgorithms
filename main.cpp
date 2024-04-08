#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std::chrono;
using namespace std;

struct movie {
    string title;
    float rating;
};

void quickSort(vector<movie>& arr, int low, int high) {
    if (low >= high)
        return;

    float pivot = arr[(low + high) / 2].rating;
    int i = low;
    int j = high;

    while (i <= j) {
        while (arr[i].rating < pivot) {
            i++;
        }
        while (arr[j].rating > pivot) {
            j--;
        }
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }

    quickSort(arr, low, j);
    quickSort(arr, i, high);
}


void printArray(const vector<movie>& tab){
    for(const auto& movie : tab) {
        cout<<movie.title<<": "<<movie.rating<<endl;
    }
    cout<<endl;
}

vector<movie> readMoviesFromFile(const string& filename, int num_movies, int max) {
    vector<movie> movies;
    if (num_movies>=max) num_movies=max;
    ifstream file(filename);
    if(!file.is_open()) {
        cerr << "Nie mogę otworzyć pliku: " << filename << endl;
        return movies;
    }

    string line,title2,rating2;
    getline(file,line);
    int i =0;
    while (getline(file, line) && i < num_movies) {
        size_t coma = line.find_last_of(",");
        if (coma != string::npos) {
            string title2 = line.substr(0, coma);
            string rating2 = line.substr(coma + 1);

            // Jeśli ocena nie jest pusta, dodaj film do wektora
            if (!rating2.empty()) {
                movie m;
                m.title = title2;
                try {
                    m.rating = stof(rating2);
                    movies.push_back(m);
                    i++;
                } catch (const invalid_argument& e) {
                    cerr << "Błąd parsowania oceny: " << e.what() << endl;
                }
            }
        }
    }
    file.close();
    return movies;
}


int main() {
    string path = "/Users/wpacocha/Studia/Semestr IV/PAA/MergeSort/dane.csv";
    int max = 962902;
    vector<int> data_sizes = {10000,100000,500000,750000,max};

    for (int size : data_sizes) {
        auto data = readMoviesFromFile(path,size,max);

        auto start = high_resolution_clock::now();
        quickSort(data,0,data.size() - 1);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Czas sortowania dla danych o rozmiarze " << size << ": " << duration.count() << endl;

    }
    return 0;
}
