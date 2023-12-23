#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/net.h>

MODULE_LICENSE("GPL");

static struct nf_hook_ops nfho;

/**
 * @brief Hook function called for each incoming packet
 * @param priv Private data associated with the hook
 * @param skb Socket buffer containing the packet data
 * @param state State information of the packet and hook
 * @return NF_ACCEPT verdict to allow packet processing
 */
unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *ip_header;
    struct tcphdr *tcp_header;

    // Retrieve the IP header from the socket buffer
    ip_header = ip_hdr(skb);

    // We only want to log if the IP packet's protocol is TCP
    if (ip_header->protocol == IPPROTO_TCP) {
        // Retrieve the TCP header
        tcp_header = tcp_hdr(skb);

        // This is where the magic happens - we log source IP and source port
        pr_info("Source IP: %pI4, Source Port: %d\n",
               &ip_header->saddr, ntohs(tcp_header->source));
    }

    // allow normal processing by accepting the packet
    return NF_ACCEPT;
}

/**
 * @brief Initialize the module
 * @return 0 on success, non-zero on failure
 */
static int __init tcp_sniffer_init(void)
{

    // Configure the nf_hook_ops structure
    nfho.hook = hook_func;               // Set the hook function
    nfho.hooknum = NF_INET_PRE_ROUTING;  // Hook at the pre-routing stage
    nfho.pf = PF_INET;                   // IPv4 packets
    nfho.priority = NF_IP_PRI_FIRST;     // Highest priority

    // Register the hook function with the Netfilter framework
    nf_register_net_hook(&init_net /* note for self: this is a global var representing the initial network namespace*/, &nfho);
    
    pr_info("Module tcp_sniffer loaded\n");

    return 0;
}

/**
 * @brief Cleanup the module
 */
static void __exit tcp_sniffer_exit(void)
{
    // Unregister the hook function from the Netfilter framework
    nf_unregister_net_hook(&init_net /* note to self: this is a global var representing the initial network namespace*/, &nfho);
    
    pr_info("Module tcp_sniffer unloaded\n");
}

module_init(tcp_sniffer_init);
module_exit(tcp_sniffer_exit);