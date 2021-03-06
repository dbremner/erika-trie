#ifndef ERIKA_TRIE
#define ERIKA_TRIE
#include "erika_bv.h"
#include "erika_vcode.h"
#include <map>
#include <string>

namespace erika {
  class result {
    ullong begin_; // beginning position of key
    ullong end_;   // ending position of key
    ullong pos_;   // node position of trie

  public:
    result(ullong begin, ullong end, ullong pos)
     : begin_(begin), end_(end), pos_(pos) {}
    ullong begin() const { return begin_; }
    ullong end()   const { return end_; }
    ullong pos()   const { return pos_; }
  };

  class trie {
    vcode                    vc_;
    bv                       is_tail_;
    std::vector<uc>          ls_;
    vcode                    tail_pos_;
    std::vector<std::string> tail_;
    trie                     *ptail_trie_;
    vcode                    value_pos_;
    trie                     *pvalue_trie_;
    std::string              revstring_;

    trie(const trie &);
    trie &operator=(const trie &);
    ullong bsearch(uc label, ullong l, ullong r) const;
    void csearch(const char *key, std::vector<result> &results,
                 bool is_cps, ullong depth, ullong max);
    void dsearch(ullong pos, const std::string &str,
                 std::vector<std::pair<std::string, ullong> > &results);
    int cmp_substr(const char *s1, const char *s2) const {
      //  0: s1 == s2
      //  1: s1 is substring of s2
      // -1: else
      while (1) {
        if (*s1 == '\0') {
          return ((*s2 == '\0') ? 0 : 1);
        }
        if (*s1 != *s2) { return -1; }
        s1++;
        s2++;
      }
    }
    const std::string &revstring(ullong pos) {
      this->revstring_ = this->tail(pos);
      reverse(this->revstring_);
      while (pos != 0) {
        this->revstring_ += this->label(pos);
        pos = this->parent(pos);
      }
      return this->revstring_;
    }

  public:
    trie();
    trie(std::ifstream &ifs);
    trie(const char *filename);
    trie(trie *ptail_trie);
    trie(trie *ptail_trie, trie *pvalue_trie);
    ~trie();

    ullong lookup(const char *key);
    void   common_prefix_search(const char *key,
                                std::vector<result> &results);
    void   predictive_search(const char *key,
               std::vector<std::pair<std::string, ullong> > &results);
    void   extract(const char *key, std::vector<result> &results);
    bool   check(const char *key);
    bool   read(std::ifstream &ifs);
    bool   read(const char *filename);
    void   write(std::ofstream &ofs) const;
    void   write(const char *filename, bool is_append = false) const;

    ullong size() const { return this->ls_.size(); }
    void push(uc label, ullong degree, bool is_tail, const char *tail) {
      this->vc_.push(degree);
      this->ls_.push_back(label);
      this->is_tail_.set(this->size() - 1, is_tail);
      if (is_tail) { this->tail_.push_back(tail); }
    }
    void push(uc label, ullong degree, bool is_tail, ullong tail_pos,
              bool is_value = false, ullong value_pos = 0) {
      this->vc_.push(degree);
      this->ls_.push_back(label);
      this->is_tail_.set(this->size() - 1, is_tail);
      if (is_tail)  { this->tail_pos_.push(tail_pos); }
      if (is_value) { this->value_pos_.push(value_pos); }
    }
    ullong child(ullong pos)   const { return this->vc_.select(pos); }
    ullong parent(ullong pos)  const { return ullong(this->vc_.rank(pos + 1) - 1); }
    ullong degree(ullong pos)  const { return this->vc_.diff(pos + 1); }
    uc     label(ullong pos)   const { return this->ls_[pos]; }
    bool   is_tail(ullong pos) const { return this->is_tail_.get(pos); }
    const std::string &tail(ullong pos) const {
      if (this->ptail_trie_) {
        ullong tpos = this->tail_pos_.diff(
                      this->is_tail_.rank(pos) - 1);
        return this->ptail_trie_->revstring(tpos);
      } else {
        return this->tail_[this->is_tail_.rank(pos) - 1];
      }
    }
    bool is_value() const { return (this->pvalue_trie_ != NULL); }
    const std::string &value(ullong pos) const {
      if (this->pvalue_trie_) {
        ullong tpos = this->value_pos_.diff(
                      this->is_tail_.rank(pos) - 1);
        return this->pvalue_trie_->revstring(tpos);
      }
      throw "erika::trie::value(): value is not set.";
    }
  };
}

#endif

