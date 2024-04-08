#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct movie{
    string title;
    float rating;
};

void bucketSort(vector<movie>& arr){
    //tworzenie kubelkow
    const int n = arr.size();
    vector<vector<movie>> buckets(n);

    //umieszczenie elementow w odpowiednich kubelkach
    for(const auto&movie : arr){
        int bucket_index=min(static_cast<int>(movie.rating*(n-1)),n-1);
        buckets[bucket_index].push_back(movie);
    }

    //sortowanie kubelkow i scalanie ich
    for(auto& bucket : buckets){
        sort(bucket.begin(), bucket.end(),[](const movie& a, const movie& b){
            return a.rating < b.rating;
        });
    }

    int index=0;
    for( const auto& bucket : buckets){
        for(const auto& movie : bucket){
            arr[index++] = movie;
        }
    }
}
    void printArray(const vector<movie>& tab) {
        for(const auto& movie: tab){
            cout<<movie.title<<": "<<movie.rating<<endl;
        }
        cout<<endl;
    }

vector<movie> readMoviesFromFile(const string& filename, int num_movies, int max) {
    vector<movie> movies;
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
    int max = 962903;
    vector<int> data_sizes = {10000,100000,500000,750000,max};

    for (int size : data_sizes) {
        auto data = readMoviesFromFile(path,size,max);

        auto start = high_resolution_clock::now();
        bucketSort(data);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Czas sortowania dla danych o rozmiarze " << size << ": " << duration.count() << endl;
        if (!data.empty()) {
            float median;
            if (size % 2 == 0) {
                median = (data[size / 2 - 1].rating + data[size / 2].rating) / 2.0f;
            } else {
                median = data[size / 2].rating;
            }

            float sum = 0.0f;
            for (const auto& movie : data) {
                sum += movie.rating;
            }
            float average = sum / size;

            cout << "Mediana dla danych o rozmiarze " << size << ": " << median << endl;
            cout << "Średnia dla danych o rozmiarze " << size << ": " << average << endl;
        } else {
            cout << "Brak danych do obliczenia mediany i średniej dla rozmiaru " << size << endl;
        }
    }

    return 0;
}