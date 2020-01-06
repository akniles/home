#include <iostream>
#include <algorithm>
#include <string>
#include <math.h>
#include <vector>
#include <ctime>
#include <iomanip>
#include <chrono>
// include <cstdlib>

using std::string;
using std::vector;

struct Point {
    double x_part, y_part;
};

struct Data {
    vector<Point> points;
    size_t to_cover;
    size_t size;
};

struct Circle {
    double radius;
    double center;
    vector<Point> points_covered;
};

Data Input() {
    Data data;
    std::cin >> data.size;
    std::cin >> data.to_cover;
    data.points.reserve(data.to_cover);
    for (size_t row = 0; row < data.size; ++row) {
        Point point;
        std::cin >> point.x_part >> point.y_part;
        point.y_part = static_cast<int>(abs(point.y_part));
        data.points.push_back(point);
    }
    return data;
};

double calc_center(Point one, Point other) {
    if (one.x_part != other.x_part) {
        double x_one = one.x_part;
        double x_other = other.x_part;
        double y_one = one.y_part;
        double y_other = other.y_part;

        return (y_other - y_one) / (x_other -x_one) * (y_one + y_other) / 2 + (x_one + x_other) / 2;
    } else {
        return one.x_part;
    }
}

double calc_radius(Point one, Point other, double center) {
    double hight = std::max(one.y_part, other.y_part);
    double x_change = (center - one.x_part);
    hight *= hight;
    double radius = (one.y_part) * (one.y_part) + x_change * x_change; 
    if (hight < radius) {
        return radius;
    } else {
        return hight;
    }
}

double get_distance(double center, Point point) {
    return point.y_part * point.y_part + (point.x_part - center) * (point.x_part - center); 
}

void CountCoveredPoints(Circle* circle, const vector<Point>* points) {
    for (const auto& point : (*points)) {
        if (circle->radius >= get_distance(circle->center, point)) {
            circle->points_covered.push_back(point);
        }
    }
}    

Circle CulcCircle(Point one, Point other, const vector<Point>* points) {
    Circle circle;
    circle.center = calc_center(one, other);    
    circle.radius = calc_radius(one, other, circle.center);
    CountCoveredPoints(&circle, points);
    return circle;
}

Circle FindCircle(const Data& data) {
    Circle circle;
    circle.radius = 2e32;
    circle.center = 2e32;
    for (size_t i = 0; i < data.points.size(); ++i) {
        for (size_t j = 0; j < data.points.size(); ++j) {
            Circle new_circle = CulcCircle(data.points[i], data.points[j], &(data.points));
            if (circle.radius > new_circle.radius and 
                new_circle.points_covered.size() >= data.to_cover) {
                circle = new_circle;
            }
        }
    }
    // printf("final_Radius: %.6lf center %lf\n", sqrt(circle.R), circle.center);
    circle.radius = sqrt(circle.radius);
    return circle;
}


Data RandomData(size_t size, size_t to_cover) {
    Data data;
    data.size = size;
    data.to_cover = to_cover;
    data.points.reserve(size);
    for (size_t row = 0; row < size; ++row) {
        Point point;
        srand(time(0) + row + size + to_cover);
        if (rand() % 2 == 0) {
            point.x_part = - rand() % 1000;    
        } else {
            point.x_part = rand() % 1000;
        }
        if (rand() % 2 == 0) {
            point.y_part = - rand() % 1000;    
        } else {
            point.y_part = rand() % 1000;
        }
        data.points.push_back(point);
    }
    return data;
}

Data OutLierData() {
    Data data;
    data.points.reserve(20000);
    for (int row = - 1000; row <= 1000; row += 2) {
        for (int rank = 991; rank <= 1000; rank += 2) {
            Point point;
            point.x_part = row;
            point.y_part = rank;
            data.points.push_back(point);
        }
    }
    for (int row = - 999; row <= 1000; row += 2) {
        for (int rank = 990; rank <= 1000; rank += 2) {
            Point point;
            point.x_part = row;
            point.y_part = rank;
            data.points.push_back(point);
        }
    }
    
    data.size = data.points.size();
    size_t to_cover;
    std::cin >> to_cover;
    data.to_cover = to_cover;
    return data;
}

size_t min(size_t one, size_t another) {
    if (one >= another) {
        return another;
    } else {
        return one;
    }
}

// real code;

