#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity)
    , _rto{retx_timeout} {}

uint64_t TCPSender::bytes_in_flight() const {
    return _bytes_in_flight;
}

void TCPSender::fill_window() {  
    if (!_exist_syn) {
        TCPSegment seg;
        seg.header().syn = true;
        send_segment(seg);
        _exist_syn = true;
        return;
    }
    if (_exist_fin)
        return;
    if (!_segments_outstanding.empty() && _segments_outstanding.front().header().syn)
        return;         //not in case for using send_empty_segment
    if (!_stream.buffer_size() && !_stream.eof())
        return;

    // Calculate available window size and consider rwnd is 1 when rwnd is 0
    if (_rwnd == 0) {
        avail_wnd = _abs_ackno + 1 - _next_seqno;
    } else {
        avail_wnd = _abs_ackno + _rwnd - _next_seqno;
    }

    while (avail_wnd > 0) {
        TCPSegment seg;
        size_t payload_size = min({TCPConfig::MAX_PAYLOAD_SIZE, static_cast<size_t>(avail_wnd), _stream.buffer_size()});
        seg.payload() = Buffer{_stream.read(payload_size)};
        if (_stream.eof() && static_cast<size_t>(avail_wnd) > payload_size) {
            seg.header().fin = true;
            _exist_fin = true;
        }
        send_segment(seg);
        avail_wnd -= static_cast<uint64_t>(seg.length_in_sequence_space());

        if (_stream.buffer_empty())
            break;
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) { 
    DUMMY_CODE(ackno, window_size);

    _abs_ackno = unwrap(ackno, _isn, _next_seqno);
    if (_abs_ackno > _next_seqno) {
        return;
    }
    const size_t prev_size = _segments_outstanding.size();
    _rwnd = window_size;

    while (!_segments_outstanding.empty()) {
        TCPSegment &seg = _segments_outstanding.front();
        if (_abs_ackno < unwrap(seg.header().seqno, _isn, _next_seqno) + seg.length_in_sequence_space()) {
            break;
        }
        _bytes_in_flight -= seg.length_in_sequence_space();
        _segments_outstanding.pop();
    }

    if (prev_size != _segments_outstanding.size()) {
        _rto = _initial_retransmission_timeout;
        _time_elaps = 0;
        _consecutive_retransmissions = 0;
    }

    if (!_bytes_in_flight)
        _turn_timer = false;

    fill_window();
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    DUMMY_CODE(ms_since_last_tick);
    if (_turn_timer) {
        _time_elaps += ms_since_last_tick;
        if (_time_elaps >= _rto) {
            _segments_out.push(_segments_outstanding.front());
            if (_segments_outstanding.front().header().syn || (_rwnd > 0)) {
                _consecutive_retransmissions++;
                _rto *= 2;
            }
            _time_elaps = 0;
        }
    }
}

unsigned int TCPSender::consecutive_retransmissions() const {
    return _consecutive_retransmissions;
}

void TCPSender::send_empty_segment() {
    TCPSegment seg;
    seg.header().seqno = wrap(_next_seqno, _isn);
    _segments_out.push(seg);
}

void TCPSender::send_segment(TCPSegment &seg) {
    seg.header().seqno = wrap(_next_seqno, _isn);
    _segments_out.push(seg);
    _segments_outstanding.push(seg);
    
    _next_seqno += seg.length_in_sequence_space();
    _bytes_in_flight += seg.length_in_sequence_space();

    if (!_turn_timer) {
        _turn_timer = true;
        _time_elaps = 0;
    }
}
