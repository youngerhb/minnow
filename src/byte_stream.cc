#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), is_close( false ),bytes(0),deq(0) {}

bool Writer::is_closed() const
{
  // Your code here.
  return this->is_close;
}

void Writer::push( string data )
{
  // Your code here.
  
  for ( auto ch : data ) {
    if ( capacity_ == deq.size() )
    return;

    deq.push_back( ch );
    bytes++;
  }
  return;
}

void Writer::close()
{
  // Your code here.
  is_close = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - deq.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes;
}

bool Reader::is_finished() const
{
  // Your code here.
  return is_close && deq.empty();
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return bytes - deq.size();
}

string Reader::peek() const
{
  // Your code here.
  string str;
  str +=deq.front();
  return str;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  uint64_t count( len > deq.size() ? deq.size() : len );
  while ( count-- > 0 ) {
    deq.pop_front();
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return deq.size();
}
