#include <Rcpp.h>
#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>
#define SMALL_BUF_SIZE (1024 * 8)

using namespace Rcpp;

//' SDBM Hash Function
//'
//' This algorithm was created for sdbm (a public-domain reimplementation of `ndbm`)
//' database library. it was found to do well in scrambling bits, causing better
//' distribution of the keys and fewer splits. It also happens to be a good general
//' hashing function with good distribution.
//'
//' @md
//' @param x character vector of strings to hash
//' @return numeric vector of the hashes of \code{x}
//' @export
//' @references [http://www.cse.yorku.ca/~oz/hash.html](http://www.cse.yorku.ca/~oz/hash.html)
//' @examples
//' setdiff(sdbm_hash(state.name), sdbm_hash(state.name))
//' ## numeric(0)
//[[Rcpp::export]]
std::vector < unsigned int > sdbm_hash(std::vector < std::string> x) {

  unsigned int input_size = x.size();
  std::vector < unsigned int > output(input_size);

  unsigned char *str;
  unsigned int hash;
  int c;

  for (unsigned int i=0; i<input_size; i++) {

    if ((i % 10000) == 0) Rcpp::checkUserInterrupt();

    str = (unsigned char *)x[i].c_str();
    hash = 0;

    while ((c = *str++)) hash = c + (hash << 6) + (hash << 16) - hash;

    output[i] = hash;

  }

  return(output);

}

//' DJB2 Hash Function
//'
//' Dan Berstein's hash function. A variation on a [linear congruential generator](https://rosettacode.org/wiki/Linear_congruential_generator)
//' It's fast but does a good job of producing decently distributed hash values.
//'
//' @param x character vector of strings to hash
//' @return numeric vector of the hashes of \code{x}
//' @export
//' @references [http://www.cse.yorku.ca/~oz/hash.html](http://www.cse.yorku.ca/~oz/hash.html) and
//'             [https://groups.google.com/forum/#!msg/comp.lang.c/lSKWXiuNOAk/zstZ3SRhCjgJ](https://groups.google.com/forum/#!msg/comp.lang.c/lSKWXiuNOAk/zstZ3SRhCjgJ)
//' @examples
//' setdiff(djb2_hash(state.name), djb2_hash(state.name))
//' ## numeric(0)
//[[Rcpp::export]]
std::vector < unsigned int > djb2_hash(std::vector < std::string> x) {


  unsigned int input_size = x.size();
  std::vector < unsigned int > output(input_size);

  unsigned char *str;
  int c;

  for (unsigned int i=0; i<input_size; i++) {

    if ((i % 10000) == 0) Rcpp::checkUserInterrupt();

    str = (unsigned char *)x[i].c_str();
    unsigned long hash = 5381;

    while ((c = *str++)) hash = ((hash << 5) + hash) + c;

    output[i] = hash;

  }

  return(output);

}

//' Count lines (fast, like `wc` because it *is* `wc`)
//'
//' This is just the line count portion of the `wc` source code ripped out and put
//' into a C-backed R function.
//'
//' @md
//' @param f full path to file
//' @export
//' @references [GNU `wc` source code](https://www.gnu.org/software/cflow/manual/html_node/Source-of-wc-command.html)
//' @examples
//' line_count(system.file("extdata", "lines.csv", package = "hrbrmiscpp"))
//[[Rcpp::export]]
SEXP line_count(SEXP f) {

  uintmax_t linect = 0;
  uintmax_t tlinect = 0;

  int fd, len;
  u_char *p;

  struct statfs fsb;

  static off_t buf_size = SMALL_BUF_SIZE;
  static u_char small_buf[SMALL_BUF_SIZE];
  static u_char *buf = small_buf;

  PROTECT(f = AS_CHARACTER(f));

  if ((fd = open(CHAR(STRING_ELT(f, 0)), O_RDONLY, 0)) >= 0) {

    if (fstatfs(fd, &fsb)) {
      fsb.f_iosize = SMALL_BUF_SIZE;
    }

    if (fsb.f_iosize != buf_size) {
      if (buf != small_buf) {
        free(buf);
      }
      if (fsb.f_iosize == SMALL_BUF_SIZE || !(buf = (u_char *)malloc(fsb.f_iosize))) {
        buf = small_buf;
        buf_size = SMALL_BUF_SIZE;
      } else {
        buf_size = fsb.f_iosize;
      }
    }

    while ((len = read(fd, buf, buf_size))) {

      if (len == -1) {
        (void)close(fd);
        break;
      }

      for (p = buf; len--; ++p)
        if (*p == '\n')
          ++linect;
    }

    tlinect += linect;

    (void)close(fd);

  }

  SEXP result;
  PROTECT(result = NEW_INTEGER(1));
  INTEGER(result)[0] = tlinect;
  UNPROTECT(2);

  return(result);

}
