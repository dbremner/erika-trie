#ifndef ERIKA_VCODE
#define ERIKA_VCODE
#include "erika_defs.h"
#include <fstream>
#include <vector>

namespace erika {
  class vcode {
    std::vector<uint>   cMSB_;
    std::vector<ullong> base_;
    std::vector<ullong> V_;
    ullong              size_;
    ullong              block_size_;
    bool                is_diff_only_;

    vcode(const vcode &);
    vcode &operator=(const vcode &);
    void initialize();

  public:
    vcode(bool is_diff_only = false);
    vcode(std::ifstream &ifs);
    vcode(const char *filename);
    ~vcode();

    ullong size() const;
    void   push(ullong d);
    ullong select(ullong pos) const;
    ullong rank(ullong pos) const;
    ullong diff(ullong pos) const;

    bool read(std::ifstream &ifs);
    bool read(const char *filename);
    void write(std::ofstream &ofs) const;
    void write(const char *filename, bool is_append = false) const;
  };
}

#endif

