#include <iostream>
#include <random>
#include <fstream>
#include <vector>
#include <algorithm>

using std::vector;

struct HeapValue {
	size_t number;
	size_t start;
	size_t end;
	size_t index;
};

struct Data {
	vector<int> sequence;
	size_t size;
};

struct Query {
	int value; // hash value
	size_t start;
	size_t end;
	size_t index;
};

struct Edge {
	int value; // hash value
	size_t start;
	size_t end;
	size_t index;
};

template<class SetValue>
class FixedSet {
private:
	int multiplier_;
	int shift_;
	int modulo_;
	int prime_modulo_;
	vector<vector<SetValue>> hash_map_;

public:
	void Initialize(const size_t size) {
		multiplier_ = 35531;
		shift_ = 2447;
		modulo_ = 2 * size + 17;
		prime_modulo_ = 1004987;
		hash_map_.resize(modulo_, vector<SetValue>(1));
	}

	SetValue Remove(const int value) {
		size_t index = std::abs(value * multiplier_ + shift_) % prime_modulo_ % modulo_;
		for (size_t iter = 1; iter < hash_map_[index].size(); ++iter) {
			if (hash_map_[index][iter].value == value) {
				SetValue result = hash_map_[index][iter];
				hash_map_[index].erase(hash_map_[index].begin() + iter);
				return result;
			}
		}
		return { 0, 0, 0, 0 };
	}

	void Add(const SetValue value) {
		size_t index = std::abs(value.value * multiplier_ + shift_) % prime_modulo_ % modulo_;
		hash_map_[index].push_back(value);
	}

	bool Contains(int value) const {
		size_t index = std::abs(value * multiplier_ + shift_) % prime_modulo_ % modulo_;
		if (hash_map_[index].size() == 1) {
			return false;
		}
		else {
			for (size_t i = 1; i < hash_map_[index].size(); ++i) {
				if (hash_map_[index][i].value == value) {
					return true;
				}
			}
			return false;
		}
	}
};

class MaxHeap {
private:
	vector<HeapValue> heap_;
	size_t size_ = 0;
	size_t max_size_ = 0;


public:
	void Initialize(size_t size) {
		max_size_ = size + 1;
		heap_.resize(max_size_);
	}

	size_t size() {
		return size_;
	}

	bool empty() {
		return size_ == 0;
	}

	int GetMax() {
		return heap_[0].number;
	}

	int GetAfterLast() {
		return heap_[size_].number;
	}

	void SiftDown(size_t index, vector<size_t>* indexes) {
		size_t left = 2 * index + 1;
		size_t right = 2 * index + 2;
		size_t more = index;
		if (left < size_ and (heap_[left].number > heap_[index].number or
			(heap_[left].number == heap_[index].number
				and heap_[left].start < heap_[index].start))) {
			more = left;
		}
		if (right < size_ and (heap_[right].number > heap_[index].number or
			(heap_[right].number == heap_[index].number and
				heap_[right].start < heap_[index].start))) {
			more = right;
		}
		if (more != index) {
			std::swap(heap_[index], heap_[more]);
			std::swap(indexes->at(heap_[more].index),
				indexes->at(heap_[index].index));
			SiftDown(more, indexes);
		}
	}

	HeapValue ExtractMax(vector<size_t>* indexes) {
		if (size_ == 0) {
			return { 0, 0 };
		}
		else {
			HeapValue maximum = heap_[0];
			std::swap(indexes->at(heap_[size_ - 1].index),
				indexes->at(heap_[0].index));
			heap_[0] = heap_[size_ - 1];
			--size_;
			SiftDown(0, indexes);
			return maximum;
		}
	}

	void Insert(HeapValue value, vector<size_t>* indexes, size_t position) {
		size_t index = size_;
		heap_[index] = value;
		indexes->at(position) = index;
		++size_;

		while (index != 0 and (heap_[(index - 1) / 2].number < heap_[index].number or
			(heap_[(index - 1) / 2].number == heap_[index].number and
				heap_[(index - 1) / 2].start > heap_[index].start))) {
			std::swap(heap_[index], heap_[(index - 1) / 2]);
			std::swap(indexes->at(heap_[(index - 1) / 2].index),
				indexes->at(heap_[index].index));
			index = (index - 1) / 2;
		}
	}