Circle find_minimum_circle(const vector<Circle>& circles) {
    double minimum = 2e32;
    Circle min_circle;
    for (const auto& circle : circles) {
        if (circle.radius < minimum) {
            minimum = circle.radius;
            min_circle = circle;
        }
    }
    return min_circle;
}

struct Pair{
    vector<double> less;
    vector<double> more;
    vector<double> equal;
};

Pair* split(const vector<double>& to_split, Pair* pair) {
    size_t size = to_split.size();
    pair->less.reserve(size);
    pair->more.reserve(size);
    pair->equal.reserve(size);
    size_t index = rand() % size;
    double partioner = to_split.at(index);
    for (const auto& point : to_split) {
        if (point < partioner) {
            pair->less.push_back(point);
        } else if (partioner < point) {
            pair->more.push_back(point);
        } else {
            pair->equal.push_back(point);
        }
    }
    return pair;
}

double min_element(const vector<double>& distances) {
    double min = 2e32;
    for (const auto& point : distances) {
        if (point < min) {
            min = point;
        }
    }
    return min;
}

double max_element(const vector<double>& distances) {
    double max = 0;
    for (const auto& point : distances) {
        if (point > max) {
            max = point;
        }
    }
    return max;
}

double get_kth_stats(const vector<double>& distances, size_t to_cover) {
    if (to_cover > distances.size()) {
        throw std::invalid_argument("received wrong data");
        return 0;
    }
    if (to_cover == 1) {
        return min_element(distances);
    }
    if (distances.size() == 1) {
        return distances.at(0);
    }
    if (distances.size() == to_cover) {
        return max_element(distances);
    }
    Pair splitted;
    split(distances, &splitted);
    if (splitted.less.size() >= to_cover) {
        return get_kth_stats(splitted.less, to_cover);
    } else if (splitted.less.size() + splitted.equal.size() >= to_cover) {
        return splitted.equal[0];
    } else if (
        splitted.more.size() >= to_cover - 
        splitted.less.size() - splitted.equal.size()) {
        return get_kth_stats(splitted.more, 
            to_cover - splitted.less.size() - splitted.equal.size());
    } else {
        std::cout << "SORTING ERROR" << "to_cover: " 
                  << to_cover << "part_size: " << splitted.less.size() << '\n';
        return 0;
    }
}

double circle_covering_k_points(const Data& data, double x_part) {
    vector<double> points_covered;
    points_covered.reserve(data.size);
    for (const auto& point : data.points) {
        points_covered.push_back(get_distance(x_part, point));
    }
    return get_kth_stats(points_covered, data.to_cover);
}

void shift(vector<Circle>* minimums, Circle value, size_t minimums_size) {
    struct CustomMore {
        inline bool operator() (const Circle& one, const Circle& other) {
            return (one.radius > other.radius);
        }
    };
    if (minimums->size() < minimums_size) {
        minimums->push_back(value);
        std::sort(minimums->begin(), minimums->end(), CustomMore());
        return;
    } 
    int position = 0;
    bool status = 0;
    for (const auto& minimum : (*minimums)) {
        if (minimum.radius < value.radius and status == 0) {
            return;
        }
        if (minimum.radius > value.radius) {
            status = 1;
        }
        if (minimum.radius < value.radius and status == 1) {
            minimums->insert((minimums->begin() + position), value);
            minimums->erase(minimums->begin());
            return;
        } 
        ++position;
    }
    if (status == 1) {
        minimums->erase(minimums->begin());
        minimums->push_back(value);
    }
}

vector<Circle> k_minimums (const vector<Circle> values) {
    vector<Circle> minimums;
    size_t size = values.size();
    size_t minimums_size = 15;
    if (size <= minimums_size) {
        return values;
    }
    minimums.reserve(size);
    for (const auto& value : values) {
        shift(&minimums, value, minimums_size);
    }
    return minimums;
}

vector<Circle> calc_points(const Data& data, double diff, double start, double end) {
    struct customLess {
        inline bool operator() (const Circle& one, const Circle& other) {
            return (one.radius < other.radius);
        }
    };
    Circle minimum_circle;
    vector<Circle> first_aproximation;
    double local_minimum = 2e32;
    first_aproximation.reserve(static_cast<int>((end - start) / diff));
    for (double point = start; point <= end; point += diff) {
        Circle new_circle;
        new_circle.radius = sqrt (circle_covering_k_points(data, point) );
        new_circle.center = point;
        if (new_circle.radius - diff * 0.35 < local_minimum) { // todo: find diff multiplier
            first_aproximation.push_back(new_circle);
            if (new_circle.radius < local_minimum) {
                local_minimum = new_circle.radius;
            }    
        }  
    }
    return k_minimums(first_aproximation); 
}

