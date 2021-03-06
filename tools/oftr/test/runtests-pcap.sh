#!/bin/bash

set -e

# Compare the SHA1 hash of a file to a known good value.
# Usage: verify_sha1 <filename> <hash_value>
function verify_sha1 {
    if [ ! -f "$1" ]; then
        echo "verify_sha1 failed: file not found: $1"
        exit 1
    fi
    # On systems with sha1sum command, use it instead of openssl.
    if command -v sha1sum; then
        sha1=`sha1sum "$1" | cut -d ' ' -f 1`
    else
        sha1=`openssl sha1 "$1" | cut -d ' ' -f 2`
    fi
    if [ "$2" != "$sha1" ]; then
        echo "verify_sha1 failed: $1: expected $2 but got $sha1"
        exit 1
    fi
    echo "verify_sha1 success: $1"
}

CURRENT_SOURCE_DIR=`dirname "$0"`
CURRENT_TEST_DIR=`pwd`
MSG_DIR="$CURRENT_TEST_DIR/oftr$$.msgs"

echo "Working Directory: $CURRENT_TEST_DIR"

if [ -d "$MSG_DIR" ]; then
    /bin/rm -f $MSG_DIR/_tcp-*
else
    mkdir $MSG_DIR
fi

LIBOFP=$CURRENT_TEST_DIR/../oftr

echo "Run oftr decode on $CURRENT_SOURCE_DIR/tcp.pcap $CURRENT_SOURCE_DIR/flow58899.pcap"
$LIBOFP_MEMCHECK $LIBOFP decode --pcap-format=yes --pcap-output-dir=$MSG_DIR --pcap-max-missing-bytes=100000 --pcap-filter '' "$CURRENT_SOURCE_DIR/tcp.pcap" "$CURRENT_SOURCE_DIR/flow58899.pcap"

verify_sha1 "$CURRENT_SOURCE_DIR/tcp.pcap" "08cf1e8ab8b499cfa8d03398e74c09ad59d3a731"
verify_sha1 "$CURRENT_SOURCE_DIR/flow58899.pcap" "69d160c2773defae44daf7d037c649df0f9a6bd0"
verify_sha1 "$CURRENT_SOURCE_DIR/cap_single.pcap" "35da4a52e18203c70f67badb0eb53808a43014c2"

verify_sha1 "$MSG_DIR/_tcp-1-127.0.0.1:61278-127.0.0.1:8888" "b0372f0c19e464a30bb2faa54510dd80e1496417"
verify_sha1 "$MSG_DIR/_tcp-1-127.0.0.1:8888-127.0.0.1:61278" "759cca92810046305aab8fd1789ef38965ae81a0"
verify_sha1 "$MSG_DIR/_tcp-2-127.0.0.1:61279-127.0.0.1:8888" "7a4fbdd6be80779d4c3990ce128caaadd2e14b66"
verify_sha1 "$MSG_DIR/_tcp-2-127.0.0.1:8888-127.0.0.1:61279" "09f00fcfa70219bf46c0a3c2ffa6e0f9c1e932b3"
verify_sha1 "$MSG_DIR/_tcp-3-127.0.0.1:61282-127.0.0.1:8888" "f7ae012958dfe4f6546d4f3c9ede680ea3d410ec"
verify_sha1 "$MSG_DIR/_tcp-3-127.0.0.1:8888-127.0.0.1:61282" "03a8f61b7148b1f1b67bf267057ac46c79af0951"

verify_sha1 "$MSG_DIR/_tcp-4-127.0.0.1:61278-127.0.0.1:8888" "b0372f0c19e464a30bb2faa54510dd80e1496417"
verify_sha1 "$MSG_DIR/_tcp-4-127.0.0.1:8888-127.0.0.1:61278" "759cca92810046305aab8fd1789ef38965ae81a0"
verify_sha1 "$MSG_DIR/_tcp-5-127.0.0.1:61279-127.0.0.1:8888" "7a4fbdd6be80779d4c3990ce128caaadd2e14b66"
verify_sha1 "$MSG_DIR/_tcp-5-127.0.0.1:8888-127.0.0.1:61279" "09f00fcfa70219bf46c0a3c2ffa6e0f9c1e932b3"
verify_sha1 "$MSG_DIR/_tcp-6-127.0.0.1:61282-127.0.0.1:8888" "f7ae012958dfe4f6546d4f3c9ede680ea3d410ec"
verify_sha1 "$MSG_DIR/_tcp-6-127.0.0.1:8888-127.0.0.1:61282" "03a8f61b7148b1f1b67bf267057ac46c79af0951"

verify_sha1 "$MSG_DIR/_tcp-7-[::1]:61987-[::1]:8888" "1805708f40d93cc740dcaf6510288c19cd393187"
verify_sha1 "$MSG_DIR/_tcp-7-[::1]:8888-[::1]:61987" "bb1cf30845c3c20a22840ee5934095fcf9b80b12"
verify_sha1 "$MSG_DIR/_tcp-8-[::1]:61988-[::1]:8888" "aa207c3edc5b664ab732935e13ad4495f3826c59"
verify_sha1 "$MSG_DIR/_tcp-8-[::1]:8888-[::1]:61988" "bece5ea2bca550bac3477e2f1c98493d4ada8fde"
verify_sha1 "$MSG_DIR/_tcp-9-[::1]:61994-[::1]:8888" "33aededd23f957c2daae1ff7aab626a493c7f3f6"
verify_sha1 "$MSG_DIR/_tcp-9-[::1]:8888-[::1]:61994" "d9bcd4b5b044938c6d77dd3a7def9f44013a66c6"

