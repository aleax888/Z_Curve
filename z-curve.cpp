#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <utility>
#include <cmath>
#include <algorithm>
#include <random>
using namespace std;
struct point;
// Functions -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
int generate_ramdon_number(int down = 1, int up = 100)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(down, up);
    return distrib(gen);
}

template<typename t>
bool sort_by_second(const pair<point*, t>& a, const pair<point*, t>& b)
{
    return (a.second < b.second);
}

// Struct Point -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
struct point
{
    int dimension;
    vector<float> coordinates;

    point()
    {
        dimension = 0;
    }

    point(int d, bool random = false, int down = 1, int up = 100)
    {
        dimension = d;
        if (random)
            for (int i = 0; i < dimension; ++i)
                coordinates.push_back(generate_ramdon_number(down, up));

        else
            for (int i = 0; i < dimension; ++i)
                coordinates.push_back(0);
    }

    point(vector<float> coords)
    {
        dimension = coords.size();
        coordinates = coords;
    }

    point(int d, vector<float> coords)
    {
        dimension = d;
        coordinates = coords;
    }

    point(const point& p)
    {
        dimension = p.dimension;
        coordinates = p.coordinates;
    }

    float& operator[](int index)
    {
        return coordinates[index];
    }

    point& operator=(point& p)
    {
        p.dimension = dimension;
        p.coordinates = coordinates;
        return *this;
    }

    void copy_point(point p)
    {
        dimension = p.dimension;
        coordinates = p.coordinates;
    }

    bool equal(point p)
    {
        if (dimension != p.dimension)
            return false;
        for (int i = 0; i < dimension; ++i)
            if ((*this)[i] != p[i])
                return false;
        return true;
    }

    double euclidean_distance(point p)
    {
        double answer = 0;
        for (int i = 0; i < dimension; ++i)
        {
            answer += pow(p.coordinates[i] - coordinates[i], 2);
        }
        return sqrt(answer);
    }

    point generate_random_point(int d, int down = 1, int up = 100)
    {
        vector<float> vec;
        for (int i = 0; i < d; ++i)
            vec.push_back(generate_ramdon_number(down, up));
        point p(d, vec);
        return p;
    }

    void print_point()
    {
        cout << "dimension: " << dimension << endl << "coordinates: ";
        for (int i = 0; i < dimension; ++i)
            cout << (*this)[i] << " ";
        cout << endl;
    }
};

// Struct Multidimesional Space -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
struct multidimesional_space
{
    int dimension;
    int capacity;
    vector<point> points;

    multidimesional_space(int d, int c, int down = 0, int up = 10)
    {
        dimension = d;
        capacity = c;

        for (int i = 0; i < capacity; ++i)
            points.push_back(point(dimension, true, down, up));
    }

    void sort_points()
    {

    }

    void print_space()
    {
        cout << "points into space: " << endl;
        for (int i = 0; i < capacity; ++i)
        {
            points[i].print_point();
            cout << endl;
        }
    }

    vector<pair<point*, unsigned long long>> make_z_curve()
    {
        vector<pair<point*, unsigned long long>> z_curve;

        for (int i = 0; i < capacity; ++i)
        {
            vector<string> binary_dimensions;
            for (int j = 0; j < dimension; ++j)
            {
                binary_dimensions.push_back(bitset<4>(points[i].coordinates[j]).to_string());
            }

            string z_string;
            for (int j = 0; j < 4; ++j)
            {
                for (int e = 0; e < dimension; ++e)
                {
                    z_string += binary_dimensions[e][j];
                }
            }
            z_curve.push_back(pair<point*, unsigned long long>{&points[i], bitset<40>(z_string).to_ullong()});
        }
        sort(z_curve.begin(), z_curve.end(), sort_by_second<unsigned long long>);
        return z_curve;
    }

