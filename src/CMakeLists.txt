add_library(genomeseq genomeseq.cc genomeseq.h simple_matrix.h hidden_markov_model.h)
target_include_directories(genomeseq PUBLIC .)

add_executable(mm markov_model.cc)

target_link_libraries(mm genomeseq fmt)

add_executable(hmm hidden_markov_model.cc)

target_link_libraries(hmm genomeseq fmt)