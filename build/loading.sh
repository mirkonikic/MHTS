#!/bin/bash

t=$(( $1 * 10000 ))

echo -ne '\t[+]                          ( 0%)\r'
usleep $t
echo -ne '\t[+]##                        (12%)\r'
usleep $t
echo -ne '\t[+]####                      (24%)\r'
sleep 0.7
echo -ne '\t[+]#####                     (33%)\r'

echo -ne '\t[+]######                    (39%)\r'
usleep $t
echo -ne '\t[+]########                  (45%)\r'
sleep 0.3
echo -ne '\t[+]############              (58%)\r'
sleep 0.5
echo -ne '\t[+]#############             (66%)\r'

echo -ne '\t[+]###############           (72%)\r'
usleep $t
echo -ne '\t[+]##################        (80%)\r'
sleep 0.3
echo -ne '\t[+]####################      (93%)\r'
usleep $t
echo -ne '\t[+]#######################   (100%)\r'
sleep 1
echo -ne '\n'
