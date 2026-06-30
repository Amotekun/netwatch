#include <arpa/inet.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pcap/pcap.h>
#include <sstream>
#include <string>

void packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {

  std::ostringstream json;
  std::ofstream *dump_file = reinterpret_cast<std::ofstream *>(user);

  // *dump_file << "packet captured: " << header->caplen << "bytes captured, " << header->len << "bytes on wire" << std::endl;
  std::cout << "Packet captured: " << header->caplen << " bytes captured, " << header->len << " bytes on wire" << std::endl;
  std::cout << "Timestamp: " << header->ts.tv_sec << "." << header->ts.tv_usec << std::endl;
  std::cout << "First bytes of packet: ";

  for (u_int i = 0; i < 16 && i < header->caplen; ++i) {
    std::cout << std::hex << static_cast<int>(packet[i]) << " ";
  }

  const struct ip *ipheader = reinterpret_cast<const struct ip *>(packet + 14);
  const struct tcphdr *tcpheader = reinterpret_cast<const struct tcphdr *>(packet + 34);
  const struct udphdr *udpheader = reinterpret_cast<const struct udphdr *>(packet + 34);

  if (header->caplen >= 14) {
    unsigned int ether_type = (packet[12] << 8) | packet[13];

    std::cout << "Ethernet: 0x" << std::setw(4) << std::setfill('0') << std::hex << ether_type << std::dec << std::endl;
    // TODO : Add the actual constant from the OS here
    if (ether_type == 0x0800) {
      std::cout << "This is an IPv4 packet." << std::endl;

      std::cout << "Source IP: " << inet_ntoa(ipheader->ip_src) << std::endl;
      std::cout << "Destination IP: " << inet_ntoa(ipheader->ip_dst) << std::endl;
      std::cout << "Protocol: " << static_cast<int>(ipheader->ip_p) << std::endl;
      if (ipheader->ip_p == IPPROTO_TCP) {
        std::cout << "This is a TCP packet." << std::endl;
        std::cout << "Source Port: " << ntohs(tcpheader->th_sport) << std::endl;
        std::cout << "Destination Port: " << ntohs(tcpheader->th_dport) << std::endl;
      } else if (ipheader->ip_p == IPPROTO_UDP) {
        std::cout << "This is a UDP packet. " << std::endl;
        std::cout << "UDP Source Port: " << ntohs(udpheader->uh_sport) << std::endl;
        std::cout << "UDP Destination Port: " << ntohs(udpheader->uh_dport) << std::endl;
      } else {
        std::cout << "This is neither a TCP nor a UDP packet." << std::endl;
      }
      // TODO: Add the actual constant from the OS here -> ETHERTYPE_IPV6
    } else if (ether_type == 0x86dd) {
      std::cout << "This is an IPv6 packet." << std::endl;
    } else {
      std::cout << "This is not an IP packet." << std::endl;
    }

    json << "{"
         << "\"timestamp\": \"" << header->ts.tv_sec << "." << header->ts.tv_usec << "\""
         << ", \"src_ip\": \"" << inet_ntoa(ipheader->ip_src) << "\""
         << ", \"dst_ip\": \"" << inet_ntoa(ipheader->ip_dst) << "\""
         << ", \"protocol\": \"" << static_cast<int>(ipheader->ip_p) << "\"";
    if (ipheader->ip_p == IPPROTO_TCP) {
      json << ", \"tcp_src_prt\": \"" << ntohs(tcpheader->th_sport) << "\""
           << ", \"tcp_dst_prt\":\"" << ntohs(tcpheader->th_dport) << "\"";
    } else if (ipheader->ip_p == IPPROTO_UDP) {
      json << ", \"udp_src_prt\": \"" << ntohs(udpheader->uh_sport) << "\""
           << ", \"udp_dst_prt\": \"" << ntohs(udpheader->uh_dport) << "\"";
    }
    json << "}";
  }

  *dump_file << json.str() << "\n";

  std::cout << std::dec << std::endl;
  std::cout << "----------------------------------------" << std::endl;
}

int main() {
  pcap_if_t *alldevs;
  char errbuf[PCAP_ERRBUF_SIZE];

  if (pcap_findalldevs(&alldevs, errbuf) == PCAP_ERROR) {
    std::cerr << "Error finding devices: " << errbuf << std::endl;
    return 1;
  }

  if (alldevs == nullptr) {
    std::cerr << "No devices found" << std::endl;
    return 1;
  }

  std::string device = alldevs->name;

  std::cout << "Using device: " << device << std::endl;

  pcap_freealldevs(alldevs);

  pcap_t *handle = pcap_open_live(device.c_str(), 65536, 1, 1000, errbuf);
  if (handle == nullptr) {
    std::cerr << "Error opening device: " << errbuf << std::endl;
    return 1;
  }

  std::ofstream dump_file("capture/output/packets.jsonl", std::ios::app);
  if (!dump_file.is_open()) {
    std::cerr << "Error opening output file" << std::endl;
    pcap_close(handle);
    return 1;
  }

  if (pcap_loop(handle, 10, packet_handler, reinterpret_cast<u_char *>(&dump_file))) {
    std::cerr << "Error capturing packets: " << pcap_geterr(handle) << std::endl;
    pcap_close(handle);
    return 1;
  }

  pcap_close(handle);

  return 0;
}
