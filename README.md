
`hrbrmiscpp` : R C/C++ mish-mash of stuff for hrbrmstr

A place for some C/C++-backed R functions until they find a better home.

The following functions are implemented:

-   `sdbm_hash`: SDBM Hash Function
-   `djb2_hash`: DJB2 Hash Function
-   `line_count`: Count lines (fast, like `wc` because it *is* `wc`)

The following data sets are included:

### Installation

``` r
devtools::install_github("hrbrmstr/hrbrmiscpp")
```

### Usage

``` r
library(hrbrmiscpp)

# current verison
packageVersion("hrbrmiscpp")
```

    ## [1] '0.1.0'

``` r
setdiff(sdbm_hash(state.name), sdbm_hash(state.name))
```

    ## numeric(0)

``` r
setdiff(djb2_hash(state.name), djb2_hash(state.name))
```

    ## numeric(0)

``` r
line_count(system.file("extdata", "lines.csv", package = "hrbrmiscpp"))
```

    ## [1] 26

### Test Results

``` r
library(hrbrmiscpp)
library(testthat)

date()
```

    ## [1] "Wed Mar 22 09:36:23 2017"

``` r
test_dir("tests/")
```

    ## testthat results ========================================================================================================
    ## OK: 0 SKIPPED: 0 FAILED: 0
    ## 
    ## DONE ===================================================================================================================
