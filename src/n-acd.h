#pragma once

/*
 * IPv4 Address Conflict Detection
 *
 * This is the public header of the n-acd library. It defines the public API
 * and all entry points.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct ether_addr;
struct in_addr;

typedef struct NAcd NAcd;
typedef void (*NAcdFn) (NAcd *acd, void *userdata);

int n_acd_new(NAcd **acdp);
NAcd *n_acd_ref(NAcd *acd);
NAcd *n_acd_unref(NAcd *acd);

bool n_acd_is_running(NAcd *acd);
void n_acd_get_fd(NAcd *acd, int *fdp);
void n_acd_get_ifindex(NAcd *acd, int *ifindexp);
void n_acd_get_mac(NAcd *acd, struct ether_addr *macp);
void n_acd_get_address(NAcd *acd, struct in_addr *addressp);

int n_acd_set_ifindex(NAcd *acd, int ifindex);
int n_acd_set_mac(NAcd *acd, const struct ether_addr *mac);
int n_acd_set_address(NAcd *acd, const struct in_addr *address);

int n_acd_dispatch(NAcd *acd);
int n_acd_start(NAcd *acd, NAcdFn fn, void *userdata);
void n_acd_stop(NAcd *acd);

static inline void n_acd_unrefp(NAcd **acd) {
        if (*acd)
                n_acd_unref(*acd);
}

#ifdef __cplusplus
}
#endif