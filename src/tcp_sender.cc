#include "tcp_sender.hh"
#include "tcp_config.hh"
#include <string>

using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  // Your code here.
  return msg_que_.size();
}

uint64_t TCPSender::consecutive_retransmissions() const
{
  // Your code here.
  return counts_retransmissions_;
}

void TCPSender::push( const TransmitFunction& transmit )
{
  // Your code here.
  uint64_t size( 0 );
  string msg_payload;

  if ( windows_size_ == 0 )
    size = 1;
  else if ( windows_size_ <= TCPConfig::MAX_PAYLOAD_SIZE )
    size = windows_size_;
  else if ( windows_size_ > TCPConfig::MAX_PAYLOAD_SIZE )
    size = TCPConfig::MAX_PAYLOAD_SIZE;

  read( reader_, size, msg_payload );
  current_ = msg_payload.size();
  if ( current_ == 0 )
    SYN_ = true;
  if ( reader_.is_finished() )
    FIN_ = true;

  Wrap32 sqeno = Wrap32::wrap( current_, isn_ );
  TCPSenderMessage msg( { sqeno, SYN_, msg_payload, FIN_, RST_ } );

  msg_que_.push( msg );
  time_que_.push( current_time_ );

  transmit( msg );
}

TCPSenderMessage TCPSender::make_empty_message() const
{
  // Your code here.
  return { Wrap32::wrap( current_, isn_ ), false, "", false, false };
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  // Your code here.
  if ( msg.ackno ) {
    counts_retransmissions_ = 0;
    windows_size_ = msg.window_size;

    uint64_t tmp_ackno
      = msg.ackno->unwrap( isn_, que_front_no_ ); // 不知道直接使用current_作为checkpoint会不会有些影响
    auto tmp_msg = msg_que_.front();

    while ( tmp_ackno <= que_front_no_ ) {
      RTO_ms_ = initial_RTO_ms_;
      msg_que_.pop();
      time_que_.pop();
      tmp_msg = msg_que_.front();
      que_front_no_ += tmp_msg.sequence_length();
    }
  }
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  // Your code here.
  current_time_ += ms_since_last_tick;
  uint64_t retrans_time = current_time_ + RTO_ms_;
  uint64_t tmp_time = time_que_.front();
  TCPSenderMessage tmp_msg( {} );

  while ( tmp_time >= retrans_time ) {
    RTO_ms_ = windows_size_ == 0 ? RTO_ms_ * 2 : RTO_ms_;
    retrans_time = current_time_ + RTO_ms_;

    tmp_msg = msg_que_.front();
    tmp_time = time_que_.front();
    msg_que_.pop();
    time_que_.pop();
    que_front_no_ += tmp_msg.sequence_length();

    transmit( tmp_msg );
    counts_retransmissions_++;
    msg_que_.push( tmp_msg );
    time_que_.push( current_time_ );
  }
}
