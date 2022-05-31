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


const int N = 100000;  // Ilosc wczytywanych filmow
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
        if(i <= mid){                   // Jezeli istnieja wpisy w lewej podtablicy 
            if(j <= end){               // Jezeli istnieja wpisy w prawej podtablicy
                base[k] = (pom[j] < pom[i]) ? pom[j++] : pom[i++];  // Przepisywanie kolejnych liczb z lewej i prawej podtablicy 
            }
            else{
                base[k] = pom[i++];         // Jezeli prawa pusta przepisz lewa
            }
        }
        else{
            base[k] = pom[j++];         // Jezeli lewa pusta przepisz prawa
        }
    }
}


/*
    Algorytm sortowania tablicy przez scalenie
    Parametry:
    [in] Film_tab& base - referencja do tablicy przeznaczonej do sortowania
    [in] int begin      - pierwsza komorka tablicy
    [in] int end        - koniec tablicy

*/
void merge_sort(Film_tab& base, int begin, int end){
    int mid;

    if(end <= begin) return;    // jednoelementowa tablica jest posortowana
    mid = (begin+end)/2;        // okreslenie srodka tablicy
    merge_sort(base,begin,mid); // Rekurencyjny podzial na dwie tablice (z lewej strony)
    merge_sort(base,mid+1,end); // Rekurencyjny podzial na dwie tablice (z prawej strony)
    merge(base,begin,mid,end);  // Wykorzystanie funkcji scalajacej
}




/*
    Algorytm sortowania szybkiego tablicy 
    Parametry:
    [in] Film_tab& base - referencja do tablicy przeznaczonej do sortowania
    [in] int left       - pierwsza komorka tablicy
    [in] int right      - koniec tablicy

*/
void quick_sort(Film_tab& base,int left, int right){
    film tmp;                   // zmienna tymczasowa potrzebna do zamiany rekordow miejscami
    if(right <= left) return;   // jednoelementowa tablica jest posortowana

    int i = left-1, j = right + 1;
    int pivot = base[(left+right)/2].grade; // jako pivot wybierany jest srodkowy element

    while(1){

        while(pivot > base[++i].grade); // pierwszy element wiekszy lub rowny pivotowi od lesej

        while(pivot < base[--j].grade); // pierwszy element mniejszy lub rowny od prawej

        if(i <= j){           // jezeli liczniki sie nie minely zamieniamy rekordy miejscami
            tmp = base[i];
            base[i] = base[j];
            base[j] = tmp;
        }
        else
            break;          // przerywamy petle dopiera po uzyskaniu dwoch tablic (mniejszej i wiekszej od pivota)
    }

    if(j > left)                // wykonujemy operacje na prawej tablicy 
        quick_sort(base, left, j);
    if(i < right)               // wykonujemy operacje na lewej tablicy
        quick_sort(base,i,right);
}



/*
    Algorytm sortowania szybkiego tablicy 
    Parametry:
    [in] Film_tab& base - referencja do tablicy przeznaczonej do sortowania

*/
void shell_sort(Film_tab& base){
    film tmp;       // zmienna tymczasowa potrzebna do zamiany rekordow miejscami 
    int j;  
    int gap = base.size()/2;    // wyznaczenie przerwy w sposob oryginalny ( zaproponowany przez Shell'a)

    while(gap >= 1){            // dopoki jest to mozliwe
        for(int i = gap; i < base.size(); ++i){     
            tmp = base[i];
            for(j = i; j >= gap && base[j-gap].grade > tmp.grade; j-= gap){ //dla kazdego co g-tego elementu przesuwaj go do tylu az powstanie posortowane wyrazenie
                base[j] = base[j-gap];
            }
            base[j] = tmp;
        }
        gap /= 2;       //aktualizacja przerwy
    }
}



/*  
    Funkcja obliczajaca srednia z ocen tablicy
    Parametry:
    [in] Film_tab& table - referencja do tablicy z filmami
    [out] double mean    - srednia z ocen filmow zawartych w tablicy 
*/
double mean(Film_tab& table){
    double mean = 0;
    for(int i = 0; i < table.size() ; ++i){
        mean += table[i].grade;
    }

    return mean / table.size();
}

/*  
    Funkcja obliczajaca mediane z ocen tablicy posortowanej
    Parametry:
    [in] Film_tab& table - referencja do posortowanej tablicy z filmami
    [out] double median  - mediana z ocen filmow zawartych w tablicy 
*/
double median(Film_tab& table){
    if(table.size() == 0)   // pusta tablica zwraca 0
        return 0;
    if(table.size() % 2 == 0)       // mediana jest srednia wartosci srodkowych
        return (table[table.size()/2].grade + table[table.size()/2 - 1].grade )/2;
    else
        return table[table.size()/2].grade; // mediana jest wartoscia srodkowa
}

int main(){
    char line[300];
    std::ifstream file("dane.csv");
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

    std::cout << "Mediana: " << median(tmp) << std::endl;
    std::cout << "Srednia: " << mean(tmp) << std::endl;
    

    return 0;
}