    vector<pair<point*, float>> k_nearest_neighbors(point* pptr, int k)
    {
        vector<pair<point*, float>> answer;

        for (int i = 0; i < capacity; ++i)
        {
            answer.push_back(pair<point*, float>{&points[i], pptr->euclidean_distance(points[i])});
        }

        sort(answer.begin(), answer.end(), sort_by_second<float>);
        answer.erase(answer.begin());
        answer.erase(answer.begin() + k, answer.end());
        
        return answer;
    }

    int match_points_z_curve(point* pptr, vector<pair<point*, unsigned long long>> z_curve)
    {
        int i = 0;
        for (; i < capacity; ++i)
        {
            if (pptr == z_curve[i].first)
            {
                return i;
            }
        }
    }

    vector<pair<point*, float>> z_curve_k_nearest_neighbors(point* pptr, int k, vector<pair<point*, unsigned long long>> z_curve)
    {
        int match_index = match_points_z_curve(pptr, z_curve);
        vector<pair<point*, float>> answer;

        for (int i = 0; i < capacity; ++i)
        {
            unsigned long long distance;
            if (z_curve[match_index].second > z_curve[i].second)
                distance = z_curve[match_index].second - z_curve[i].second;
            else
                distance = z_curve[i].second - z_curve[match_index].second;
            
            answer.push_back(pair<point*, float>{z_curve[i].first, distance});
        }

        sort(answer.begin(), answer.end(), sort_by_second<float>);
        answer.erase(answer.begin());
        answer.erase(answer.begin() + k, answer.end());

        return answer;
    }

    int count_coincidences(vector<pair<point*, float>> vec1, vector<pair<point*, float>> vec2, int k)
    {
        int answer = 0;
        for (int i = 0; i < k; ++i)
        {
            for (int j = 0; j < k; ++j)
            {
                if (vec1[i].first->equal(*vec2[j].first))
                {
                    answer++;
                    break;
                }
            }
        }
        return answer;
    }

    void experiment(int *k, int t, int amount = 10)
    {
        for (int i = 0; i < amount; ++i)
        {
            point* random_point = &points[generate_ramdon_number(0, capacity - 1)];
            vector<pair<point*, unsigned long long>> z_curve = make_z_curve();
            for (int j = 0; j < t; ++j)
            {
                vector<pair<point*, float>> knn = k_nearest_neighbors(random_point, k[j]);
                vector<pair<point*, float>> zcknn = z_curve_k_nearest_neighbors(random_point, k[j], z_curve);

                cout << "point: " << i + 1 << ", k: "<< k[j] << "  --------------------";
                // si se desea comprobar los datos descomentar el siguiente fragmento
                /*cout << endl << "random point: " << endl;
                random_point->print_point();

                cout << endl << "knn: " << endl;
                for (int i = 0, s = knn.size(); i < s; ++i)
                {
                    cout << "distance: " << knn[i].second << endl;
                    knn[i].first->print_point();
                }

                cout << endl << "zcknn" << endl;
                for (int i = 0, s = zcknn.size(); i < s; ++i)
                {
                    cout << "distance: " << zcknn[i].second << endl;
                    zcknn[i].first->print_point();
                }*/

                cout << endl << float(count_coincidences(knn, zcknn, k[j])) / k[j] * 100 << "% winrate" << endl << endl;
            }
        }
    }
};

int main()
{
    // valores de k
    int k[] = {5, 10, 15, 20};
    // calculo del tama�o del arreglo de valores de k
    int t = sizeof(k) / sizeof(int);
    // numero de dimensiones para los puntos aleatorios
    int dimension = 10;
    // cuantos puntos aleatorios tengra el conjunto de muestra
    int capacity = 5000;
    // generacion del conjunto de muestra aleatorio segun los parametros anteriores
    // parametros (numero de dimensiones, cantidad de puntos a generar, 
                 //valor minimo de cada dimension(valor por defecto 0), 
                 //valor maximo de cada dimension(valor por defecto 10))
    multidimesional_space ms(dimension, capacity);
    
    // funcion que evalua la presicion al hacer knn en z_curve
    // parametros: (arreglos de valores para k, el tama�o del arreglo)
    ms.experiment(k, t);
}