#include <tools.cpp>
#include <max_heap.cpp>
#include <fixed_set.h>

using std::vector;

struct Data {
    vector<int> sequence;
    size_t size = 0;
};

struct Query {
    int value = 0; // hash value
    size_t start = 0;
    size_t end = 0;
    size_t index = 0;
};

struct Edge {
    int value = 0; // hash value
    size_t start = 0;
    size_t end = 0;
    size_t index = 0;
};


Data RandomData(int seed) {
    std::mt19937 generator(42 * seed);
    Data data;
    std::uniform_int_distribution<int> random_seed(1, 2e9);
    std::uniform_int_distribution<int> chance(1, 10);
    data.size = 1e6;
    data.sequence.resize(10);
    int index = 1;
    for (auto &element : data.sequence) {
        if (chance(generator) < 3) {
            element = -(random_seed(generator) % index + 1);
        } else {
            element = random_seed(generator);
            ++index;
        }
    }
    return data;
}

bool Check(const vector<int> &one, const vector<int> &other) {
    if (one.size() != other.size()) {
        return false;
    } else {
        for (size_t index = 0; index < one.size(); ++index) {
            if (one[index] != other[index]) {
                return false;
            }
        }
    }
    return true;
}

size_t GetStart(const vector<int> &memory, int command) {
    vector<Edge> sectors;
    sectors.reserve(memory.size());
    Edge sector{0, 0, 0, 0};
    for (size_t start = 0; start < memory.size(); ++start) {
        if (memory[start] == 0) {
            if (sector.value == 0 and start != 0) {
                sector.start = start;
            }
            ++sector.value;
            ++sector.end;
        } else {
            sectors.push_back(sector);
            sector = {0, 0, 0, 0};
        }
    }
    sectors.push_back(sector);
    struct CustomLess {
        inline bool operator()(const Edge &one, const Edge &other) {
            return (one.value < other.value or (one.value == other.value and one.start > other.start));
        }
    };

    std::sort(sectors.begin(), sectors.end(), CustomLess());
    if (sectors.back().value >= command) {
        return sectors.back().start + 1;
    }
    return 0;
}

void TrueAnswer(const Data &data, vector<int> *result) {
    FixedSet<Query> queries;
    result->reserve(data.sequence.size());
    queries.Initialize(data.sequence.size());
    vector<int> memory(data.size);
    int index = 1;
    for (const auto &command : data.sequence) {
        if (command > 0) {
            size_t start = GetStart(memory, command);
            if (start == 0) {
                result->push_back(-1);
            } else {
                result->push_back(start);
                queries.Add({index, start, start + command - 1, static_cast<size_t>(index)});
                for (size_t position = start; position < start + command; ++position) {
                    memory[position - 1] = 1;
                }
            }
        } else {
            if (queries.Contains(abs(command))) {
                Query deleted = queries.Remove(abs(command));
                for (size_t start = deleted.start; start <= deleted.end; ++start) {
                    memory[start - 1] = 0;
                }
            }
        }
        ++index;
    }
}

size_t ReadInput(vector<int> *data) {
    std::ifstream input_file;
    input_file.open("D:/yandex.school/algorithms/input.txt");
    size_t size;
    size_t command_number;
    input_file >> size >> command_number;
    data->resize(command_number);
    for (auto &element : *data) {
        input_file >> element;
    }
    return size;
}

// todo: make up the full algorithm
void AllocateMemory(MaxHeap *sectors, FixedSet<Query> *set, FixedSet<Edge> *starts, FixedSet<Edge> *ends,
                    vector<int> *result, vector<size_t> *indexes, size_t command, size_t index) {
    HeapValue longest = sectors->ExtractMax(indexes);
    result->push_back(longest.start);
    Query query{static_cast<int>(index), longest.start, longest.start + command - 1};
    Edge start{static_cast<int>(longest.start + command),
               longest.start + command, longest.end, longest.index};
    Edge end{static_cast<int>(longest.end),
             longest.start + command, longest.end, longest.index};
    set->Add(query);
    starts->Remove(longest.start);
    ends->Remove(longest.end);
    if (longest.number > command) {
        starts->Add(start);
        ends->Add(end);
        longest.number -= command;
        longest.start += command;
        sectors->Insert(longest, indexes, index);
    }
}

