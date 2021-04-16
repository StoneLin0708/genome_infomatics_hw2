add_executable(test_genomeseq test_genomeseq.cc)
target_link_libraries(test_genomeseq genomeseq fmt gtest_main)
gtest_discover_tests(test_genomeseq)

add_executable(test_matrix test_matrix.cc)
target_link_libraries(test_matrix genomeseq fmt gtest_main)
gtest_discover_tests(test_matrix)

add_executable(test_hmm test_hmm.cc)
target_link_libraries(test_hmm genomeseq fmt gtest_main)
gtest_discover_tests(test_hmm)