#include <exception>

class BooleanSemanticError : virtual public std::exception {
 protected:
  int error_number = 0;
  int error_offset = 0;
  std::string error_message;

 public:
  explicit BooleanSemanticError(const std::string& msg, int err_num, int err_off) {
    error_number = err_num;
    error_offset = err_off;
    error_message = msg;
  }

  virtual ~BooleanSemanticError() noexcept {}

  virtual const char* what() const throw() {
    return error_message.c_str();
  }

  virtual int getErrorNumber() const throw() {
    return error_number;
  }

  virtual int getErrorOffset() const throw() {
    return error_offset;
  }
};