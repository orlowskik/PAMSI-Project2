#include<iostream>
#include<math.h>
#include<fstream>
#include<string>



/*
    Struktura definiujaca pozycje w rankingu filmow
*/
struct film{
    char name[100];  // Tablica znakowa przechowujaca nazwe filmu
    double grade;    // Ocena danego filmu

    /*
        Konstruktor struktury
    */
    film(){
        grade = 0;  // Przyporzadkowanie 0 pomaga przy pomijaniu blednych wpisow
    }

    /*
        Konstruktor parametryczny struktury
        Parametry:
        [in] char a[] - Tablica znakowa przechowujaca nazwe filmu
        [in] double b - Ocena filmu
    */
    film(char a[], double b){
        int i = 0;
        while(a[i] != EOF){         // Petla uzupelniajaca nazwe filmu
            name[i] = a[i];
            ++i;
        }
        grade = b; // Przypisanie oceny
    }


    bool operator <(const film& a) const;   //  Komparator filmow
};


/*
    Komparator filmow

    Zwraca wartosc TRUE, gdy ocena porownywanego filmu jest wieksza do dwoch liczb po przecinku.
    W przeciwnym przypadku zwraca FALSE.
*/
bool film::operator< (const film& a) const{
    if( this->grade - a.grade  < 0.01)
        return true;
    return false;
}

/*


/*
    Klasa definiujaca tablice przechowujaca filmy

*/
class Film_tab{
    private:
        film *elem;      // Wskaznik na element struktury typu film
        int amount;     // Ilosc danych przechowywanych w tablicy
    public:
        /*
            Konstruktor parametryczny klasy
            Parametry:
            [in] int n - ilosc danych, ktore mozna przechowac w tablicy
        */
        Film_tab(int n) : elem{new film[n]},amount{n}
        {
          for(int i=0; i <n; ++i) elem[i] = film();  // wywolanie konstruktora struktury film dla kazdego elementu tablicy
        }
        ~Film_tab(){delete[] elem;}     // Destruktor klasy zwalniajacy pamiec calej tablicy

    film& operator[](int i) const;  //  Operator pozwalajacy na dostep do komuoek tablicy. Tylko do odczytu
    film& operator[](int i);        //  Operator pozwalajacy na modyfikacje komorek tablicy
    int size() const {return amount;};               //  Funkcja zwracajaca rozmiar tablicy. Tylko do odczytu
};  

/*
int Film_tab::size() const{
    return amount;
}
*/

/*
    Operator modyfikujacy wartosci komorek tablicy
*/
film& Film_tab::operator[](int i){
    if(i >= size() || i < 0){        // Zabezpieczenie przed przekroczeniem zakresu tablicy
        std::cout << "Tablica poza zakresem!!\n";
        exit(1);
    }
    return elem[i];
}

/*
    Operator dostepu do komorek tablicy tylko do odczytu
*/
film& Film_tab::operator[](int i) const{
    if(i >= size() || i < 0){          // Zabezpieczenie przed przekroczeniem zakresu tablicy
        std::cout << "Tablica poza zakresem!!\n";
        exit(1);
    }
    return elem[i];
}


/*
    Funkcja przetwarzajaca dany ciag znakow i dodajaca film do tablicy
    Parametry:
    [in] char a[]      - ciag znakow z rankingu filmow
    [in] Film_tab& tab - referencja do tablicy filmow, gdzie nalezy dodac film
    [in] int n         - pozycja tablicy, na ktorej ma zostac ustawiony film

    [out] true  - jezeli film zostal dodany prawidlowo
    [out] false - w przeciwnym wypadku

*/
bool parse(char a[],Film_tab& tab, int n){
    film* elem = (film*) malloc(sizeof(film)*n);  // Alokowanie pamieci na film
    char number[4];     // Pomocnicza zmienna przechowujaca ocene filmu
    int i = 0, j = 0, k = 0;

    while(a[i] != ',' && a[i] != '\n'){     // Pominiecie numeru wierszu
        ++i;
    }
    i++;
    if(a[i] == '"'){    // W przypadku gdy podany jest serial (nazwa w cudzyslowie)
        ++i;
        while(a[i] != '"'){         // Uzupelnia nazwe filmu az nie napotka konca
            elem->name[j] = a[i];
            ++i;
            ++j;
        }
        ++i;
    }
    while(a[i] != ',' && a[i] != '\n'){ // Jezeli podana nazwa bez cudzyslowia
        elem->name[j] = a[i];
        ++i;
        ++j;
    }
    ++i;
    while(a[i] != ',' && a[i] != '\n' && k <= 3){   // Wczytanie oceny filmu, ograniczone do trzech znakow
        number[k] = a[i];
        ++i;
        ++k;
    }

    elem->grade = atof(number);     // Zamiana oceny na liczbe i przypisanie
    if(elem->grade != 0){           // Jezeli prawidlowo podao ocene dodaje rekord do tablicy
        tab[n] = *elem;
        return true;
    }


    return false;   // W przeciwnym przypadku
}


