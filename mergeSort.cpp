#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std::chrono;

using namespace std;

struct movie{
    string title;
    float rating;
};

void merge(vector<movie>& tab, int left, int mid, int right){
    int n1 = mid - left + 1;
    int n2 = right - mid;

    //tworzenie tablic pom
    vector<movie> L(n1), R(n2);

    //kopiowanie danych do tablic pom
    for (int i =0;i<n1;i++) L[i] = tab[left + i];
    for (int i=0;i<n2;i++) R[i] = tab[mid+1+i];

    //scalanie tab pom
    int i=0,j=0,k=left;
    while(i<n1&&j<n2){
        if(L[i].rating<=R[j].rating){
            tab[k] = L[i];
            i++;
        } else {
            tab[k] = R[j];
            j++;
        }
        k++;
    }

    //kopiowanie pozostalych elem l[]
    while(i<n1){
        tab[k] = L[i];
        i++;
        k++;
    }

    //kopiowanie pozostalych elem r[]
    while(i<n2){
        tab[k] = R[j];
        j++;
        k++;
    }
};

void mergeSort(vector<movie>& tab, int left, int right){
    if(left<right) {
        //znalezienie srodka
        int mid = left + (right-left)/2;
        mergeSort(tab,left,mid);
        mergeSort(tab,mid+1,right);
        merge(tab,left,mid,right);
    }}

void printArray(const vector<movie>& tab){
    for (const auto& movie : tab) {
        cout<< movie.title << ": "<<movie.rating<<endl;
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
    int max = 962903;
    vector<int> data_sizes = {10000,100000,500000,750000,max};
    for (int size : data_sizes) {
        auto data = readMoviesFromFile(path,size,max);

        auto start = high_resolution_clock::now();
        mergeSort(data,0,data.size() - 1);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "Czas sortowania dla danych o rozmiarze " << size << ": " << duration.count() << endl;

    }
    return 0;
}