void append(vector<Circle>* minimums, const vector<Circle> to_append, double diff, double minimum) {
    for (const auto& circle : to_append) {
        if (circle.radius - diff < minimum) {
            minimums->push_back(circle);    
        }        
    }
    return;
}

void count_duplicate(vector<Circle> values_circles) {
    vector<double> values;
    for (const auto& circle : values_circles) {
        values.push_back(circle.center);
    }
    int count_duplicates = 0;
    vector<double> seen;
    for (const auto& value : values) {
        if (std::count(seen.begin(), seen.end(), value) == 0) {
            count_duplicates += std::count(values.begin(), values.end(), value) - 1;    
            seen.push_back(value);
        }
    }
    std::cout << "duplicates number: " << count_duplicates << '\n';
}

vector<Circle> remove_duplicates(vector<Circle> with_duplicates) {
    vector<double> centers;
    vector<Circle> circles;
    circles.reserve(with_duplicates.size());
    centers.reserve(with_duplicates.size());
    for (const auto& duplicate : with_duplicates) {
        if (std::count(centers.begin(), centers.end(), duplicate.center) == 0) {
            centers.push_back(duplicate.center);
            circles.push_back(duplicate);
        }
    }
    return circles;
}

Point minmax(const Data& data) {
    double min = 1000;
    double max = - 1000;
    Point result;
    for (const auto& point : data.points) {
        if (point.x_part < min) {
            min = point.x_part;
        } 
        if (point.x_part > max) {
            max = point.x_part;
        }
    }
    result.x_part = min;
    result.y_part = max;
    return result;
}

Circle FastSearch(const Data& data) { // todo: optimise
    vector<Circle> second_minimums;
    Point boarders = minmax(data);
    auto minimums = calc_points(data, 20, boarders.x_part, boarders.y_part);
    Circle min_circle = find_minimum_circle(minimums);
    second_minimums = minimums;
    for (const auto& circle : minimums) {
        auto tmp_minimums = calc_points(data, 2, circle.center - 18, circle.center + 18);
        Circle local_minimum = find_minimum_circle(tmp_minimums);
        if (local_minimum.radius - 2 < min_circle.radius) {
            if (local_minimum.radius < min_circle.radius) {
                min_circle = local_minimum;
            }
            append(&second_minimums, tmp_minimums, 1, min_circle.radius);
        }
    }
    
    second_minimums = remove_duplicates(second_minimums);
    second_minimums = k_minimums(second_minimums);
    auto third_minimums = second_minimums;
    for (const auto& circle : second_minimums) { 
        auto tmp_minimums = calc_points(data, 0.1, circle.center - 1.9, circle.center + 1.9);
        Circle local_minimum = find_minimum_circle(tmp_minimums);
        if (local_minimum.radius - 0.1 < min_circle.radius) {
            if (local_minimum.radius < min_circle.radius) {
                min_circle = local_minimum;
            }
            append(&third_minimums, tmp_minimums, 0.1, min_circle.radius);
        }
    }
    third_minimums = remove_duplicates(third_minimums);
    third_minimums = k_minimums(third_minimums);
    for (const auto& circle : third_minimums) {
        auto tmp_minimums = calc_points(data, 0.01, circle.center - 0.09, circle.center + 0.09);
        Circle local_minimum = find_minimum_circle(tmp_minimums);
        if (local_minimum.radius < min_circle.radius) {
            min_circle = local_minimum;
        }
    }

    for (double step = 0.001; step >= 0.000001; step /= 10) {
        second_minimums = calc_points(
            data, 
            step, 
            min_circle.center - 9 * step, 
            min_circle.center + 9 * step);
        min_circle = find_minimum_circle(second_minimums);
    }

    return min_circle;
}

bool wrong_answer(double new_answer, Circle true_circle) {
    double first = ( true_circle.radius );
    double second = ( new_answer );
    return abs(first - second) > 10e-3;
}

void Output(const Data& data, double new_circle, Circle true_circle, size_t i, size_t j, size_t k) {
    for (const auto& point : data.points) {
        std::cout << point.x_part << ' ' << point.y_part  << '\n';
    }
    std::cout << "test " << k << " failed! \t n : " << i << "\t k : " << j << "\t diff: " << true_circle.radius - new_circle << '\n';
    std::cout << std::fixed << std::setprecision(6) << "right answer: " << true_circle.radius << "\tyour answer: " << new_circle << '\n';
    std::cout << "right center: " << true_circle.center << "\tyour center: " << new_circle << '\n';
}
struct point {
    double xi;
    int type;
};