// todo: make up the full algorithm + discuss
void Free(MaxHeap *sectors, FixedSet<Query> *set, FixedSet<Edge> *ends, FixedSet<Edge> *starts,
          vector<size_t> *indexes, int command, size_t index) {
    auto deleted = set->Remove(command);
    if (ends->Contains(deleted.start - 1) and starts->Contains(deleted.end + 1)) {
        Edge deleted_before = ends->Remove(deleted.start - 1);
        Edge deleted_after = starts->Remove(deleted.end + 1);
        HeapValue new_value{deleted_after.end - deleted_before.start + 1,
                            deleted_before.start, deleted_after.end, deleted_before.index};
        sectors->ChangeKey(indexes->at(deleted_before.index), new_value, indexes);
        sectors->Remove(indexes->at(deleted_after.index), indexes);
        deleted_after = ends->Remove(deleted_after.end);
        deleted_before = starts->Remove(deleted_before.start);
        deleted_after.start = deleted_before.start;
        deleted_before.end = deleted_after.end;
        deleted_before.index = new_value.index;
        deleted_after.index = new_value.index;
        ends->Add(deleted_after);
        starts->Add(deleted_before);
    } else if (starts->Contains(deleted.end + 1)) {
        Edge deleted_after = starts->Remove(deleted.end + 1);
        HeapValue new_value{deleted_after.end - deleted.start + 1,
                            deleted.start, deleted_after.end, deleted_after.index};
        sectors->ChangeKey(indexes->at(deleted_after.index), new_value, indexes);
        deleted_after.start = deleted.start;
        deleted_after.value = deleted_after.start;
        Edge deleted_end = ends->Remove(deleted_after.end);
        deleted_end.start = deleted.start;
        ends->Add(deleted_end);
        starts->Add(deleted_after);
    } else if (ends->Contains(deleted.start - 1)) {
        Edge deleted_before = ends->Remove(deleted.start - 1);
        HeapValue new_value{deleted.end - deleted_before.start + 1,
                            deleted_before.start, deleted.end, deleted_before.index};
        sectors->ChangeKey(indexes->at(deleted_before.index), new_value, indexes);
        deleted_before.end = deleted.end;
        deleted_before.value = deleted_before.end;
        Edge deleted_start = starts->Remove(deleted_before.start);
        deleted_start.end = deleted.end;
        ends->Add(deleted_before);
        starts->Add(deleted_start);
    } else {
        HeapValue sector{deleted.end - deleted.start + 1, deleted.start, deleted.end, index};
        sectors->Insert(sector, indexes, index);
        Edge start{static_cast<int>(deleted.start), deleted.start, deleted.end, index};
        Edge end{static_cast<int>(deleted.end), deleted.start, deleted.end, index};
        starts->Add(start);
        ends->Add(end);
    }
}

void Process(const Data &data, vector<int> *result) {
    vector<size_t> indexes(data.sequence.size());
    FixedSet<Query> set;
    FixedSet<Edge> ends, starts;
    MaxHeap sectors;
    result->reserve(data.sequence.size());
    set.Initialize(data.sequence.size());
    ends.Initialize(data.sequence.size());
    starts.Initialize(data.sequence.size());
    sectors.Initialize(data.sequence.size());
    HeapValue sector{data.size, 1, data.size, 0};
    sectors.Insert(sector, &indexes, 0);
    for (size_t index = 0; index < data.sequence.size(); ++index) {
        if (data.sequence[index] > 0) {
            if (!sectors.empty() and sectors.GetMax() >= data.sequence[index]) {
                AllocateMemory(&sectors, &set, &starts, &ends, result, &indexes, data.sequence[index], index);
            } else {
                result->push_back(-1);
            }
        } else if (set.Contains(abs(data.sequence[index]) - 1)) {
            Free(&sectors, &set, &ends, &starts, &indexes, abs(data.sequence[index]) - 1, index);
        }
    }
}

int main() {
    /*vector<int> data;
    size_t size = ReadInput(&data);
    Data input{data, size};
    vector<int> result;
    Process(input, &result);
    // TrueAnswer(input, &result);
    PrintArray(result);
*/
    StressTest(RandomData, Process, TrueAnswer, Check, PrintArray, PrintData, 1000);
    return 0;
}
