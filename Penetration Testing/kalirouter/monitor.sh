#!/bin/bash

# based on https://www.psattack.com/articles/20160410/setting-up-a-wireless-access-point-in-kali/
# Interface that we want to monitor on
WIRELESS_MONITOR_INTERFACE=wlan0
WIRED_MONITOR_INTERFACE=eth1
# Bridge between the above two interfaces (created on demand)
BRIDGE_INTERFACE=br0
# Interface that is connected to our regular network (e.g. Internet)
INTERNET_INTERFACE=eth0
# Network address range we use for our monitor network
MONITOR_NETWORK=172.16.0.0/12
# The address we assign to our router, dhcp, and dns server.
MONITOR_MAIN=172.16.0.1/12
# PROXY BOX (e.g. burp). The ip address of the machine running a transparent proxy.
PROXYBOX=172.16.0.1
# port on which the proxy is listening
PROXYBOX_HTTP_PORT=80
PROXYBOX_HTTPS_PORT=443
# configuration directory
CONFIGDIR=./conf
# directory to which to write wireshark dumps
DUMPDIR=./dumps

# It monitors until we hit Ctrl c
trap ctrl_c INT
function ctrl_c(){
    echo Killing processes.
    killall dnsmasq
    killall hostapd
    echo Bringing down interfaces.
    ifconfig $WIRELESS_MONITOR_INTERFACE down
    ifconfig $WIRED_MONITOR_INTERFACE down
    ifconfig $BRIDGE_INTERFACE down
    echo Deleting bridge
    brctl delbr $BRIDGE_INTERFACE
}
# make the bridge unamnaged - see https://askubuntu.com/questions/472794/hostapd-error-nl80211-could-not-configure-driver-mode
nmcli radio wifi off
rfkill unblock wlan

# delete all addresses for wireless and wired
ip addr flush dev $WIRELESS_MONITOR_INTERFACE
ip addr flush dev $WIRED_MONITOR_INTERFACE
# bring the ethernet interface up
ip link set dev $WIRED_MONITOR_INTERFACE up
# create bridge interface
brctl addbr $BRIDGE_INTERFACE
# add the wire to the bridge
brctl addif $BRIDGE_INTERFACE $WIRED_MONITOR_INTERFACE
# bring the bridge up
ip link set dev $BRIDGE_INTERFACE up
# bring up the wireless network interface
 ip link set dev $WIRELESS_MONITOR_INTERFACE up
# configure it to be an access point (and add it to the bridge)
hostapd $CONFIGDIR/hostapd.conf -B
ip addr add $MONITOR_MAIN dev br0

# configure our DHCP server
dnsmasq -C $CONFIGDIR/dnsmasq.conf

# Add a forward rule for ipv4 traffic from MONITOR towards INTERNET
sysctl -w net.ipv4.ip_forward=1
iptables -P FORWARD ACCEPT
iptables -t nat -A POSTROUTING -o $INTERNET_INTERFACE -j MASQUERADE

# redirect HTTP traffic to burp running on another machine; http://www.tldp.org/HOWTO/TransparentProxy-6.html
# Note: this approach only works for HTTP/1.1. Read the URI above to make our approach more generic (but this requires
# send the packets to proxybox from our bridge interface. port is the port that we want to intercept. Do this for each port we want to intercept. Leave the port out to proxy all tcp traffic.
# intercept http traffic
iptables -t nat -A PREROUTING -i $BRIDGE_INTERFACE -p tcp --dport 80 -j DNAT --to-destination $PROXYBOX:$PROXYBOX_HTTP_PORT
# intercept https traffic
iptables -t nat -A PREROUTING -i $BRIDGE_INTERFACE -p tcp --dport 443 -j DNAT --to-destination $PROXYBOX:$PROXYBOX_HTTPS_PORT

# make sure that the reply gets sent back through our monitor, instead of directly to the client we monitor (important!)
iptables -t nat -A POSTROUTING -o $INTERNET_INTERFACE -s $MONITOR_NETWORK -d $PROXYBOX -j MASQUERADE #use masquerade as our monitor gets a dynamic IP address

# make sure our monitor will forward the appropriate packets to the proxybox. It may not be needed.
# http traffic
iptables -A FORWARD -s $MONITOR_NETWORK -d $PROXYBOX -i $BRIDGE_INTERFACE -o $INTERNET_INTERFACE -p tcp --dport $PROXYBOX_HTTP_PORT -j ACCEPT
# https traffic
iptables -A FORWARD -s $MONITOR_NETWORK -d $PROXYBOX -i $BRIDGE_INTERFACE -o $INTERNET_INTERFACE -p tcp --dport $PROXYBOX_HTTPS_PORT -j ACCEPT


# Configure tshark (wireshark) to write whatever passes over our monitored interface to a pcap file.
tshark -i $BRIDGE_INTERFACE -w $DUMPDIR/output.pcap -P