	void SiftUp(size_t index, vector<size_t>* indexes) {
		if (index == 0) {
			return;
		}
		size_t previous = (index - 1) / 2;
		if (heap_[previous].number < heap_[index].number or
			(heap_[previous].number == heap_[index].number and
				heap_[previous].start > heap_[index].start)) {
			std::swap(heap_[index], heap_[previous]);
			std::swap(indexes->at(heap_[previous].index),
				indexes->at(heap_[index].index));
			SiftUp(previous, indexes);
		}
	}

	void ChangeKey(size_t index, HeapValue new_value, vector<size_t>* indexes) {
		heap_[index] = new_value;
		int previous = static_cast<int>(index - 1) / 2;
		if (heap_[previous].number < heap_[index].number or
			(heap_[previous].number == heap_[index].number and
				heap_[previous].start > heap_[index].start)) {
			SiftUp(index, indexes);
		}
		else {
			SiftDown(index, indexes);
		}
	}

	void Remove(size_t index, vector<size_t>* indexes) {
		std::swap(heap_[index], heap_[size_ - 1]);
		std::swap(indexes->at(heap_[size_ - 1].index),
			indexes->at(heap_[index].index));
		--size_;
		if (size_ == 0) {
			return;
		}

		int previous = static_cast<int>(index - 1) / 2;
		if (heap_[previous].number < heap_[index].number or
			(heap_[previous].number == heap_[index].number and
				heap_[previous].start > heap_[index].start)) {
			SiftUp(index, indexes);
		}
		else {
			SiftDown(index, indexes);
		}
	}

	void PrintHeap() {
		std::cout << "size: " << size_ << "\n";
		for (size_t index = 0; index < size_; ++index) {
			std::cout << heap_[index].number << " ";
		}
		std::cout << "\n";
	}
};

size_t ReadInput(vector<int>* data) {
	// std::ifstream input_file;
	// input_file.open("D:/yandex.school/algorithms/input.txt");
	size_t size;
	size_t command_number;
	std::cin >> size >> command_number;
	data->resize(command_number);
	for (auto& element : *data) {
		std::cin >> element;
	}
	return size;
}

