# library name
set(TARGET Network)

# library source files
set(SOURCES
    Message.cpp
    Message.h
    MessageFwd.h
    Network.cpp
    Network.h
    Peer.cpp
    Peer.h
    Protocol.h
    ProxyMessage.cpp
    ProxyMessage.h
    ReadMe.txt
    Stream.h
    Types.h
)

# create static library with standard build settings
td_add_library(${TARGET}
    LIBRARY_TYPE
        STATIC
    SOURCES
        ${SOURCES}
)
