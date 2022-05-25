#include<iostream>
#include<math.h>
#include<fstream>
#include<string>




struct film{
    char name[100];
    double grade;

    film(){
        grade = 0;
    }

    film(char a[], double b){
        int i = 0;
        while(a[i] != EOF){
            name[i] = a[i];
            ++i;
        }
        grade = b;
    }


    bool operator <(const film& a) const;
};

bool film::operator< (const film& a) const{
    if( this->grade - a.grade  < 0.01)
        return true;
    return false;
}


class Film_tab{
    private:
        film *elem; 
        int amount;
    public:
        Film_tab(int n) : elem{new film[n]},amount{n}
        {
          for(int i=0; i <n; ++i) elem[i] = film();
        }
        ~Film_tab(){delete[] elem;}

    film& operator[](int i) const;
    film& operator[](int i);
    int size() const;
};  


int Film_tab::size() const{
    return amount;
}

film& Film_tab::operator[](int i){
    if(i >= size()){
        std::cout << "Tablica poza zakresem!!\n";
        exit(1);
    }


    return elem[i];
}

film& Film_tab::operator[](int i) const{
    if(i >= size()){
        std::cout << "Tablica poza zakresem!!\n";
        exit(1);
    }

    return elem[i];
}



bool parse(char a[],Film_tab& tab, int n){
    film* elem = (film*) malloc(sizeof(film)*n);
    char number[4];
    int i = 0, j = 0, k = 0;

    while(a[i] != ',' && a[i] != '\n'){
        ++i;
    }
    i++;
    if(a[i] == '"'){
        ++i;
        while(a[i] != '"'){
            elem->name[j] = a[i];
            ++i;
            ++j;
        }
        ++i;
    }
    while(a[i] != ',' && a[i] != '\n'){
        elem->name[j] = a[i];
        ++i;
        ++j;
    }
    ++i;
    while(a[i] != ',' && a[i] != '\n' && k <= 3){
        number[k] = a[i];
        ++i;
        ++k;
    }

    elem->grade = atof(number);
    if(elem->grade != 0){
        tab[n] = *elem;
        return true;
    }


    return false;
}


const int N = 1000000;
Film_tab pom(N);

void merge(Film_tab& base,int begin, int mid, int end){
    int i,j;
    for(int i = begin; i<= end; i++)
        pom[i] = base[i];
    i = begin; j = mid+1;

    for(int k = begin; k <= end; k++){
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