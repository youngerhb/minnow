#pragma once

#include "byte_stream.hh"
#include "tcp_receiver_message.hh"
#include "tcp_sender_message.hh"

#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <optional>
#include <queue>
#include <string>

class TCPSender
{
public:
  /* Construct TCP sender with given default Retransmission Timeout and possible ISN */
  TCPSender( ByteStream&& input, Wrap32 isn, uint64_t initial_RTO_ms )
    : input_( std::move( input ) )
    , isn_( isn )
    , writer_( writer() )
    , reader_( reader() )
    , initial_RTO_ms_( initial_RTO_ms )
    , RTO_ms_( initial_RTO_ms_ )
    , windows_size_( 1 )
    , current_(0)
    , SYN_(false);
    , FIN_(false);
    , RST_(false);
    , current_time_(0);
    , que_front_no_( 0 )
    , counts_retransmissions_(0)
    , msg_que_({})
    , time_que_({})
  {}

  /* Generate an empty TCPSenderMessage */
  TCPSenderMessage make_empty_message() const;

  /* Receive and process a TCPReceiverMessage from the peer's receiver */
  void receive( const TCPReceiverMessage& msg );

  /* Type of the `transmit` function that the push and tick methods can use to send messages */
  using TransmitFunction = std::function<void( const TCPSenderMessage& )>;

  /* Push bytes from the outbound stream */
  void push( const TransmitFunction& transmit );

  /* Time has passed by the given # of milliseconds since the last time the tick() method was called */
  void tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit );

  // Accessors
  uint64_t sequence_numbers_in_flight() const;  // How many sequence numbers are outstanding?
  uint64_t consecutive_retransmissions() const; // How many consecutive *re*transmissions have happened?
  Writer& writer() { return input_.writer(); }
  const Writer& writer() const { return input_.writer(); }

  // Access input stream reader, but const-only (can't read from outside)
  const Reader& reader() const { return input_.reader(); }

private:
  // Variables initialized in constructor
  ByteStream input_;
  Wrap32 isn_;
  Writer& writer_;
  Reader& reader_;

  uint64_t initial_RTO_ms_;
  uint64_t RTO_ms_;

  uint64_t windows_size_;
  uint64_t current_; 
  bool SYN_;
  bool FIN_;
  bool RST_;
  uint64_t current_time_
  uint64_t que_front_no_;
  uint64_t counts_retransmissions_;
  queue<TCPSenderMessage> msg_que_;
  queue<uint64_t> time_que_;
};
