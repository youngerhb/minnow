#include "wrapping_integers.hh"
#include <iostream>
using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  return Wrap32( static_cast<uint32_t>((uint64_t)zero_point.raw_value_ + n)%((uint64_t)UINT32_MAX +1));
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  // Your code here.
  uint64_t digits = checkpoint / ( (uint64_t)__UINT32_MAX__ + 1 );
  uint64_t remainder = checkpoint % ( (uint64_t)__UINT32_MAX__ + 1 );
  uint64_t result = (uint64_t)(raw_value_ - zero_point.raw_value_);

  if ( remainder > result ) {
    if ( remainder - result >= ( (uint64_t)__UINT32_MAX__ + 1 ) / 2 )
      result += ( digits + 1 ) * ( (uint64_t)__UINT32_MAX__ + 1 );
    else
      result += ( digits ) * ( (uint64_t)__UINT32_MAX__ + 1 );
  } else {
    if ( result - remainder >= ( (uint64_t)__UINT32_MAX__ + 1 ) / 2 )
      result += ( (uint64_t)__UINT32_MAX__ + 1 )*(digits>0?( digits -1 ):(digits));
    else
      result += ( digits ) * ( (uint64_t)__UINT32_MAX__ + 1 );
  }
  // cout << "result2： " << result << endl;
  return result;
}
