#ifndef _EXTRA_EXCEPT_
#define _EXTRA_EXCEPT_

#include <exception>
#include <string>

class KeyError : public std::exception {
   public:
    KeyError() {}
    KeyError(const std::string& msg_) throw() : msg(msg_) {}
    virtual ~KeyError() throw() {}
    const char* what() const throw() { return msg.c_str(); }

   private:
    const std::string msg;
};

#endif /* _EXTRA_EXCEPT_ */
