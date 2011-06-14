cmake_minimum_required(VERSION 2.6)
project(SimpleTBL)

include (CheckIncludeFile)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

include_directories(${SimpleTBL_SOURCE_DIR})

set (CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

find_package(Boost 1.36.0 REQUIRED)
if(BOOST_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set (LIBSIMPLETBL_HEADERS
  simpletbl/corpus/BrownCorpusReader.hh
  simpletbl/corpus/Corpus.hh
  simpletbl/corpus/Sentence.hh
  simpletbl/tbl/TBLContextualTagger.hh
  simpletbl/tbl/TBLInitialTagger.hh
  simpletbl/tbl/TBLLearner.hh
  simpletbl/tbl/TBLRule.hh
  simpletbl/tbl/TBLRuleGenerator.hh
  simpletbl/utility/PairSecondGtComp.hh
  simpletbl/wordhandler/DummyWordHandler.hh
  simpletbl/wordhandler/KnownWordHandler.hh
  simpletbl/wordhandler/WordHandler.hh
)

set (LIBSIMPLETBL_SOURCES
  lib/corpus/BrownCorpusReader.cpp
  lib/tbl/TBLContextualTagger.cpp
  lib/tbl/TBLInitialTagger.cpp
  lib/tbl/TBLLearner.cpp
  lib/tbl/TBLRule.cpp
  lib/tbl/TBLRuleGenerator.cpp
  lib/wordhandler/DummyWordHandler.cpp
  lib/wordhandler/KnownWordHandler.cpp
)


add_library(simpletbl STATIC
  ${LIBSIMPLETBL_HEADERS}
  ${LIBSIMPLETBL_SOURCES}
)

add_executable(brill-learn
  util/brill-learn.cpp
)
target_link_libraries(brill-learn simpletbl)

add_executable(make-lexicon
  util/make-lexicon.cpp
)
target_link_libraries(make-lexicon simpletbl)

add_executable(tbl-evaluate
  util/tbl-evaluate.cpp
)
target_link_libraries(tbl-evaluate simpletbl)

