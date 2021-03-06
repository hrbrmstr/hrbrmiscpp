% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/RcppExports.R
\name{djb2_hash}
\alias{djb2_hash}
\title{DJB2 Hash Function}
\usage{
djb2_hash(x)
}
\arguments{
\item{x}{character vector of strings to hash}
}
\value{
numeric vector of the hashes of \code{x}
}
\description{
Dan Berstein's hash function. A variation on a [linear congruential generator](https://rosettacode.org/wiki/Linear_congruential_generator)
It's fast but does a good job of producing decently distributed hash values.
}
\examples{
setdiff(djb2_hash(state.name), djb2_hash(state.name))
## numeric(0)
}
\references{
[http://www.cse.yorku.ca/~oz/hash.html](http://www.cse.yorku.ca/~oz/hash.html) and
            [https://groups.google.com/forum/#!msg/comp.lang.c/lSKWXiuNOAk/zstZ3SRhCjgJ](https://groups.google.com/forum/#!msg/comp.lang.c/lSKWXiuNOAk/zstZ3SRhCjgJ)
}
