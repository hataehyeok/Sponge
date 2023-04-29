#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { 
    return _sender.stream_in().remaining_capacity();
}

size_t TCPConnection::bytes_in_flight() const { 
    return _sender.bytes_in_flight();
}

size_t TCPConnection::unassembled_bytes() const {
    return _receiver.unassembled_bytes();
}

size_t TCPConnection::time_since_last_segment_received() const {
    return _time_since_last_segment_received;
}

void TCPConnection::segment_received(const TCPSegment &seg) {
    DUMMY_CODE(seg);

    const TCPHeader &tcp_header = seg.header();
    _time_since_last_segment_received = 0;

    // sets both the inbound and outbound streams to the error state and kills the connection
    if (tcp_header.rst) {
        _error_state = true;
        _receiver.stream_out().set_error();
        _sender.stream_in().set_error();
        return;
    }
    // responding to a “keep-alive” segment.
    if (_receiver.ackno().has_value() and (seg.length_in_sequence_space() == 0) and seg.header().seqno == _receiver.ackno().value() - 1) {
        _sender.send_empty_segment();
        return;
    }
    // gives the segment to the TCPReceiver
    _receiver.segment_received(seg);
    // inbound stream ends before the TCPConnection has reached EOF on its outbound stream, this variable needs to be set to false.
    if (_receiver.stream_out().eof() && !(_sender.stream_in().eof() && _fin_sent)) {
        _linger_after_streams_finish = false;
    }
    // Check if the ACK flag is set
    const bool check_ack = tcp_header.ack;
    if (check_ack) {
        _sender.ack_received(tcp_header.ackno, tcp_header.win);
    }
    if (seg.length_in_sequence_space() > 0) {
        if (!check_ack) {
            _sender.fill_window();
        }
        if (_sender.segments_out().empty()) {
            _sender.send_empty_segment();
        }
    }
    send_segments();
}

bool TCPConnection::active() const {
    // Unclean Shutdown
    if (_error_state) {
        return false;
    }
    // Clean Shutdown
    if (check_prerequisite() && !_linger_after_streams_finish) {
        return false;
    }
    return true;
}

size_t TCPConnection::write(const string &data) {
    DUMMY_CODE(data);

    if (active()) {
        const size_t size = _sender.stream_in().write(data);
        _sender.fill_window();
        return size;
    }
    return 0;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) {
    DUMMY_CODE(ms_since_last_tick);

    _time_since_last_segment_received += ms_since_last_tick;
    _sender.tick(ms_since_last_tick);
    // Abort
    if (_sender.consecutive_retransmissions() > _cfg.MAX_RETX_ATTEMPTS) {
        _error_state = true;
        _receiver.stream_out().set_error();
        _sender.stream_in().set_error();
    }

    if (check_prerequisite() && _time_since_last_segment_received >= 10 * _cfg.rt_timeout) {
        _linger_after_streams_finish = false;
    }
    send_segments();
}

void TCPConnection::end_input_stream() {
    _sender.stream_in().end_input();
    _sender.fill_window();
    send_segments();
}

void TCPConnection::connect() {
    // Initiate a connection by sending a SYN segment.
    // SYN is already setted in tcp_sender.cc
    _sender.fill_window();
    send_segments();
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // Your code here: need to send a RST segment to the peer
            _error_state = true;
            _receiver.stream_out().set_error();
            _sender.stream_in().set_error();
            send_segments();
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}

// private
// Set seqno, SYN, FIN, Payload from sender
// Set ackno, win from receiver
// But seqno, payload and SYN, FIN is already set in TCPsender
// If there is an ackno -> set ACK
void TCPConnection::send_segments() {
    queue<TCPSegment> &outgoing_queue = _sender.segments_out();

    while (!outgoing_queue.empty()) {
        TCPSegment tcp_seg = outgoing_queue.front();
        TCPHeader &tcp_header = tcp_seg.header();
        
        if (_receiver.ackno().has_value()) {
            tcp_header.ackno = _receiver.ackno().value();
            tcp_header.ack = true;
        }
        tcp_header.rst = _error_state;
        _fin_sent = tcp_header.fin | _fin_sent;
        tcp_header.win = _receiver.window_size();
        if (_receiver.window_size() > static_cast<size_t>(numeric_limits<uint16_t>().max())) {
            tcp_header.win = static_cast<size_t>(numeric_limits<uint16_t>().max());
        }
                
        _segments_out.push(tcp_seg);
        outgoing_queue.pop();
    }
}

bool TCPConnection::check_prerequisite() const {
    if ( _receiver.stream_out().eof()
        && _sender.stream_in().eof()
        && _fin_sent
        && _sender.bytes_in_flight() == 0) {
            return true;
    }
    return false;
}