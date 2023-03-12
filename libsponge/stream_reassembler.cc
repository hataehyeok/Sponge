#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity)
    : _output(capacity)
    , _capacity(capacity)
    , _unassembled(0)
    , _buffer(capacity, make_pair(0, false))
    , _eof(false) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);
    
    const size_t first_unassembled = _output.bytes_written();
    const size_t first_unacceptable = first_unassembled  - _output.buffer_size() + _capacity;
    const size_t assem_begin = max(index, first_unassembled);
    const size_t assem_end = min(index + data.size(), first_unacceptable);
    std::string temp_assem;
    
    if((index + data.size() == assem_end) && eof) {
        _eof = true;
    }

    for(size_t i = assem_begin; i < assem_end; i++) {
        if (_buffer[i - first_unassembled].second == false) {
            _buffer[i - first_unassembled].first = data[i - index];
            _buffer[i - first_unassembled].second = true;
            _unassembled++;
        }
    }

    while(_buffer[0].second) {
        temp_assem.push_back(_buffer[0].first);
        _buffer.pop_front();
        _buffer.push_back(make_pair(0, false));
        _unassembled--;
    }

    _output.write(temp_assem);

    // Complete assembling
    if (_unassembled == 0 && _eof) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const {
    return _unassembled;
}

bool StreamReassembler::empty() const {
    return _unassembled == 0;
}
