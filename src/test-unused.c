/*
 * Test with unused address
 * Run the ACD engine with an address that is not used by anyone else on the
 * link. This should just pass through, with a short, random timeout.
 */

#include <stdlib.h>
#include "test.h"

static void test_unused(int ifindex, const struct ether_addr *mac) {
        NAcdConfig config = {
                .ifindex = ifindex,
                .mac = *mac,
                .ip = { htobe32((192 << 24) | (168 << 16) | (1 << 0)) },
        };
        struct pollfd pfds;
        NAcd *acd;
        int r, fd;

        r = n_acd_new(&acd);
        assert(r >= 0);

        n_acd_get_fd(acd, &fd);
        r = n_acd_start(acd, &config);
        assert(r >= 0);

        for (;;) {
                NAcdEvent event;
                pfds = (struct pollfd){ .fd = fd, .events = POLLIN };
                r = poll(&pfds, 1, -1);
                assert(r >= 0);

                r = n_acd_dispatch(acd);
                assert(r >= 0);

                r = n_acd_pop_event(acd, &event);
                if (r >= 0) {
                        assert(event.event == N_ACD_EVENT_READY);
                        break;
                } else {
                        assert(r == -EAGAIN);
                }
        }

        n_acd_free(acd);
}

int main(int argc, char **argv) {
        struct ether_addr mac;
        int r, ifindex;

        r = test_setup();
        if (r)
                return r;

        test_veth_new(&ifindex, &mac, NULL, NULL);
        test_unused(ifindex, &mac);

        return 0;
}