point point_constructor(double xi, int type) {
    return {xi, type};
}

bool comp(point first, point second) {
    bool res = (first.xi < second.xi || (first.xi == second.xi && first.type > second.type));
    return res;
}

bool Rad(double middle, const std::vector<std::vector<int>> &mas, int ni, int ki ) {
    int res = 0;
    std::vector<point> ot;
    for (int ind = 0; ind < ni; ++ind) {
        if (middle < mas[ind][1]) {
            continue;
        }
        double xFirst =  mas[ind][0] + sqrt(pow(middle, 2) - pow(mas[ind][1], 2));
        double xSecond =  mas[ind][0] - sqrt(pow(middle, 2) - pow(mas[ind][1], 2));
        ot.push_back(point_constructor(xSecond, 1));
        ot.push_back(point_constructor(xFirst, -1));
    }
    sort(ot.begin(), ot.end(), comp);
    for (auto index : ot)
    {
        res += index.type;
        if (res == ki) {
            return true;
        }
    }
    return false;
  }

double MinimalRadius(const std::vector<std::vector<int>> &mas, int ni, int ki) {
    double left = 0, right = 1000 * sqrt(2);
    while (left + 0.0001 < right) {
        double middle = left + (right - left) / 2;
        if (Rad(middle, mas, ni, ki)) {
            right = middle;
        } else {
            left = middle;
        }
    }
    return left;
  }

vector<vector<double>> translate (vector<Point> points) {
    vector<vector<double>> result;
    for (const auto& point : points) {
        result.push_back({point.x_part, point.y_part});
    }
    return result;
}

bool IsFit(const vector<vector<double>> &points_array,
           const int &kmin, const double &radius) {
    vector<double> leftends;
    vector<double> rightends;
    for (auto &point : points_array) {
        if (abs(point[1]) <= radius) {
            double leftelement = point[0] -
                                 sqrt(abs(std::pow(radius, 2) - std::pow(point[1], 2)));
            leftends.push_back(leftelement);
            double rightelement = point[0] +
                                  sqrt(abs(std::pow(radius, 2) - std::pow(point[1], 2)));
            rightends.push_back(rightelement);
        }
    }
    if (int(leftends.size()) < kmin) {
        return false;
    }
    std::sort(leftends.begin(), leftends.end());
    std::sort(rightends.begin(), rightends.end());
    size_t left_index = 0;
    size_t right_index = 0;
    int res = 0;
    while ((res < kmin) &&
           (left_index < leftends.size()) && (right_index < rightends.size())) {
        if (leftends[left_index] <= rightends[right_index]) {
            ++res;
            ++left_index;
        } else {
            --res;
            ++right_index;
        }
    }
    if (res == kmin) {
        return true;
    } else {
        return false;
    }

}

double MinRadius(const vector<vector<double>> &points_array, const int &kmin) {
    double minradius = 0;
    double maxradius = 1415;
    double midradius = minradius + (maxradius - minradius) / 2;
    while (maxradius - minradius > 0.0009) {
        if (IsFit(points_array, kmin, midradius) == false) {
            minradius = midradius;
            midradius = minradius + (maxradius - minradius) / 2;
        } else {
            maxradius = midradius;
            midradius = minradius + (maxradius - minradius) / 2;
        }
    }
    return midradius;
}


void StressTest(size_t times = 0) {
    for (size_t t = 0; t < times; ++ t) {
        for (size_t i = 1; i < 101; ++ i) {
            for (size_t j = 1; j <= i; ++ j) {
 
                Data data = RandomData(i, j);
                Circle true_circle = FindCircle(data);
                auto points = translate(data.points);/*
                for (const auto& point : points) {
                    std::cout << point[0] << point[1] << '\n';
                }
                std::cout << data.size << "\t" << data.to_cover << '\n';*/
                auto new_answer = MinRadius(points, data.to_cover);
 
                if (wrong_answer(new_answer, true_circle)) {                    
                    Output(data, new_answer, true_circle, i, j, t);
                    return;
                } else {
                    std::cout << "test passed! \t n : " << i << "\t k : " << j << '\n';
                }
 
            }
        } 
    }
}
 
int main() {
    StressTest(1);
    return 0;
}
