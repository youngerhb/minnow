#include "tcp_receiver.hh"
#include <iostream>
using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  // Your code here.
  if ( message.SYN && !is_connect_ ) {
    zero_point_ = message.seqno;
    ackno = message.seqno + 1;
    is_connect_ = true;
  }

  if ( is_connect_ ) {
    uint64_t first_index = message.seqno.unwrap( zero_point_, checkpoint_ );
    first_index = ( first_index == 0 && message.SYN )
                    ? 0
                    : first_index - 1; // Ack报不带Data会出现first_index=0的情况不能直接减一
    reassembler_.insert( first_index, message.payload, message.FIN );

    if ( reassembler_.writer().is_closed() )
      ackno = ackno + ( reassembler_.writer().bytes_pushed() - checkpoint_ ) + 1;
    else
      ackno = ackno + ( reassembler_.writer().bytes_pushed() - checkpoint_ );
    checkpoint_ = reassembler_.writer().bytes_pushed();
  }
}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  uint16_t window_size = reassembler_.writer().available_capacity() > __UINT16_MAX__
                           ? __UINT16_MAX__
                           : reassembler_.writer().available_capacity();
  if ( is_connect_ ) {
    return { ackno, window_size, RST_ };
  } else {
    return { {}, window_size, RST_ };
  }
}
