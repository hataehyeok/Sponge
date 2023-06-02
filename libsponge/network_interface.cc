#include "network_interface.hh"

#include "arp_message.hh"
#include "ethernet_frame.hh"

#include <iostream>

using namespace std;

// Dummy implementation of a network interface
// Translates from {IP datagram, next hop address} to link-layer frame, and from link-layer frame to IP datagram

// For Lab 5, please replace with a real implementation that passes the
// automated checks run by `make check_lab5`.

// You will need to add private members to the class declaration in `network_interface.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

//! \param[in] ethernet_address Ethernet (what ARP calls "hardware") address of the interface
//! \param[in] ip_address IP (what ARP calls "protocol") address of the interface
NetworkInterface::NetworkInterface(const EthernetAddress &ethernet_address, const Address &ip_address)
    : _ethernet_address(ethernet_address), _ip_address(ip_address) {
    cerr << "DEBUG: Network interface has Ethernet address " << to_string(_ethernet_address) << " and IP address "
         << ip_address.ip() << "\n";
}

//! \param[in] dgram the IPv4 datagram to be sent
//! \param[in] next_hop the IP address of the interface to send it to (typically a router or default gateway, but may also be another host if directly connected to the same network as the destination)
//! (Note: the Address type can be converted to a uint32_t (raw 32-bit IP address) with the Address::ipv4_numeric() method.)
void NetworkInterface::send_datagram(const InternetDatagram &dgram, const Address &next_hop) {
    // convert IP address of next hop to raw 32-bit representation (used in ARP header)
    const uint32_t next_hop_ipv4 = next_hop.ipv4_numeric();
    auto dst_info = arp_table.find(next_hop_ipv4);

    auto time = arp_time_table.find(next_hop_ipv4);
    if (time != arp_time_table.end() && current_time <= time->second.second + 5000)
        return;

    if (dst_info != arp_table.end() && current_time <= dst_info->second.second + 30000) {
        send_ethernet_frame(dst_info->second.first, EthernetHeader::TYPE_IPv4, dgram.serialize());
        return;
    }

    send_arp(next_hop_ipv4, 1);
    arp_time_table.emplace(next_hop_ipv4, make_pair(dgram, current_time));
}

//! \param[in] frame the incoming Ethernet frame
optional<InternetDatagram> NetworkInterface::recv_frame(const EthernetFrame &frame) {
    const auto &ether_header = frame.header();

    if (ether_header.dst.empty()) {
        return nullopt;
    }

    if (ether_header.type == EthernetHeader::TYPE_IPv4 && ether_header.dst == _ethernet_address) {
        InternetDatagram _dgram;
        ParseResult parse_res = _dgram.parse(frame.payload());
        if (parse_res == ParseResult::NoError) {
            return _dgram;
        }
    } else if (ether_header.type == EthernetHeader::TYPE_ARP || ether_header.dst == ETHERNET_BROADCAST) {
        ARPMessage _msg;
        ParseResult parse_res = _msg.parse(frame.payload());
        if (parse_res == ParseResult::NoError) {
            if (_msg.target_ip_address != _ip_address.ipv4_numeric())
                return nullopt;

            uint32_t _get_ip = _msg.sender_ip_address;
            arp_table[_get_ip] = make_pair(_msg.sender_ethernet_address, current_time);

            auto sent_frame = arp_time_table.find(_get_ip);
            if (sent_frame != arp_time_table.end()) {
                send_ethernet_frame(
                    arp_table[_get_ip].first, EthernetHeader::TYPE_IPv4, arp_time_table[_get_ip].first.serialize());
                arp_time_table.erase(_get_ip);
            }
            if (_msg.opcode == ARPMessage::OPCODE_REQUEST) {
                send_arp(_get_ip, 0);
            }
        }
    }
    return nullopt;
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void NetworkInterface::tick(const size_t ms_since_last_tick) {
    current_time += ms_since_last_tick;

    auto it = arp_table.begin();
    while (it != arp_table.end()) {
        if (current_time > it->second.second + 30000) {
            it = arp_table.erase(it);
        } else {
            ++it;
        }
    }
}

void NetworkInterface::send_ethernet_frame(const EthernetAddress &dst, uint16_t type, const BufferList &payload) {
    EthernetFrame frame;
    frame.header().src = _ethernet_address;
    frame.header().dst = dst;
    frame.header().type = type;
    frame.payload() = payload;
    _frames_out.emplace(frame);
}

void NetworkInterface::send_arp(const uint32_t dst_ip, bool request) {
    ARPMessage arp;
    arp.sender_ip_address = _ip_address.ipv4_numeric();
    arp.sender_ethernet_address = _ethernet_address;
    arp.target_ip_address = dst_ip;

    if (request) {
        arp.opcode = ARPMessage::OPCODE_REQUEST;
        send_ethernet_frame(ETHERNET_BROADCAST, EthernetHeader::TYPE_ARP, arp.serialize());
    } else {
        arp.opcode = ARPMessage::OPCODE_REPLY;
        arp.target_ethernet_address = arp_table[dst_ip].first;
        send_ethernet_frame(arp_table[dst_ip].first, EthernetHeader::TYPE_ARP, arp.serialize());
    }
}