verify_sha1 "$MSG_DIR/_tcp-10-[::1]:61987-[::1]:8888" "1805708f40d93cc740dcaf6510288c19cd393187"
verify_sha1 "$MSG_DIR/_tcp-10-[::1]:8888-[::1]:61987" "bb1cf30845c3c20a22840ee5934095fcf9b80b12"
verify_sha1 "$MSG_DIR/_tcp-11-[::1]:61988-[::1]:8888" "aa207c3edc5b664ab732935e13ad4495f3826c59"
verify_sha1 "$MSG_DIR/_tcp-11-[::1]:8888-[::1]:61988" "bece5ea2bca550bac3477e2f1c98493d4ada8fde"
verify_sha1 "$MSG_DIR/_tcp-12-[::1]:61994-[::1]:8888" "33aededd23f957c2daae1ff7aab626a493c7f3f6"
verify_sha1 "$MSG_DIR/_tcp-12-[::1]:8888-[::1]:61994" "d9bcd4b5b044938c6d77dd3a7def9f44013a66c6"

verify_sha1 "$MSG_DIR/_tcp-17-172.16.133.78:58899-184.96.221.120:64510" "c08dc3ddc802c6d89ef3e8dd1b9e41f2823e6e66"
verify_sha1 "$MSG_DIR/_tcp-17-184.96.221.120:64510-172.16.133.78:58899" "f18d85da70cf6c082640988b4c58baceabcfb8d6"
verify_sha1 "$MSG_DIR/_tcp-18-172.16.133.84:58899-172.16.139.250:5440" "ec6139bad6d010c782e2dc07e889fc45b75f8132"

/bin/rm -rf "$MSG_DIR"

# Check illegal argument combinations to make sure they are rejected.

# You cannot combine the --pcap-device option with any input arguments.
echo "Run oftr decode combinging --pcap-device option with file arguments."
! $LIBOFP_MEMCHECK $LIBOFP decode --pcap-device=en0 filename 2> /dev/null || {
    echo "Test Failed: Don't use --pcap-device with files. ($?)"
    exit 1
}

# Decode OpenFlow messages in `cap_single.pcap`.

echo "Run oftr decode on $CURRENT_SOURCE_DIR/cap_single.pcap"
cap_single_out="$CURRENT_TEST_DIR/cap_single$$.pcap.out"
$LIBOFP_MEMCHECK $LIBOFP decode --json-array "$CURRENT_SOURCE_DIR/cap_single.pcap" > "$cap_single_out"
echo "Compare $cap_single_out to $CURRENT_SOURCE_DIR/cap_single.pcap.out"
diff "$cap_single_out" "$CURRENT_SOURCE_DIR/cap_single.pcap.out"
rm "$cap_single_out"

# Decode packets in pcap files (--pcap-convert-packetin)

echo "Run oftr decode --pcap-convert-packetin on $CURRENT_SOURCE_DIR/tcp.pcap $CURRENT_SOURCE_DIR/flow58899.pcap $CURRENT_SOURCE_DIR/cap_single.pcap"
pcap_packetin_out="$CURRENT_TEST_DIR/pcap-packetin$$.out"
$LIBOFP_MEMCHECK $LIBOFP decode --json-array --pcap-convert-packetin --pkt-decode $CURRENT_SOURCE_DIR/tcp.pcap $CURRENT_SOURCE_DIR/flow58899.pcap $CURRENT_SOURCE_DIR/cap_single.pcap > "$pcap_packetin_out"
echo "Compare $pcap_packetin_out to $CURRENT_SOURCE_DIR/pcap-packetin.out"
diff "$pcap_packetin_out" "$CURRENT_SOURCE_DIR/pcap-packetin.out"
rm "$pcap_packetin_out"

# Decode OpenFlow message in `cap_single.pcap`; filter to PacketIn/PacketOut containing ARP packets.
# Also, extract these arp packets to a separate arp.pcap file.

echo "Run oftr decode with --pkt-filter='arp' on $CURRENT_SOURCE_DIR/cap_single.pcap"
pcap_filtered_out="$CURRENT_TEST_DIR/cap_single$$.pcap.filtered"
arp_pcap="arp$$.pcap"
$LIBOFP_MEMCHECK $LIBOFP decode --json-array --msg-include='PACKET_*' --pkt-filter='arp' --pkt-decode --pkt-write-file="$arp_pcap" "$CURRENT_SOURCE_DIR/cap_single.pcap" > "$pcap_filtered_out"
echo "Compare $pcap_filtered_out to $CURRENT_SOURCE_DIR/cap_single.pcap.filtered"
diff "$pcap_filtered_out" "$CURRENT_SOURCE_DIR/cap_single.pcap.filtered"
rm "$pcap_filtered_out"
echo "Compare $arp_pcap to $CURRENT_SOURCE_DIR/arp.pcap"
diff "$arp_pcap" "$CURRENT_SOURCE_DIR/arp.pcap"
rm "$arp_pcap"

echo "Done."
exit 0
