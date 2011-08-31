#ifndef __OUTPUT_CAPTURE_H__
#define __OUTPUT_CAPTURE_H__
class OutputCapture
{
  stringstream test_buffer;
  streambuf* cout_sbuf;
public:
  void clear(void)
  {
    test_buffer.str(string());
    cout_sbuf = std::cout.rdbuf(); // save original sbuf
  }
  void start( void )
  {
    // Capture the input to a test buffer
    cout.rdbuf(test_buffer.rdbuf()); // redirect 'cout' to a 'fout'
  }
  void stop( void )
  {
    // Restore the output stream
    cout.rdbuf(cout_sbuf); // restore the original stream buffer
  }
  string buffer(void) const
  {
    return test_buffer.str();
  }
};

#endif // __OUTPUT_CAPTURE_H__
