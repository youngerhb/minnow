#include "reassembler.hh"
#include <map>
#include <string>
using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.
  auto& writer = output_.writer();
  size_ = writer.available_capacity();

  if ( is_last_substring )
    max_size = first_index + data.size();

  if ( first_index >= current_ + size_ )
    return;
  if ( first_index + data.size() < current_ )
    return;

  if ( first_index < current_ ) {
    data = data.substr( current_ - first_index );
    first_index = current_;
  }

  int tail = first_index + data.size() - current_ - size_;
  if ( tail > 0 ) {
    data = data.substr( 0, data.size() - tail );
  }

  if ( first_index == current_ ) {

    if ( data.size() <= size_ ) {

      writer.push( data );
      current_ += data.size();
    } else {
      string first_str = data.substr( 0, size_ );
      current_ += first_str.size();
      if ( first_str.size() != 0 )
        writer.push( first_str );
    }
  } else {
    auto itel = map_.find( first_index );
    if ( itel != map_.end() )
      itel->second = data.size() > itel->second.size() ? data : itel->second;
    else
      map_.insert( { first_index, data } );
  }

  if ( !map_.empty() && writer.available_capacity() != 0 ) {
    auto itel = map_.begin();
    uint64_t map_current = itel->first;
    string tmp = itel->second;

    if ( map_current + tmp.size() < current_ ) {
      while ( itel->first + itel->second.size() < current_ && itel != map_.end() ) 
        itel++;

      map_.erase( map_.begin(), itel );
      if ( !map_.empty() ) {
        itel = map_.begin();
        map_current = itel->first;
        tmp = itel->second;
      }
    }
 
    if ( map_current <= current_ &&!map_.empty()) {
      map_.erase( itel );
      insert( map_current, tmp, false );
    }
  }
  if ( max_size == writer.bytes_pushed() )
    writer.close();
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  if ( map_.empty() )
    return 0;
  uint64_t map_size( 0 );
  uint64_t  end( 0 );
  for ( auto itel : map_ ) {
    
    if ( itel.first + itel.second.size() > end ) {
      if ( itel.first > end )
        map_size += itel.second.size();
      else
        map_size += itel.second.size() - end + itel.first -1;
      end = itel.first  + itel.second.size()-1;
    }
  }
  return map_size;
}
