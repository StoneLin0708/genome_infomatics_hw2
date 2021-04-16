# Markov Model & Hidden Markov Model for Genome Sequence
## project structure
* data/ : store genome sequence data, it wont be uploaded
* scripts/ : tools written by python, for splitting the dataset and a reference answer to my result
* src/ : my c++ code
* test/ : some testing for my c++ code

## Requirements
* c++ compiler with c++17 support
* cmake > 3.14
* [libfmt](https://github.com/fmtlib/fmtl) for output

## Download and split genome sequence
```bash
wget ftp://ftp.ncbi.nlm.nih.gov/refseq/H_sapiens/annotation/GRCh38_latest/refseq_identifiers/GRCh38_latest_genomic.fna.gz
7za x GRCh38_latest_genomic.fna.gz
mv GRCh38_latest_genomic.fna data
python3 scripts/split.py
```

## Build Project
```bash
$ mkdir build && cd build
build$ cmake -DCMAKE_BUILD_TYPE=Release ..
build$ make -j
```

## Run Markov model
```bash
build$ time ./src/mm ../data/train ../data/test
order 0:
train log probability : -1987487.01
test  log probability : -1986855.28
order 1:
train log probability : -1945020.29
test  log probability : -1945200.88
order 2:
train log probability : -1932855.01
test  log probability : -1933079.21
./src/mm ../data/train ../data/test 0.03s user 0.00s system 99% cpu 0.030 total
```

## Order 0,1,2 Markov Model
NC_000006.12 Homo sapiens chromosome 6, GRCh38.p13 Primary Assembly.
* train : [  100,000 – 1,099,999]
* test  : [1,100,000 – 2,099,999]

It cost 0.03 sec in total for all train and test on i7-8700

|                order |           0 |           1 |           2 |
|----------------------|-------------|-------------|-------------|
|train log probability | -1987487.01 | -1945020.29 | -1932855.01 |
|test  log probability | -1986855.28 | -1945200.88 | -1933079.21 |


## Run Hidden Markov Model
```bash
build$ time ./src/hmm ../data/train ../data/test
train log probability -182.67
test log probability -180.86
./src/hmm ../data/train ../data/test 0.02s user 0.00s system 99% cpu 0.021 total
```

## Hidden Markov Model Viterbi with random initialize
There is only random initialization for all variables, learning based or any manually tunning are not applied.

NC_000006.12 Homo sapiens chromosome 6, GRCh38.p13 Primary Assembly.
* train : [  100,000 – 1,099,999]
* test  : [1,100,000 – 2,099,999]

It cost 0.02 sec in total for all train and test on i7-8700

|       | log probability |
|-------|-----------------|
| train |         -182.67 |
| test  |         -180.86 |


## Program note
*  I'm using one int32_t to store 16 sequence