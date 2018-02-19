#include <libcouchbase/couchbase.h>
#include <libcouchbase/iops.h>

namespace lcb {

class Buffer {
public:
    lcb_IOV *get_buf_segs() { return niov; }
    size_t get_num_segs() { return niov; }
    std::vector<lcb_IOV *> get_vector();

    using const_iterator = lcb_IOV*;
    using value_type = lcb_IOV;

    const_iterator begin() { return iov; }
    const_iterator end() { return iov + niov; }

private:
    lcb_IOV *iov;
    size_t niov;
};

/**
 * Implementation must support run(), stop()
 */
template <class Implementation>
class CompletionRoutines {
    using ConnectedHandler = lcb_io_connect_cb;
    using WriteHandler = lcb_ioC_write2_callback;
    using ReadHandler = lcb_ioC_read2_callback;
    using SD = lcb_sockdata_st;
    using socktype_ = typename Implementation::socket_type;
    using timer_ = typename Implementation::timer_type;
    struct ConnectContext { ConnectedHandler handler; };
    struct ReadContext { ReadHandler handler; void *arg; };
    struct WriteContext { WriteHandler handler; void *arg; };
    struct TimerContext { lcb_ioE_callback handler; void *arg; };

    socktype_ newsock(int domain, int family, int protocol);

    bool connect_socket(socktype_&, const sockaddr*, unsigned, const ConnectContext&);
    void socket_connected(socktype_&, bool, const ConnectContext&);
    bool start_read(socktype_&, Buffer&, const ReadContext&);
    void got_read(socktype_&, bool, const ReadContext&);
    bool start_write(socktype_&, const Buffer&, const WriteContext&);
    void got_write(socktype_&, const WriteContext&);
    void close_socket(socktype_&);
    timer_ create_timer();
    void run();
    void pause();
};

}
