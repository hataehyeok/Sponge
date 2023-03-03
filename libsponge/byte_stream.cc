#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) { 
    DUMMY_CODE(capacity);
    ByteStream::_capacity = capacity;
    ByteStream::_write_sum = 0;
    ByteStream::_read_sum = 0;
    ByteStream::_reach_eof = false;
    ByteStream::_buffer = "";
    ByteStream::_error = false;
}

size_t ByteStream::write(const string &data) {
    const size_t usable = _capacity - _buffer.size();
    const size_t term = data.size() < usable ? data.size() : usable;
    _buffer.append(data.substr(0, term));
    DUMMY_CODE(data);
    _write_sum += term;
    return term;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    DUMMY_CODE(len);
    const size_t min = len < _buffer.size() ? len : _buffer.size();
    return _buffer.substr(0, min);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    DUMMY_CODE(len);
    const size_t min = len < _buffer.size() ? len : _buffer.size();
    _buffer.erase(0, min);
    _read_sum += min;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    DUMMY_CODE(len);
    const size_t min = len < _buffer.size() ? len : _buffer.size();
    std::string read = _buffer.substr(0, min);
    pop_output(min);
    return read;
}

void ByteStream::end_input() { _reach_eof = true;}

bool ByteStream::input_ended() const {
    return _reach_eof; 
}

size_t ByteStream::buffer_size() const { return _buffer.size(); }

bool ByteStream::buffer_empty() const { return _buffer.empty(); }

bool ByteStream::eof() const {
    return input_ended() && _buffer.empty();
}

size_t ByteStream::bytes_written() const { return _write_sum; }

size_t ByteStream::bytes_read() const { return _read_sum; }

size_t ByteStream::remaining_capacity() const {
    return _capacity - _buffer.size();
}