const int N = 10;  // Ilosc wczytywanych filmow
Film_tab pom(N);   // Tablica pomocniczna uzuwana przy sortowaniu przez scalanie


/*
    Algorytm scalania tablic z filmami wykorzystany w sortowaniu przez scalanie
    Parametry:
    [in] Film_tab& base - referencja do tablicy przeznaczonej do sortowania
    [in] int begin      - pierwsza komorka tablicy
    [in] int mid        - srodek tablicy
    [in] int end        - koniec tablicy

*/
void merge(Film_tab& base,int begin, int mid, int end){
    int i,j;
    for(int i = begin; i<= end; i++)    // przepisanie tablicy
        pom[i] = base[i];
    i = begin; j = mid+1;   // ustawienie wartosci do przeszukiwania tablicy

    for(int k = begin; k <= end; k++){  // Przechodzi cala podana tablice
        if(i <= mid){
            if(j <= end){
                base[k] = (pom[j] < pom[i]) ? pom[j++] : pom[i++];  
            }
            else{
                base[k] = pom[i++];
            }
        }
        else{
            base[k] = pom[j++];
        }
    }
}

void merge_sort(Film_tab& base, int begin, int end){
    int mid;

    if(end <= begin) return;
    mid = (begin+end)/2;
    merge_sort(base,begin,mid);
    merge_sort(base,mid+1,end);
    merge(base,begin,mid,end);
}


void quick_sort(Film_tab& base,int left, int right){
    film tmp;
    if(right <= left) return;

    int i = left-1, j = right + 1;
    int pivot = base[(left+right)/2].grade;

    while(1){

        while(pivot > base[++i].grade);

        while(pivot < base[--j].grade);

        if(i <= j){
            tmp = base[i];
            base[i] = base[j];
            base[j] = tmp;
        }
        else
            break;
    }

    if(j > left)
        quick_sort(base, left, j);
    if(i < right)
        quick_sort(base,i,right);
}


void shell_sort(Film_tab& base){
    film tmp;
    int j;
    int gap = base.size()/2;

    while(gap >= 1){
        for(int i = gap; i < base.size(); ++i){
            tmp = base[i];
            for(j = i; j >= gap && base[j-gap].grade > tmp.grade; j-= gap){
                base[j] = base[j-gap];
            }
            base[j] = tmp;
        }
        gap /= 2;
    }
}




int main(){
    char line[300];
    std::ifstream file("dane2.csv");
    int n = 0;
    Film_tab tmp(N);

    if(file.is_open()){
        while(file.getline(line,300) && n < tmp.size()){
            if(parse(line,tmp,n))
                ++n;

        };
    }

    std::cout << "Przed sortowaniem: \n";
    for(int i=0; i < tmp.size();++i){
        std::cout << i << " Film: " << tmp[i].name << " Ocena: " << tmp[i].grade << std::endl;
    }

    std::cout << "\n\n\n" ;
    std::cout << "Po sortowaniu: \n";
    //merge_sort(tmp,0,tmp.size()-1);
    //quick_sort(tmp,0,tmp.size()-1);
    shell_sort(tmp);

    for(int i=0; i < tmp.size();++i){
        std::cout << i << " Film: " << tmp[i].name << " Ocena: " << tmp[i].grade << std::endl;
    }


    return 0;
}