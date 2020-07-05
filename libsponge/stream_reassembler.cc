#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity), _capacity(capacity), unassembled_data(_capacity), exist_data(_capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (eof) {
        end_index = index + data.size();
        eof_set = true;
    }
    if (cur_max_index > index + data.size())
        return;
    size_t offset = max(index, cur_max_index) - index;
    for (size_t i = offset; i < data.size(); ++i) {
        unassembled_data[(i + index) % _capacity] = data[i];
        if (!exist_data[(i + index) % _capacity]) {
            exist_data[(i + index) % _capacity] = true;
            unassembled_size++;
        }
    }
    assemble_data();
}

size_t StreamReassembler::unassembled_bytes() const { return unassembled_size; }

bool StreamReassembler::empty() const { return unassembled_size == 0; }

void StreamReassembler::assemble_data() {
    string data_to_assemble = "";
    while (exist_data[cur_max_index % _capacity]) {
        exist_data[cur_max_index % _capacity] = false;
        data_to_assemble += unassembled_data[cur_max_index++ % _capacity];
        unassembled_size--;
    }
    _output.write(data_to_assemble);
    if (eof_set && cur_max_index == end_index) {
        _output.end_input();
    }
}
