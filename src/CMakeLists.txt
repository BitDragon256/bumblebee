project(bumblebee C)

set(BB_SOURCES
    symbols.c
    lexical_analysis.c
    tokenStream.c
    )

set(BB_HEADERS
    symbols.h
    tokenStream.h
    )

add_executable(bblex ${BB_SOURCES} ${BB_HEADERS})
