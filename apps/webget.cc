#include "../util/socket.hh"

#include <cstdlib>
#include <iostream>
#include <span>
#include <string>
#include <vector>
using namespace std;

void get_URL( const string& host, const string& path )
{
  cerr << "Function called: get_URL(" << host << ", " << path << ")\n";
  // cerr << "Warning: get_URL() has not been implemented yet.\n";
  TCPSocket sc;
  Address add(host,"http");
  cout<<add.ip()<<endl;
  
  // string str = "GET /hello HTTP/1.1\r\nHost: cs144.keithw.org\r\nConnection: close \r\n\r\n";
  // cout<<str<<endl;
  // sc.connect(add);
  // sc.write(str);
  // sc.read(msg);
  // cout<<msg<<endl;
  // sc.close();

  string msg;
  sc.connect(add);
  // sc.write(str);
  sc.write("GET ");
  sc.write(path);
  sc.write(" HTTP/1.1");
  sc.write("\r\n");
  sc.write("Host: ");
  sc.write(host);
  sc.write("\r\n");
  sc.write("Connection: close \r\n\r\n");
  sc.read(msg);
  cout<<msg<<endl;
  sc.close();

}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