// todo: make up the full algorithm
void AllocateMemory(MaxHeap* sectors, FixedSet<Query>* set,
	FixedSet<Edge>* starts, FixedSet<Edge>* ends,
	vector<int>* result, vector<size_t>* indexes, size_t command, size_t index) {
	HeapValue longest = sectors->ExtractMax(indexes);
	result->push_back(longest.start);
	Query query{ static_cast<int>(index), longest.start, longest.start + command - 1 };
	Edge start{ static_cast<int>(longest.start + command),
			   longest.start + command, longest.end, longest.index };
	Edge end{ static_cast<int>(longest.end),
			 longest.start + command, longest.end, longest.index };
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
void Free(MaxHeap* sectors, FixedSet<Query>* set, FixedSet<Edge>* ends, FixedSet<Edge>* starts,
	vector<size_t>* indexes, int command, size_t index) {
	Query deleted = set->Remove(command);
	if (ends->Contains(deleted.start - 1) and starts->Contains(deleted.end + 1)) {
		Edge deleted_before = ends->Remove(deleted.start - 1);
		Edge deleted_after = starts->Remove(deleted.end + 1);
		HeapValue new_value{ deleted_after.end - deleted_before.start + 1,
							deleted_before.start, deleted_after.end, deleted_before.index };
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
	}
	else if (starts->Contains(deleted.end + 1)) {
		Edge deleted_after = starts->Remove(deleted.end + 1);
		HeapValue new_value{ deleted_after.end - deleted.start + 1,
							deleted.start, deleted_after.end, deleted_after.index };
		sectors->ChangeKey(indexes->at(deleted_after.index), new_value, indexes);
		deleted_after.start = deleted.start;
		deleted_after.value = deleted_after.start;
		Edge deleted_end = ends->Remove(deleted_after.end);
		deleted_end.start = deleted.start;
		ends->Add(deleted_end);
		starts->Add(deleted_after);
	}
	else if (ends->Contains(deleted.start - 1)) {
		Edge deleted_before = ends->Remove(deleted.start - 1);
		HeapValue new_value{ deleted.end - deleted_before.start + 1,
							deleted_before.start, deleted.end, deleted_before.index };
		sectors->ChangeKey(indexes->at(deleted_before.index), new_value, indexes);
		deleted_before.end = deleted.end;
		deleted_before.value = deleted_before.end;
		Edge deleted_start = starts->Remove(deleted_before.start);
		deleted_start.end = deleted.end;
		ends->Add(deleted_before);
		starts->Add(deleted_start);
	}
	else {
		HeapValue sector{ deleted.end - deleted.start + 1, deleted.start, deleted.end, index };
		sectors->Insert(sector, indexes, index);
		Edge start{ static_cast<int>(deleted.start), deleted.start, deleted.end, index };
		Edge end{ static_cast<int>(deleted.end), deleted.start, deleted.end, index };
		starts->Add(start);
		ends->Add(end);
	}
}

void Process(const Data& data, vector<int>* result) {
	vector<size_t> indexes(data.sequence.size());
	FixedSet<Query> set;
	FixedSet<Edge> ends, starts;
	MaxHeap sectors;
	result->reserve(data.sequence.size());
	set.Initialize(data.sequence.size());
	ends.Initialize(data.sequence.size());
	starts.Initialize(data.sequence.size());
	sectors.Initialize(data.sequence.size());
	HeapValue sector{ data.size, 1, data.size, 0 };
	sectors.Insert(sector, &indexes, 0);
	for (size_t index = 0; index < data.sequence.size(); ++index) {
		if (data.sequence[index] > 0) {
			if (!sectors.empty() and sectors.GetMax() >= data.sequence[index]) {
				AllocateMemory(&sectors, &set, &starts, &ends, result,
					&indexes, data.sequence[index], index);
			}
			else {
				result->push_back(-1);
			}
		}
		else if (set.Contains(abs(data.sequence[index]) - 1)) {
			Free(&sectors, &set, &ends, &starts,
				&indexes, abs(data.sequence[index]) - 1, index);
		}
	}
}

Data RandomData(int seed) {
	std::mt19937 generator(42 * seed);
	Data data;
	data.size = seed;
	std::uniform_int_distribution<int> random_seed(1, seed);
	std::uniform_int_distribution<int> big_seed(1, 10);
	data.sequence.resize(10);
	int index = 1;
	for (auto& element : data.sequence) {
		if (big_seed(generator) % 10 < 3) {
			element = -(random_seed(generator) % index + 1);
		}
		else {
			element = random_seed(generator);
			++index;
		}
	}
	return data;
}

bool Check(const vector<int>& one, const vector<int>& other) {
	if (one.size() != other.size()) {
		return false;
	}
	else {
		for (size_t index = 0; index < one.size(); ++index) {
			if (one[index] != other[index]) {
				return false;
			}
		}
	}
	return true;
}

size_t GetStart(const vector<int>& memory, int command) {
	vector<Edge> sectors;
	sectors.reserve(memory.size());
	Edge sector{ 0, 0, 0, 0 };
	for (size_t start = 0; start < memory.size(); ++start) {
		if (memory[start] == 0) {
			if (sector.value == 0 and start != 0) {
				sector.start = start;
			}
			++sector.value;
			++sector.end;
		}
		else {
			sectors.push_back(sector);
			sector = { 0, 0, 0, 0 };
		}
	}
	sectors.push_back(sector);
	struct CustomLess {
		inline bool operator()(const Edge& one, const Edge& other) {
			return (one.value < other.value or
				(one.value == other.value and one.start > other.start));
		}
	};

	std::sort(sectors.begin(), sectors.end(), CustomLess());
	if (sectors.back().value >= command) {
		return sectors.back().start + 1;
	}
	return 0;
}

void TrueAnswer(const Data& data, vector<int>* result) {
	FixedSet<Query> queries;
	result->reserve(data.sequence.size());
	queries.Initialize(data.sequence.size());
	vector<int> memory(data.size);
	int index = 1;
	for (const auto& command : data.sequence) {
		if (command > 0) {
			size_t start = GetStart(memory, command);
			if (start == 0) {
				result->push_back(-1);
			}
			else {
				result->push_back(start);
				queries.Add({ index, start, start + command - 1, static_cast<size_t>(index) });
				for (size_t position = start; position < start + command; ++position) {
					memory[position - 1] = 1;
				}
			}
		}
		else {
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

template<class T>
void PrintArray(const T& sequence) {
	for (const auto& element : sequence) {
		std::cout << element << '\n';
	}
	// std::cout << '\n';
}

int main() {
	vector<int> data;
	size_t size = ReadInput(&data);
	Data input{ data, size };
	vector<int> result;
	Process(input, &result);
	PrintArray(result);
	return 0;
}
