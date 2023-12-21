#include <iostream>
#include <stdexcept>
#include <iomanip>
#include  <algorithm>

class Array {
public:
  using Iterator = double*;

  Array(size_t allocsize = minsize)
    : m_elements(0), m_allocsize(0), m_size(0) {
    allocate(allocsize);
  }

  Array(const Array& array)
    : Array() {
    *this = array;
  }

  Array(double* array, int size)
    : Array() {

    Array created = Array(size);

    for (int index = 0; index < size; ++index)
    {
      created.push_back(array[index]);
    }

    *this = created;
  }

  Array(Iterator first, Iterator last)
    : Array() {
    if (first > last)
      throw (std::logic_error("invalid iterators order"));
    Array created;
    for (Iterator it = first; it != last; ++it)
      created.push_back(*it);
    *this = created;
  }

  ~Array() { free(); }

  double& operator[](size_t index) {
    if (0 <= index && index < size())
      return m_elements[index];
    else throw (std::out_of_range("out of range"));
  }

  friend std::ostream& operator<<(std::ostream& out, const Array& array) {
    out << std::setprecision(3) << std::fixed;
    for (int i = 0; i < array.size(); i++) {
      out << array.m_elements[i] << " ";
    }
    return out << std::endl;
  }

  Array& operator=(const Array& array) {
    if (this != &array) {
      free();
      m_allocsize = array.m_allocsize;
      m_size = array.m_size;
      m_elements = new double[m_allocsize];
      for (unsigned i = 0; i < m_size; ++i) {
        m_elements[i] = array.m_elements[i];
      }
    }
    return *this;
  }

  double* begin() const { return m_elements; }
  double* end()   const { return (m_elements + m_size); }

  void clear() { resize(10); m_size = 0; }

  void resize(size_t reallocsize) {
    if (reallocsize > maxsize)
      throw(std::invalid_argument("array too long"));
    if (reallocsize == m_allocsize) return;
    if (reallocsize == 0) {
      free();
      m_elements = 0;
    }
    else {
      double* new_elements = new double[reallocsize];
      m_allocsize = reallocsize;
      if (m_size > reallocsize)
        m_size = reallocsize;
      for (unsigned i = 0; i < m_size; ++i)
        new_elements[i] = m_elements[i];
      free();
      m_elements = new_elements;
    }
  }

  size_t size()     const { return m_size; }
  size_t capacity() const { return m_allocsize; }

  void pop_back() {
    if (m_size) --m_size;
    else throw (std::logic_error("empty array deleting"));
  }

  void push_back(double new_element) {
    if (size() >= capacity())
      resize(std::min(capacity() + 10, maxsize));
    ++m_size;
    m_elements[size() - 1] = new_element;
  }

  void print(bool writeNewLine = true) const {
    if (size() == 0) {
      std::cout << "array is empty";
      if (writeNewLine) std::cout << "\n";
      return;
    }
    for (auto element : *this)
      std::cout << element << " ";
    if (writeNewLine) std::cout << "\n";
  }


private:
  static size_t    minsize;
  static size_t    maxsize;
  double* m_elements;
  size_t           m_allocsize;
  size_t           m_size;

  void allocate(size_t allocsize) {
    if (allocsize > maxsize)
      throw(std::invalid_argument("array too long"));
    if (allocsize < minsize) allocsize = minsize;
    m_allocsize = allocsize;
    m_elements = new double[m_allocsize];
  }

  void free() {
    if (m_elements) {
      delete[] m_elements;
      m_elements = 0;
    }
  }

};

size_t Array::minsize = 10;
size_t Array::maxsize = 10000;

Array create_Array(double* array, int size) {
  return Array(array, size);
}

double rand_double(int min, int max) {
  int range = max - min + 1;
  return std::rand() % range + min;
}

struct main_task_model {
  double sum;
  double avg;
};

main_task_model main_task(Array& array) {
  double sum = 0;
  double avg = 0;

  std::cout << array;

  for (int i = 0; i < array.size(); i++) {
    sum += array[i];
  }

  avg = sum / array.size();

  array.push_back(sum);
  array.push_back(avg);

  std::cout << array;

  main_task_model result = { sum, avg };

  return result;
}

void task20(Array& array) {
  main_task_model main_result = main_task(array);

  double avg = main_result.avg;
  double sum = main_result.sum;
  
  int positive = 0;

  for (int i = 0; i < array.size(); i++) {
    positive += static_cast<int>(array[i] > 0);
    if (positive == 3)  {
      array[i] = avg;
      positive = 0;
    }
  }

  std::cout << array;
}

int main()
{
  int array_size;
  std::cin >> array_size;
  double* input = new double[array_size];

  for (int i = 0; i < array_size; i++) {
    std::cin >> input[i];
  }

  Array arr = create_Array(input, array_size);

  int code;
  double argument;

  std::cout << arr.size() << std::endl;
  task20(arr);

  while (std::cin >> code)
  {
    if (code == 0)
      break;

    switch (code)
    {
    case 1:
      std::cin >> argument;
      arr.push_back(argument);
      std::cout << "+: " << arr.size() << std::endl;
      task20(arr);
      break;
    case 2:
      arr.pop_back();
      std::cout << "-: " << arr.size() << std::endl;
      task20(arr);
      break;
    }
  }
}