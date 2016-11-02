#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <ctime>


using namespace std;

class City
{
    public:
        City();
        City(double x, double y);

        void setX(double x);
        void setY(double y);
        double getX() const;
        double getY() const;

        static double dist(City &p, City &q);

    private:
        double x;
        double y;
};

City::City() : x(0), y(0)
{}

City::City(double x, double y) : x(x), y(y)
{}

void City::setX(double x)
{
    this->x = x;
}

void City::setY(double y)
{
    this->y = y;
}

double City::getX() const
{
    return this->x;
}

double City::getY() const
{
    return this->y;
}

double City::dist(City &p, City &q)
{
    double dx = p.x - q.x;
    double dy = p.y - q.y;
    double result = sqrt(dx*dx + dy*dy);
    return result;
}





int find_nearest_neighbour(int indexOfCity, vector<City> &cityList, vector<int> used){
    int best_neighbour_index = -1;
    for(unsigned int j=0; j<cityList.size(); j++){
        if(used[j]==0 && (best_neighbour_index==-1 || City::dist(cityList[indexOfCity], cityList[j]) < City::dist(cityList[indexOfCity], cityList[best_neighbour_index]) ) ){
            best_neighbour_index = j;
        }
    }
    return best_neighbour_index;
}

int pick_unvisited_city(vector<int> used){
    int numberOfCities = used.size();
    int index = rand() % numberOfCities;
    while(used[index] == 1){
        index = rand() % numberOfCities;
    }
    return index;
}

double insertion_cost(City city1, City city2, City city3){
    return City::dist(city1, city3) + City::dist(city3, city2) - City::dist(city1, city2);
}


// new_city doesnt belong to tour
// return the position in which new_city will be inserted in tour that will minimize the insertion cost for the tour
int find_minimum_triangular_distance_edge(int new_city, vector<int> tour, vector<City> city_list){
    int index_to_insert;
    unsigned int number_visited_cities(tour.size());
    double min_cost = 99999;
    double current_cost;
    for(unsigned int p=0; p<number_visited_cities-1; p++){
            current_cost = insertion_cost(city_list[tour[p]], city_list[tour[p+1]], city_list[new_city]);
            if(current_cost<min_cost){
                min_cost = current_cost;
                index_to_insert = p+1;
            }
    }
    return index_to_insert;
}

vector<int>  select_shortest_edge(vector<City> city_list){
    vector<int> shortest_edge;
    double min_edge = 99999;
    double current_edge;
    int index_i;
    int index_j;
    for(unsigned int i=0; i<city_list.size()-1; i++){
        for(unsigned int j=i+1; j<city_list.size(); j++){
            current_edge = City::dist(city_list[i],city_list[j]);
            if(current_edge < min_edge){
                min_edge = current_edge;
                index_i = i;
                index_j = j;
            }
        }
    }
    shortest_edge.push_back(index_i);
    shortest_edge.push_back(index_j);

    return shortest_edge;
}

vector<int> generate_random_tour(vector<City> city_list){
    vector<int> random_tour;
    vector<int> used(city_list.size());
    int index = rand() % city_list.size();
    while(random_tour.size() != city_list.size()){
        if(used[index] ==0){
            random_tour.push_back(index);
            used[index] = 1;
        }
        index = rand() % city_list.size();
    }
    return random_tour;
}

vector<int> swap_edge(vector<int> tour, int i, int j){
    vector<int> new_tour;

    for(int p=0; p<i; p++){
        new_tour.push_back(tour[p]);
    }

    int q = 0;
    for(int p=i; p<j+1; p++){
        new_tour.push_back(tour[j-q]);
        q++;
    }

    for(unsigned int p=j+1; p<tour.size(); p++){
        new_tour.push_back(tour[p]);
    }

    return new_tour;
}

double distance_city(int i, int j, vector<City> city_list){
    return City::dist(city_list[i], city_list[j]);
}

double tour_distance(const vector<int> tour, vector<City> city_list){
    double distance = 0;
    for(unsigned int p=0; p<tour.size()-1; p++){
        distance += City::dist(city_list[tour[p]], city_list[tour[p+1]]);
    }
    distance += distance_city(tour[0], tour[tour.size()-1], city_list);
    return distance;
}

vector<int> nearest_neighbor_tour(vector<City> &cityList){

    int number_of_cities(cityList.size());
    vector<int> tour;
    vector<int> used(number_of_cities);

    // Randomly choose the first city
    int first_city = rand() % number_of_cities;
    tour.push_back(first_city);
    used[first_city] = 1;

    int new_city = first_city;
    int number_of_visited_cities=1;

    while(number_of_visited_cities != number_of_cities){

        // find the nearest neighbor of the last city inserted in the tour
        new_city = find_nearest_neighbour(new_city, cityList, used);
        tour.push_back(new_city);
        used[new_city] = 1;
        number_of_visited_cities++;
    }

    return tour;
}

vector<int> nearest_insertion_tour(vector<City> &cityList){


    int number_of_cities(cityList.size());
    vector<int> tour;
    vector<int> used(number_of_cities);

    // Initialization: select the shortest edge and make a subtour of it
    int first_city = rand() % number_of_cities;
    tour.push_back(first_city);
    used[first_city] = 1;


    int new_city;
    int index_new_city;
    int number_of_visited_cities=1;
    while(number_of_visited_cities != number_of_cities){

        if(number_of_visited_cities==1){

            new_city =  find_nearest_neighbour(first_city, cityList, used);
            tour.push_back(new_city);

        } else {
            // Selection
            new_city = pick_unvisited_city(used);

            // Insertion
            index_new_city = find_minimum_triangular_distance_edge(new_city, tour, cityList);
            tour.insert(tour.begin() + index_new_city, new_city);
        }

        used[new_city] = 1;
        number_of_visited_cities++;

    }

    return tour;
}

int two_opt(vector<City> &city_list){
    vector<int> tour = nearest_insertion_tour(city_list);
    for(unsigned int p=0; p<tour.size(); p++){
        cout << tour[p];
        cout << " ";
    }
    vector<int> new_tour;
    int number_of_tour=0;
    int number_of_cities(tour.size());

    while(number_of_tour<20){
        for(int i = 1; i<number_of_cities; i++){
            for(int j=i+1; j<number_of_cities-1 ; j++){
                if(distance_city(tour[i-1],tour[j],city_list) + distance_city(tour[i],tour[j+1],city_list)< distance_city(tour[i-1],tour[i],city_list) + distance_city(tour[j],tour[j+1],city_list)){
                    tour = swap_edge(tour, i,j);
                }
            }
        }
        number_of_tour++;
    }
    cout << "" << endl;;
    for(unsigned int p=0; p<tour.size(); p++){
        cout << tour[p];
        cout << " ";
    }

    return 0;

}

int main()
{

    srand(time(NULL));

    vector<City> cityList;
    string line;
    int it = 0;
    double x;
    double y;
    City city;
    while (getline(cin, line))
    {
        string word;
        stringstream stream(line);
        int iteration = 0;
        while( getline(stream, word, ' ') ){
            if(iteration==0){
                x=atof(word.c_str());
            }else{
                y=atof(word.c_str());
            }
            iteration++;
        }
        cityList.push_back(City(x,y));
        it++;
    }

    two_opt(cityList);

    return 0;
}
