#pragma once

/*
 * IPv4 Address Conflict Detection
 *
 * This is the public header of the n-acd library, implementing IPv4 Address
 * Conflict Detection as described in RFC-5227. This header defines the public
 * API and all entry points if n-acd.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct ether_addr;
struct ether_arp;
struct in_addr;

typedef struct NAcd NAcd;
typedef void (*NAcdFn) (NAcd *acd, void *userdata, unsigned int event, const struct ether_arp *conflict);

enum {
        N_ACD_DEFEND_NEVER,
        N_ACD_DEFEND_ONCE,
        N_ACD_DEFEND_ALWAYS,
        _N_ACD_DEFEND_N,
};

enum {
        N_ACD_EVENT_READY,
        N_ACD_EVENT_USED,
        N_ACD_EVENT_DEFENDED,
        N_ACD_EVENT_CONFLICT,
        N_ACD_EVENT_DOWN,
        _N_ACD_EVENT_N,
};

int n_acd_new(NAcd **acdp);
NAcd *n_acd_ref(NAcd *acd);
NAcd *n_acd_unref(NAcd *acd);

bool n_acd_is_running(NAcd *acd);
void n_acd_get_fd(NAcd *acd, int *fdp);
void n_acd_get_ifindex(NAcd *acd, unsigned int *ifindexp);
void n_acd_get_mac(NAcd *acd, struct ether_addr *macp);
void n_acd_get_ip(NAcd *acd, struct in_addr *ipp);

int n_acd_set_ifindex(NAcd *acd, unsigned int ifindex);
int n_acd_set_mac(NAcd *acd, const struct ether_addr *mac);
int n_acd_set_ip(NAcd *acd, const struct in_addr *ip);

int n_acd_dispatch(NAcd *acd);
int n_acd_start(NAcd *acd, NAcdFn fn, void *userdata);
void n_acd_stop(NAcd *acd);
int n_acd_announce(NAcd *acd, unsigned int defend);

static inline void n_acd_unrefp(NAcd **acd) {
        if (*acd)
                n_acd_unref(*acd);
}

#ifdef __cplusplus
}
#endif
